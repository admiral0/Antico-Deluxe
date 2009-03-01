/* 
 *  This file is taken from QMMP project (http://qmmp.ylsoftware.com/index_en.php).
 *
 *  Copyright (C) 2006-2008 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru
 *
 *	Some modifications & stripping made by me. <ludmiloff@gmail.com>
 *
 */
 
#ifndef __SOUNDOUTPUT_P_H
#define __SOUNDOUTPUT_P_H

#include <Sound>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSettings>
#include <AmeDirs>

#include <alsa/asoundlib.h>

#define BLOCK_SIZE 2048		// 2*1024
#define BUFFER_SIZE 65536	// BLOCK_SIZE * 32

class Buffer
// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//
// 2009-02-15: small modifications <ludmiloff@gmal.com>
{
public:
	Buffer();
	~Buffer();
	
    unsigned char *data;
    unsigned long nbytes;
    unsigned long rate;
    unsigned long exceeding;
};

class Recycler
// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//
// 2009-02-15: small modifications <ludmiloff@gmal.com>
{
public:
	Recycler(unsigned int sz); // sz = size in bytes
	~Recycler();

	bool full() const;
	bool empty() const;

	int available() const;
	int used() const;

	Buffer *next(); // get next in queue
	Buffer *get(unsigned long size); // get next in recycle

	void add(); // add to queue
	void done(); // add to recycle

	void clear(); // clear queue

	unsigned int size() const; // size in bytes

	QMutex *mutex() { return &mtx; }
	QWaitCondition *cond() { return &cnd; }
	
private:
	unsigned int buffer_count, add_index, done_index, current_count;
	Buffer **buffers;
	QMutex mtx;
	QWaitCondition cnd;
};

class SoundOutput : public QThread
{
	Q_OBJECT
public:
	SoundOutput(bool withMixer, QObject *parent = 0);
	virtual ~SoundOutput();

	bool initialize();
	void uninitialize();
	bool isInitialized() const;
	virtual void configure(long, int, int, int);

	void stop();
	void close();
	void setVolume(int l, int r);
	void volume(int *l, int *r);

	void readSettings();

	Recycler *recycler();
	QMutex *mutex();

signals:
	void stopped();

private:
	// Output
	bool m_inited;
	bool m_play;
	bool m_pause;
	bool m_userStop;

	long m_bps;
	int m_rate, m_frequency, m_channels, m_precision;
	uint buffer_time;
	uint period_time;

	//alsa
	snd_pcm_t *pcm_handle;
	char *pcm_name;

	snd_pcm_uframes_t m_chunk_size;
	size_t m_bits_per_frame;
	long m_totalWritten;
    
	void reset();

    // helper functions
    void status();
	long alsaWrite(unsigned char *data, long size);

	//
    QMutex mtx;
    Recycler r;
	
	// Thread run
	void run();
	
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