/* 
 *  Parts of this file are taken from vorbis-tools ogg123 
 *  and QMMP project http://qmmp.ylsoftware.com/index_en.php
 *
 *	Some modifications & stripping made by me. <ludmiloff@gmail.com>
 *
 */
 
#ifndef __SOUNDOUTPUT_P_H
#define __SOUNDOUTPUT_P_H

#include <Sound>
#include <QThread>
#include <QSettings>
#include <AmeDirs>

#include <alsa/asoundlib.h>

class SoundOutput : public QThread
{
	Q_OBJECT
public:
	SoundOutput(VorbisDecoder *dec, bool withMixer = true, QObject *parent = 0);
	virtual ~SoundOutput();

	bool initialize();
	void uninitialize();

	virtual void configure(long, int, int, int);

	void stop();

	//alsa mixer
	void setVolume(int l, int r);
	void volume(int *l, int *r);

signals:
	void stopped();

	// Thread run
	virtual void run();

private:
	// Output
	bool mPlay;
	bool mPause;
	bool mUserStop;

	VorbisDecoder *decoder;

	long m_bps;
	int m_rate, m_frequency, m_channels, m_precision;
	uint buffer_time;
	uint period_time;
		
	//alsa mixer
	bool useMixer;
	int setupMixer(QString card, QString device);
	void parseMixerName(char *str, char **name, int *index);
	
	int getMixer(snd_mixer_t **mixer, QString card);
	snd_mixer_elem_t* getMixerElem(snd_mixer_t *mixer, char *name, int index);
	snd_mixer_t *mixer;
	snd_mixer_elem_t *pcm_element;
	bool m_use_mmap;
};


#endif
