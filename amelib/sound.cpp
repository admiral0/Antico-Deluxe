#include <Sound>
#include <QString>
#include <phonon/mediaobject.h>

#include "soundoutput_p.h"
#include "vorbisdecoder_p.h"

AmeSystemSound::AmeSystemSound(QObject *parent)
	: QObject(parent)
{
	blocked = false;
	paused = false;
	
	output = new SoundOutput(true, this);
	decoder = NULL;
	
	media = new Phonon::MediaObject(this);
	audio = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	//audio->setOutputDevice("hw:0,0");
	Phonon::createPath(media, audio);
	connect(media, SIGNAL(finished()), this, SLOT(onQuit()));
	connect(audio, SIGNAL(volumeChanged(qreal)), this, SLOT(onVolumeChanged(qreal)));
	source = "";
}

AmeSystemSound::~AmeSystemSound()
{
}

void AmeSystemSound::setSound(int soundType) 
{
	switch (soundType) {
		case Click: {
			source = ":/common/sounds/Click1.ogg";
			break;
		}
		case Error1: {
			source = ":/common/sounds/Error1.ogg";
			break;
		}
		case Error2: {
			source = ":/common/sounds/Error2.ogg";
			break;
		}
		case Clap: {
			source = ":/common/sounds/Kopete_status.ogg";
			break;
		}
		case Maximize: {
			source = ":/common/sounds/Maximize.ogg";
			break;
		}
		case Minimize: {
			source = ":/common/sounds/Minimize4.ogg";
			break;
		}
		case Popup: {
			source = ":/common/sounds/Popup.ogg";
			break;
		}
		case Question: {
			source = ":/common/sounds/Question.ogg";
			break;
		}
		case Background: {
			source = ":/common/sounds/Question_background.ogg";
			break;
		}
		case RestoreDown: {
			source = ":/common/sounds/Restore_down.ogg";
			break;
		}
		case RestoreUp: {
			source = ":/common/sounds/Restore_up.ogg";
			break;
		}
		case ShadeDown: {
			source = ":/common/sounds/Shade_down.ogg";
			break;
		}
		case ShadeUp: {
			source = ":/common/sounds/Shade_up.ogg";
			break;
		}
	}
	media->stop();
	media->setCurrentSource(source);
}

bool AmeSystemSound::play()
{
	stopPlaying();
	if (source.isEmpty())
		return false;
	
	input = new QFile(source);
	
	// open device
	if (!output->initialize())
		return false;
	
	return decode();
}

void AmeSystemSound::stopPlaying()
{
	if (blocked)
		return;
	paused = false;
	
}

bool AmeSystemSound::decode()
{
	qDebug() << "AmeSystemSound: decode begin";
	if (!decoder) {
		decoder = new VorbisDecoder(input, output, this);
		qDebug() << "create new decoder";
	}
	
	blocked = false;
	
	if (decoder->initialize()) {
		output->start();
		decoder->start();
		qDebug() << "now playing";
		return true;
	}
	stopPlaying();
	return false;
}

void AmeSystemSound::onQuit()
{
	//media->setCurrentSource(source);
	//quit();
}

qreal AmeSystemSound::volume()
{
	return audio->volume();
}

void AmeSystemSound::setVolume(qreal v)
{
	audio->setVolume(v);
}

bool AmeSystemSound::isMuted()
{
	return audio->isMuted();
}

void AmeSystemSound::onVolumeChanged(qreal volume)
{
	emit systemVolumeChanged(volume);
}
