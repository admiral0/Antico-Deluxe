/* 
 *  This file is taken from QMMP project (http://qmmp.ylsoftware.com/index_en.php).
 *
 *  Copyright (C) 2006-2008 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru
 *
 *	Some modifications & stripping made by me. <ludmiloff@gmail.com>
 *
 */
 
#ifndef __VORBISDECODER_P_H
#define __VORBISDECODER_P_H

#include <Sound>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtDebug>
#include <vorbis/vorbisfile.h>

class SoundOutput;
	
class VorbisDecoder : public QThread
{
	Q_OBJECT
public:

	enum DecoderState { NoneState, Decoding, Stopped, Finished, Info, Error };

    VorbisDecoder(QIODevice *i, SoundOutput *o, QObject *parent = 0);
    virtual ~VorbisDecoder();

    // Standard Decoder API
    bool initialize();
	void deinit();
    double lengthInSeconds();
    void seek(double);
    void stop();
	
	ulong produceSound(char *data, ulong output_bytes, ulong bitrate, int nch);
	
	QIODevice *input();
	SoundOutput *output();
	QMutex *mutex();
	QWaitCondition *cond();
	
signals:
	void decoderState(int newState);
	
protected:
	void configure(long freq, int channels, int prec, int bitrate);

private:
    // thread run function
    void run();

    // helper functions
    void flush(bool = FALSE);

    bool inited, user_stop;
    int state;

    // output buffer
    char *output_buf;
    ulong output_bytes, output_at;
	unsigned int blksize;

	//
    QIODevice *m_input;
    SoundOutput *m_output;
    QMutex mtx;
    QWaitCondition cnd;

    // OggVorbis Decoder
    OggVorbis_File oggfile;

    unsigned int bks;
    bool done, finish;
    long len, freq, bitrate;
    int chan;
    unsigned long output_size;
    double totalTime, seekTime;
		
	// ogg vorbis stream
	//static size_t oggread (void *buf, size_t size, size_t nmemb, void *src);
	//static int oggseek(void *src, int64_t offset, int whence);
	//static int oggclose(void *src);
	//static long oggtell(void *src);
};

#endif