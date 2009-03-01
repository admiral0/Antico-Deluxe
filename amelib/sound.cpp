#include <Sound>
#include <QString>

#include "soundoutput.h"
#include "vorbisdecoder.h"

AmeSystemSound::AmeSystemSound(QObject *parent)
	: QObject(parent)
{
	blocked = false;
	paused = false;
	
	output = new SoundOutput(true, this);
	connect(output, SIGNAL(stopped()), this, SLOT(stopPlaying()));
	decoder = NULL;
	input = NULL;
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
	qDebug() << "STOP PLAYING, BEGIN STOPPING THREADS";
	if (decoder && decoder->isRunning()) {
		decoder->mutex()->lock();
		decoder->stop();
		decoder->mutex()->unlock();
	}
	
	if (output) {
		output->mutex()->lock();
		qDebug() << "STOP PLAYING, BEGIN STOPPING OUTPUT";
		output->stop();
		output->mutex()->unlock();
	}
	
	// wake up threads
	if (decoder) {
		decoder->mutex()->lock();
		decoder->cond()->wakeAll();
		decoder->mutex()->unlock();
	}
    
	if (output) {
		output->recycler()->mutex()->lock();
		output->recycler()->cond()->wakeAll();
		output->recycler()->mutex()->unlock();
	}
	if (decoder)
		decoder->wait();
	if (output) {
		output->wait();
		output->close();
	}
	//if (output && output->isInitialized()) {
	//	output->uninitialize();
	//}
	
	if (decoder) {
		//decoder->deinit();
		disconnect(decoder, SIGNAL(decoderState(int)), 0, 0);
		delete decoder;
		decoder = 0;
	}
	
	if (input) {
		input->close();
		delete input;
		input = 0;
	}	
}

bool AmeSystemSound::decode()
{
	qDebug() << "AmeSystemSound: decode begin";
	if (!decoder) {
		decoder = new VorbisDecoder(input, output, this);
		qDebug() << "create new decoder";
		connect(decoder, SIGNAL(decoderState(int)), this, SLOT(onDecoderState(int)));
	}
		
	if (decoder->initialize()) {
		output->start();
		decoder->start();
		blocked = false;
		qDebug() << "now playing";
		return true;
	}
	stopPlaying();
	blocked = false;
	return false;
}

void AmeSystemSound::onDecoderState(int state)
{
	qDebug() << "VORBIS DECODER STATE: " << state;
	if (state == VorbisDecoder::Finished) {
		qDebug() << "VORBIS DECODER FINISHED";
		stopPlaying();
	}
}

void AmeSystemSound::onQuit()
{
	//media->setCurrentSource(source);
	//quit();
}

int AmeSystemSound::volume()
{
	int l, r;
	output->volume(&l, &r);
	qDebug() << "VOLUME l=" << l << "r=" << r;
	return (l+r)/2;
}

void AmeSystemSound::setVolume(int v)
{
	output->setVolume(v, v);
}

bool AmeSystemSound::isMuted()
{
	//return audio->isMuted();
	return false;
}

void AmeSystemSound::onVolumeChanged(int volume)
{
	emit systemVolumeChanged(volume);
}
