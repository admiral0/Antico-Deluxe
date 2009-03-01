/* 
 *  This file is taken from QMMP project (http://qmmp.ylsoftware.com/index_en.php).
 *
 *  Copyright (C) 2006-2008 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru
 *
 *	Some modifications & stripping made by me. <ludmiloff@gmail.com>
 *
 */

#include <QtDebug>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "soundoutput.h"

Buffer::Buffer()
{
	data = new unsigned char[BLOCK_SIZE];
	nbytes = 0;
	rate = 0;
	exceeding = 0;
}

Buffer::~Buffer()
{
	delete data;
	data = 0;
	nbytes = 0;
	rate = 0;
	exceeding = 0;
}

Recycler::Recycler ( unsigned int sz )
	: add_index ( 0 ), done_index ( 0 ), current_count ( 0 )
{
	buffer_count = (sz / BLOCK_SIZE);
	if ( buffer_count < 1 ) {
        buffer_count = 1;
	}
	
	buffers = new Buffer*[buffer_count];
	for ( unsigned int i = 0; i < buffer_count; i ++ ) {
		buffers[i] = new Buffer;
	}
}


Recycler::~Recycler()
{
	for ( unsigned int i = 0; i < buffer_count; i++ ) {
		delete buffers[i];
		buffers[i] = 0;
	}
	delete [] buffers;
}


bool Recycler::full() const
{
	return current_count == buffer_count;
}


bool Recycler::empty() const
{
	return current_count == 0;
}


int Recycler::available() const
{
	return buffer_count - current_count;
}

int Recycler::used() const
{
	return current_count;
}


Buffer *Recycler::get(unsigned long size)
{
	if (full())
		return 0;
	if(size > BLOCK_SIZE + buffers[add_index]->exceeding) {
        delete buffers[add_index]->data;
		buffers[add_index]->data = new unsigned char[size];
		buffers[add_index]->exceeding = size - BLOCK_SIZE;
		//qDebug("new size = %d, index = %d", size, add_index);
	}
	return buffers[add_index];
}


void Recycler::add()
{
	add_index = ++add_index % buffer_count;
	current_count++;
}


Buffer *Recycler::next()
{
	return buffers[done_index];
}


void Recycler::done()
{
	done_index = ++done_index % buffer_count;
	current_count--;
}


void Recycler::clear()
{
	add_index = done_index = current_count = 0;
	/*for ( unsigned int i = 0; i < buffer_count; i ++ ) {
		if(buffers[i]->exceeding > 0) {
			delete buffers[i]->data;
			buffers[i]->data = new unsigned char[BLOCK_SIZE];
			buffers[i]->exceeding = 0;
			buffers[i]->nbytes = 0;
		}
	}*/
}


unsigned int Recycler::size() const
{
	return buffer_count * BLOCK_SIZE;
}

SoundOutput::SoundOutput(bool withMixer, QObject *parent)
	: QThread(parent),
	  r(stackSize()),
	  useMixer(withMixer)
{
	pcm_name = 0;
	pcm_handle = 0;
	mixer = 0;
	
	m_inited = false;
	m_pause = false;
	m_play = false;
	m_userStop = false;
	m_totalWritten = 0;
	
	m_bps = -1;
	m_frequency = -1;
	m_channels = -1;
	m_precision = -1;
	
	/* load settings from config */
	readSettings();
}

SoundOutput::~SoundOutput()
{
	uninitialize();
}

bool SoundOutput::isInitialized() const
{
	return m_inited;
}

void SoundOutput::readSettings()
{
	uninitialize();
	
	QSettings settings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
	settings.beginGroup("ALSA");
	
	QString device = settings.value("device", "default").toString();
	pcm_name = strdup(device.toAscii().data());
	pcm_handle = 0;
	
	m_use_mmap = settings.value("use_mmap", false).toBool();
	
	buffer_time = settings.value("buffer_time",500).toUInt()*1000;
	period_time = settings.value("period_time",100).toUInt()*1000;
	
	if (useMixer) {
		QString mixerCard = settings.value("mixer_card", "hw:0").toString();
		QString mixerDevice = settings.value("mixer_device", "PCM").toString();
		setupMixer(mixerCard, mixerDevice);
	}
	
	settings.endGroup();
}

void SoundOutput::uninitialize()
{
	if (pcm_name) {
		free(pcm_name);
		pcm_name = 0;
	}
	
	if (mixer) {
		snd_mixer_close(mixer);	
		mixer = 0;
	}
	
	m_inited = false;
	m_pause = false;
	m_play = false;
	m_userStop = false;
	
}

void SoundOutput::close()
{
	if (pcm_handle) {
		snd_pcm_close(pcm_handle);
		pcm_handle = 0;
	}
}

bool SoundOutput::initialize()
{
	m_inited = m_pause = m_play = m_userStop = FALSE;
	
	if (pcm_handle)
		return FALSE;
	
	m_totalWritten = 0;
	if (snd_pcm_open(&pcm_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0) {
		qWarning ("AmeSystemSound::Output: Error opening PCM device %s", pcm_name);
		return FALSE;
	}
	qDebug() << "PCM DEVICE INITIALIZED";
	m_inited = TRUE;
	return m_inited;
}

void SoundOutput::reset()
{
	close();
	initialize();
}

void SoundOutput::configure(long freq, int chan, int prec, int brate)
{
    // we need to configure
    if (freq != m_frequency || chan != m_channels || prec != m_precision)
    {
        m_frequency = freq;
        m_channels = chan;
        m_precision = prec;
        m_bps = freq * chan * (prec / 8);
        uint rate = m_frequency; /* Sample rate */
        uint exact_rate = m_frequency;   /* Sample rate returned by */

        snd_pcm_hw_params_t *hwparams = 0;
        snd_pcm_sw_params_t *swparams = 0;
        int err; //alsa error code

        //hw params
        snd_pcm_hw_params_alloca(&hwparams);
        if ((err = snd_pcm_hw_params_any(pcm_handle, hwparams)) < 0)
        {
            qWarning("SoundOutput: Can not read configuration for PCM device: %s", snd_strerror(err));
            return;
        }
        if (m_use_mmap)
        {
            if ((err = snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_MMAP_INTERLEAVED)) < 0)
            {
                qWarning("SoundOutput: Error setting mmap access: %s", snd_strerror(err));
                m_use_mmap = FALSE;
            }
        }
        if (!m_use_mmap)
        {
            if ((err = snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
            {
                qWarning("SoundOutput: Error setting access: %s", snd_strerror(err));
                return;
            }
        }
        snd_pcm_format_t format = SND_PCM_FORMAT_UNKNOWN;
        switch (prec)
        {
        case 8:
            format = SND_PCM_FORMAT_S8;
            break;
        case 16:
            format = SND_PCM_FORMAT_S16_LE;
            break;
        case 24:
            format = SND_PCM_FORMAT_S24_LE;
            break;
        case 32:
            format = SND_PCM_FORMAT_S32_LE;
            break;
        default:
            qWarning("SoundOutput: unsupported format detected");
            return;
        }
        if ((err = snd_pcm_hw_params_set_format(pcm_handle, hwparams, format)) < 0)
        {
            qDebug("SoundOutput: Error setting format: %s", snd_strerror(err));
            return;
        }
        exact_rate = rate;// = 11000;
        qDebug("SoundOutput: frequency=%d, channels=%d, bitrate=%d",
               rate, chan, brate);

        if ((err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0)) < 0)
        {
            qWarning("SoundOutput: Error setting rate: %s", snd_strerror(err));
            return;
        }
        if (rate != exact_rate)
        {
            qWarning("SoundOutput: The rate %d Hz is not supported by your hardware.\n==> Using %d Hz instead.", rate, exact_rate);
        }
        uint c = m_channels;
        if ((err = snd_pcm_hw_params_set_channels_near(pcm_handle, hwparams, &c)) < 0)
        {
            qWarning("SoundOutput: Error setting channels: %s", snd_strerror(err));
            return;
        }
        if ((err = snd_pcm_hw_params_set_period_time_near(pcm_handle, hwparams, &period_time ,0)) < 0)
        {
            qWarning("SoundOutput: Error setting period time: %s", snd_strerror(err));
            return;
        }
        if ((err = snd_pcm_hw_params_set_buffer_time_near(pcm_handle, hwparams, &buffer_time ,0)) < 0)
        {
            qWarning("SoundOutput: Error setting buffer time: %s", snd_strerror(err));
            return;
        }
        if ((err = snd_pcm_hw_params(pcm_handle, hwparams)) < 0)
        {
            qWarning("SoundOutput: Error setting HW params: %s", snd_strerror(err));
            return;
        }
        //read some alsa parameters
        snd_pcm_uframes_t buffer_size = 0;
        snd_pcm_uframes_t period_size = 0;
        if ((err = snd_pcm_hw_params_get_buffer_size(hwparams, &buffer_size)) < 0)
        {
            qWarning("SoundOutput: Error reading buffer size: %s", snd_strerror(err));
            return;
        }
        if ((err = snd_pcm_hw_params_get_period_size(hwparams, &period_size, 0)) < 0)
        {
            qWarning("SoundOutput: Error reading period size: %s", snd_strerror(err));
            return;
        }
        //swparams
        snd_pcm_sw_params_alloca(&swparams);
        snd_pcm_sw_params_current(pcm_handle, swparams);
        if ((err = snd_pcm_sw_params_set_start_threshold(pcm_handle, swparams,
                   buffer_size - period_size)) < 0)
            qWarning("SoundOutput: Error setting threshold: %s", snd_strerror(err));
        if ((err = snd_pcm_sw_params(pcm_handle, swparams)) < 0)
        {
            qWarning("SoundOutput: Error setting SW params: %s", snd_strerror(err));
            return;
        }
        //setup needed values
        m_bits_per_frame = snd_pcm_format_physical_width(format) * chan;
        m_chunk_size = period_size;
		
		qDebug() << "AmeSystemSound::SoundOutput: configure done";
    }
}

void SoundOutput::stop()
{
	m_userStop = true;
}

void SoundOutput::run()
{
	mutex()->lock ();
	if (! m_inited) {
		mutex()->unlock();
		return;
	}
	
	m_play = TRUE;
	
	mutex()->unlock();
	
	Buffer *b = 0;
	bool done = FALSE;
	long m = 0;
	snd_pcm_uframes_t l;
	
	long prebuffer_size = BLOCK_SIZE + m_bits_per_frame * m_chunk_size / 8;
	
	unsigned char *prebuffer = (unsigned uchar *)malloc(prebuffer_size);
	ulong prebuffer_fill = 0;

    //dispatch(OutputState::Playing);
    qDebug() << "BEGIN PLAYING";
	
	while (!done) {
		mutex()->lock();
		recycler()->mutex()->lock();
		
		done = m_userStop;
		
		while (!done && (recycler()->empty() || m_pause)) {
			qDebug() << "RECYCLER EMPTY OR DONE";
			mutex()->unlock();
			recycler()->cond()->wakeOne();
			qDebug() << "WAKE ONE";
			recycler()->cond()->wait(recycler()->mutex());
			qDebug() << "WAIT MUTEX";
			mutex()->lock ();
			qDebug() << "END RECYCLER EMPTY OR DONE";
			done = m_userStop;
			qDebug() << "DONE OUTPUT = " << done;
		}
		status();
		
		if (! b) {
			b = recycler()->next();
			if (b->rate)
				m_rate = b->rate;
			}
			
		recycler()->cond()->wakeOne();
		recycler()->mutex()->unlock();
			
		if (b) {
			if ((ulong)prebuffer_size < prebuffer_fill + b->nbytes) {
				prebuffer_size = prebuffer_fill + b->nbytes;
				prebuffer = (unsigned char*) realloc(prebuffer, prebuffer_size);
				qDebug() << "REALLOC DONE";
			}
			
			qDebug() << "COPY NEXT CHUNK";
			memcpy(prebuffer + prebuffer_fill, b->data, b->nbytes);
			prebuffer_fill += b->nbytes;
			
			l = snd_pcm_bytes_to_frames(pcm_handle, prebuffer_fill);
			
			while (l >= m_chunk_size) {
				snd_pcm_wait(pcm_handle, 10);
				if ((m = alsaWrite(prebuffer, m_chunk_size)) >= 0) {
					l -= m;
					m = snd_pcm_frames_to_bytes(pcm_handle, m); // convert frames to bytes
					prebuffer_fill -= m;
					memcpy(prebuffer, prebuffer + m, prebuffer_fill); //move data to begin
					m_totalWritten += m;
					status();
                    //dispatchVisual(b, m_totalWritten, m_channels, m_precision);
				} else
					break;
			}
		}
		
		qDebug() << "FORCE BUFFER CHANGE";
		//force buffer change
		recycler()->mutex()->lock();
		recycler()->done();
		recycler()->mutex()->unlock();
		b = 0;
		mutex()->unlock();
		qDebug() << "BUFFER CHANGE DONE";
	}
	
	qDebug() << "WRITE REMAINING DATA";
	mutex()->lock();
	//write remaining data
	if (prebuffer_fill > 0 && recycler()->empty()) {
		l = snd_pcm_bytes_to_frames(pcm_handle, prebuffer_fill);
		while (l > 0) {
			if ((m = alsaWrite(prebuffer, l)) >= 0) {
				l -= m;
				m = snd_pcm_frames_to_bytes(pcm_handle, m); // convert frames to bytes
				prebuffer_fill -= m;
				memcpy(prebuffer, prebuffer + m, prebuffer_fill);
				m_totalWritten += m;
				status();
			} else
				break;
		}
	}
	m_play = FALSE;
    //dispatch(OutputState::Stopped);
	emit stopped();
    qDebug() << "STOP PLAYING";
	free(prebuffer);
	prebuffer = 0;
	mutex()->unlock();
}

long SoundOutput::alsaWrite(unsigned char *data, long size)
{
	long m = 0;
	if (m_use_mmap)
		m = snd_pcm_mmap_writei (pcm_handle, data, size);
	else
		m = snd_pcm_writei (pcm_handle, data, size);
	
	if (m == -EAGAIN) {
		mutex()->unlock();
		snd_pcm_wait(pcm_handle, 500);
		mutex()->lock ();
		return 0;
	} else if (m >= 0) {
		if (m < size) {
			mutex()->unlock();
			snd_pcm_wait(pcm_handle, 500);
			mutex()->lock ();
		}
		return m;
	} else if (m == -EPIPE) {
		qDebug ("AmeSystemSound::SoundOutput: buffer underrun!");
		if ((m = snd_pcm_prepare(pcm_handle)) < 0) {
			qDebug ("AmeSystemSound::SoundOutput: Can't recover after underrun: %s", snd_strerror(m));
			/* TODO: reopen the device */
			return -1;
		}
	} else if (m == -ESTRPIPE) {
		qDebug ("AmeSystemSound::SoundOutput: Suspend, trying to resume");
		while ((m = snd_pcm_resume(pcm_handle)) == -EAGAIN) sleep (1);
		if (m < 0) {
			qDebug ("AmeSystemSound::SoundOutput: Failed, restarting");
			if ((m = snd_pcm_prepare(pcm_handle)) < 0) {
				qDebug ("AmeSystemSound::SoundOutput: Failed to restart device: %s.", snd_strerror(m));
				return -1;
			}
		}
	}
	return -1;
}

/* ****** MIXER ******* */

int SoundOutput::setupMixer(QString card, QString device)
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

    setVolume(a * 100 / alsa_max_vol, b * 100 / alsa_max_vol);

    qDebug("SoundOutput: setupMixer() success");

    return 0;
}

void SoundOutput::parseMixerName(char *str, char **name, int *index)
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

snd_mixer_elem_t* SoundOutput::getMixerElem(snd_mixer_t *mixer, char *name, int index)
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

void SoundOutput::setVolume(int l, int r)
{
	if (!pcm_element)
		return;
	snd_mixer_selem_set_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, l);
	snd_mixer_selem_set_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, r);
}

void SoundOutput::volume(int *l, int *r)
{
	if (!pcm_element)
		return;
	
	long ll = *l, lr = *r;
	snd_mixer_handle_events(mixer);
	snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, &ll);
	snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, &lr);
	*l = ll;
	*r = lr;
}

int SoundOutput::getMixer(snd_mixer_t **mixer, QString card)
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

// Helper functions

Recycler *SoundOutput::recycler()
{
	return &r;
}

QMutex *SoundOutput::mutex()
{
	return &mtx;
}

void SoundOutput::status()
{
}

