//////////////////////////////////////////////////////////
//  File      : sound.cpp				//
//  Written by: ludmiloff@gmail.com			//
//  Copyright : GPL2					//
//	Some parts of the implementation		//
//  are taken/inspired from Ogg123 code			//
//  THE Ogg123 SOURCE CODE IS (C) COPYRIGHT 2000-2001	//
//  by Stan Seibert <volsung@xiph.org> AND OTHER 	//
//  CONTRIBUTORS, http://www.xiph.org/			//
//							//
//  Sound volume mixer taken fro QMMP project - 	//
//  http://qmmp.ylsoftware.com/				//
//							//
//  Some modifications & stripping made by me: 		//
//  <ludmiloff@gmail.com>				//
//////////////////////////////////////////////////////////

#include <Sound>
#include <SoundTheme>
#include <QString>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "vorbisdecoder.h"

AmeSystemSound::AmeSystemSound(bool withMixer, QObject *parent)
	: QThread(parent),
	  mInited(false),
	  useMixer(withMixer)
{	
	initialize();
}

AmeSystemSound::~AmeSystemSound()
{
	uninitialize();
	ao_shutdown();
}

bool AmeSystemSound::play(const QString &src)
{
	if (!src.isEmpty())
		source = src;

	if (source.isEmpty())
		return false;
	
	if (input)
		delete input;
	input = new QFile(source);
	
	if (!decoder) {
		decoder = new VorbisDecoder(input);
	} else {
		// reinit the decoder
		decoder->reinitialize(input);
	}
	if (mInited && decoder->state == VorbisDecoder::Ready)
		start();
	else {
		qDebug() << "AmeSystemSound: Not inited yet, cannot play."; 
	}
	return mInited;
}

bool AmeSystemSound::play()
{
	return play("");
}

void AmeSystemSound::stop()
{
	mUserStop = true;
}

void AmeSystemSound::pause()
{
}

void AmeSystemSound::resume()
{
}

void AmeSystemSound::setEmbedSound(int soundId, bool force)
{
	AmeSoundThemeItem *sound = AmeSoundTheme::global()->getSound(soundId);
	if (sound != NULL) {
		if (sound->enabled || force)
			source = sound->fileName;
		else
			source = "";
	} else
		source = "";
}

int AmeSystemSound::volume()
{
	if (!pcm_element)
		return 0;
	
	long l, r;
	snd_mixer_handle_events(mixer);
	snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, &l);
	snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, &r);

	return (l+r)/2;
}

void AmeSystemSound::setVolume(int v)
{
	if (!pcm_element)
		return;
	snd_mixer_selem_set_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, v);
	snd_mixer_selem_set_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, v);
}


bool AmeSystemSound::isMuted()
{
	return false;
}

void AmeSystemSound::onVolumeChanged(int volume)
{
	emit systemVolumeChanged(volume);
}


void AmeSystemSound::initialize()
{
	if (mInited)
		uninitialize();
	
	source = "";
	decoder = 0;
	input = 0;
	mixer = 0;
	aoDevice = 0;
	aoDriverId = -1;
		
	mPause = false;
	mPlay = false;
	mUserStop = false;
	
	// set preferred audio format (used by decoder)
	aoFormat.bits = AO_WORD_SIZE * 8;
	aoFormat.rate = -1;
	aoFormat.channels = -1;
	aoFormat.byte_format = (ao_is_big_endian() ? AO_FMT_BIG : AO_FMT_LITTLE);
	
	aoOptions = NULL;
	
	ao_initialize();
	
	QSettings settings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
	
	deviceName = settings.value("AudioDevice/device", "alsa").toString();
	char *dev = strdup(deviceName.toAscii().data());
	aoDriverId = ao_driver_id(dev);
	if (aoDriverId < 0) {
		// no driver for this device, try default device
		qDebug() << "AmeSoundOutput: no driver found, try default device/driver";
		aoDriverId = ao_default_driver_id();
		
		if (aoDriverId < 0)
			return;
	}
		
	if (deviceName.startsWith("alsa")) {
		// TODO:
		//mUse_mmap = settings.value("use_mmap", false).toBool();
	}
	
	// TODO:
	//buffer_time = settings.value("buffer_time",500).toUInt()*1000;
	//period_time = settings.value("period_time",100).toUInt()*1000;
	
	if (useMixer) {
		QString mixerCard = settings.value("Mixer/mixer_card", "hw:0").toString();
		QString mixerDevice = settings.value("Mixer/mixer_device", "PCM").toString();
		setupMixer(mixerCard, mixerDevice);
	}
	
	mPause = false;
	mPlay = false;
	mUserStop = false;
	mInited = true;
	return;
}

void AmeSystemSound::uninitialize()
{	
	if (aoDevice) {
		// TODO: free aoOptions structure, if not null
		ao_close(aoDevice);
		aoDevice = 0;
		aoDriverId = -1;
	}
	
	if (mixer) {
		snd_mixer_close(mixer);	
		mixer = 0;
	}
	
	if (input) {
		delete input;
		input = 0;
	}
	
	if (decoder) {
		delete decoder;
		decoder = 0;
	}
	
	mPause = false;
	mPlay = false;
	mUserStop = false;
	
	mInited = false;
}

void AmeSystemSound::run()
{
	int len;
	
	while (decoder->state == VorbisDecoder::Decoding || decoder->state == VorbisDecoder::Ready) {
			   
		ao_sample_format newFmt = aoFormat;
		   
		len = decoder->read(&newFmt);
			   
		if (len == 0)
			break;
		if (len < 0) {
			// error in decoder
			qDebug() << "AmeSystemSound:: decoding failure";
			break;
		}
		
		// if new format is different, reopen device
		if (newFmt.rate != aoFormat.rate || newFmt.channels != aoFormat.channels || aoDevice == NULL) {
			if (aoDevice) {
				ao_close(aoDevice);
				aoDevice = 0;
			}
			aoFormat = newFmt;
			if ((aoDevice = ao_open_live(aoDriverId, &aoFormat, aoOptions)) == NULL ) {
				qDebug() << "AmeSoundOutput: error while opening device. errno = " << errno;
				return;
			}
			if (!aoDevice)
				return;
		}
		
		if (ao_play(aoDevice, decoder->buffer(), len) == 0) {
			qDebug() << "AmeSystemSound: error while playing, the device should be closed";
		}
	}
	
	// quick fix to ALSA: underrun, at least 0ms. message
	ao_close(aoDevice);
	aoDevice = 0;
}


void AmeSystemSound::stopPlaying()
{	
}


void AmeSystemSound::onQuit()
{
}


/* ****** MIXER PRIVATE ******* */

int AmeSystemSound::setupMixer(QString card, QString device)
{
    char *name;
    long int a, b;
    long alsa_min_vol = 0, alsa_max_vol = 100;
    int err, index;
    pcm_element = 0;

    qDebug("AmeSystemSound::SoundOutput: setupMixer()");

    if ((err = getMixer(&mixer, card)) < 0)
        return err;

    parseMixerName(device.toAscii().data(), &name, &index);

    pcm_element = getMixerElem(mixer, name, index);

    free(name);

    if (!pcm_element)
    {
        qWarning("SoundOutput: Failed to find mixer element");
        return -1;
    }

    /* This hack was copied from xmms.
     * Work around a bug in alsa-lib up to 1.0.0rc2 where the
     * new range don't take effect until the volume is changed.
     * This hack should be removed once we depend on Alsa 1.0.0.
     */
    snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, &a);
    snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, &b);

    snd_mixer_selem_get_playback_volume_range(pcm_element,
            &alsa_min_vol, &alsa_max_vol);
    snd_mixer_selem_set_playback_volume_range(pcm_element, 0, 100);

    if (alsa_max_vol == 0)
    {
        pcm_element = NULL;
        return -1;
    }

    setVolume(a * 100 / alsa_max_vol);

    qDebug("SoundOutput: setupMixer() success");

    return 0;
}

void AmeSystemSound::parseMixerName(char *str, char **name, int *index)
{
	char *end;
	
	while (isspace(*str))
		str++;
	
	if ((end = strchr(str, ',')) != NULL) {
		*name = strndup(str, end - str);
		end++;
		*index = atoi(end);
	} else {
		*name = strdup(str);
		*index = 0;
	}
}

snd_mixer_elem_t* AmeSystemSound::getMixerElem(snd_mixer_t *mixer, char *name, int index)
{
	snd_mixer_selem_id_t* selem_id;
	snd_mixer_elem_t* elem;
	snd_mixer_selem_id_alloca(&selem_id);
	
	if (index != -1)
		snd_mixer_selem_id_set_index(selem_id, index);
	if (name != NULL)
		snd_mixer_selem_id_set_name(selem_id, name);
	
	elem = snd_mixer_find_selem(mixer, selem_id);
	return elem;
}

int AmeSystemSound::getMixer(snd_mixer_t **mixer, QString card)
{
    char *dev;
    int err;
	
	dev = strdup(card.toAscii().data());
	if ((err = snd_mixer_open(mixer, 0)) < 0) {
		qWarning("SoundOutput: Failed to open empty mixer: %s", snd_strerror(-err));
		mixer = NULL;
		return -1;
	}
	
	if ((err = snd_mixer_attach(*mixer, dev)) < 0) {
		qWarning("SoundOutput: Attaching to mixer %s failed: %s", dev, snd_strerror(-err));
		return -1;
	}
    
	if ((err = snd_mixer_selem_register(*mixer, NULL, NULL)) < 0) {
		qWarning("SoundOutput: Failed to register mixer: %s", snd_strerror(-err));
		return -1;
	}
	
	if ((err = snd_mixer_load(*mixer)) < 0) {
		qWarning("SoundOutput: Failed to load mixer: %s", snd_strerror(-err));
		return -1;
	}
	
	free(dev);
	return (*mixer != NULL);
}
