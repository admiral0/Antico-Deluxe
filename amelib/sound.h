//////////////////////////////////////////////////////////
//  File      : sound.h					//
//  Written by: ludmiloff@gmail.com			//
//  Copyright : GPL2					//
//	Some parts of the implementation		//
//  are taken/inspired from Ogg123 code			//
//  THE Ogg123 SOURCE CODE IS (C) COPYRIGHT 2000-2001	//
//  by Stan Seibert <volsung@xiph.org> AND OTHER	//
//  CONTRIBUTORS, http://www.xiph.org/			//
//  Some modifications & stripping made by me:		//
//  <ludmiloff@gmail.com>				//
//////////////////////////////////////////////////////////

 
#ifndef __AMESOUND_H
#define __AMESOUND_H

#include <Global>
#include <QFile>
#include <QThread>
#include <QSettings>
#include <AmeDirs>

#include <alsa/asoundlib.h>
#include <ao/ao.h>

#define AO_WORD_SIZE 2

class VorbisDecoder;

class AME_EXPORT AmeSystemSound : public QThread
{
	Q_OBJECT
public:
	AmeSystemSound(bool withMixer = true, QObject *parent = 0);
	~AmeSystemSound();

	// Player API
	bool play(const QString &src);
	bool play();
	void stop();
	void pause();
	void resume();
	void setEmbedSound(int soundId, bool force=false);

	// Mixer API
	int volume();
	void setVolume(int);
	bool isMuted();
	
	//
	void initialize();
	void uninitialize();
		
public slots:
	void stopPlaying();
	void onQuit();
	void onVolumeChanged(int);

signals:
	void stopped();
	void systemVolumeChanged(int);
	
protected:
	// Thread run
	virtual void run();

private:
	QString source;
	QFile *input;
	bool mInited;

	// Output
	QString deviceName;
	int aoDriverId;
	ao_device *aoDevice;
	ao_option *aoOptions;
	ao_sample_format aoFormat;
	bool mUseMmap;

	// Output cmd
	bool mPlay;
	bool mPause;
	bool mUserStop;

	VorbisDecoder *decoder;

	uint buffer_time;
	uint period_time;

	// Alsa mixer
	bool useMixer;
	int setupMixer(QString card, QString device);
	void parseMixerName(char *str, char **name, int *index);
	
	int getMixer(snd_mixer_t **mixer, QString card);
	snd_mixer_elem_t* getMixerElem(snd_mixer_t *mixer, char *name, int index);
	snd_mixer_t *mixer;
	snd_mixer_elem_t *pcm_element;
};

#endif
