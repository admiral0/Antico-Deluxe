#ifndef __VORBISDECODER_P_H
#define __VORBISDECODER_P_H

#include <Sound>
#include <QThread>
#include <vorbis/vorbisfile.h>

class AmeSystemSound::VorbisDecoder : public QThread
{
public:

	enum DecoderState { NoneState, Decoding, Stopped, Finished, Info, Error };

    VorbisDecoder(QIODevice *i, AmeSystemSound::SoundOutput *o, QObject *parent = 0);
    virtual ~VorbisDecoder();

    // Standard Decoder API
    bool initialize();
    double lengthInSeconds();
    void seek(double);
    void stop();
	
	ulong produceSound(char *data, ulong output_bytes, ulong bitrate, int nch);
	
	QIODevice *input();
	AmeSystemSound::SoundOutput *output();
	QMutex *mutex();
	QWaitCondition *cond();
	
protected:
	void configure(long freq, int channels, int prec, int bitrate);

private:
    // thread run function
    virtual void run();

    // helper functions
    void flush(bool = FALSE);
    void deinit();

    bool inited, user_stop;
    int state;

    // output buffer
    char *output_buf;
    ulong output_bytes, output_at;
	unsigned int blksize;

	//
    QIODevice *m_input;
    AmeSystemSound::SoundOutput *m_output;
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
	static size_t oggread (void *buf, size_t size, size_t nmemb, void *src);
	static int oggseek(void *src, int64_t offset, int whence);
	static int oggclose(void *src);
	static long oggtell(void *src);
};

size_t AmeSystemSound::VorbisDecoder::oggread (void *buf, size_t size, size_t nmemb, void *src)
{
	if (src == 0) return 0;
	
	AmeSystemSound::VorbisDecoder *decoder = (AmeSystemSound::VorbisDecoder *) src;
	int len = decoder->input()->read((char *) buf, (size * nmemb));
	return len / size;
}


int AmeSystemSound::VorbisDecoder::oggseek(void *src, int64_t offset, int whence)
{
	AmeSystemSound::VorbisDecoder *decoder = (AmeSystemSound::VorbisDecoder *) src;
	if (decoder->input()->isSequential ())
		return -1;
	
	long start = 0;
	switch (whence) {
		case SEEK_END:
			start = decoder->input()->size();
			break;
		
		case SEEK_CUR:
			start = decoder->input()->pos();
			break;
		
		case SEEK_SET:
		default:
			start = 0;
	}
	
	if (decoder->input()->seek(start + offset))
		return 0;
	return -1;
}


int AmeSystemSound::VorbisDecoder::oggclose(void *src)
{
    AmeSystemSound::VorbisDecoder *decoder = (AmeSystemSound::VorbisDecoder *) src;
    decoder->input()->close();
    return 0;
}


long AmeSystemSound::VorbisDecoder::oggtell(void *src)
{
    AmeSystemSound::VorbisDecoder *decoder = (AmeSystemSound::VorbisDecoder *) src;
    long t = decoder->input()->pos();
    return t;
}


// Decoder class

AmeSystemSound::VorbisDecoder::VorbisDecoder(QIODevice *i, AmeSystemSound::SoundOutput *o, 
		QObject *parent)
	: QThread(parent), m_input(i), m_output(o)
{
	m_output->recycler()->clear();
	blksize = BUFFER_SIZE;
    inited = FALSE;
    user_stop = FALSE;
    state = NoneState;
    output_buf = 0;
    output_bytes = 0;
    output_at = 0;
    bks = 0;
    done = FALSE;
    finish = FALSE;
    len = 0;
    freq = 0;
    bitrate = 0;
    seekTime = -1.0;
    totalTime = 0.0;
    chan = 0;
    output_size = 0;
	qDebug("VorbisDecoder: creation done");
}


AmeSystemSound::VorbisDecoder::~VorbisDecoder()
{
    deinit();

    if (output_buf)
        delete [] output_buf;
    output_buf = 0;
}


void AmeSystemSound::VorbisDecoder::stop()
{
    user_stop = TRUE;
}


void AmeSystemSound::VorbisDecoder::flush(bool final)
{
    ulong min = final ? 0 : bks;

    while ((! done && ! finish) && output_bytes > min)
    {
        output()->recycler()->mutex()->lock ();

        while ((! done && ! finish) && output()->recycler()->full())
        {
            mutex()->unlock();

            output()->recycler()->cond()->wait(output()->recycler()->mutex());

            mutex()->lock ();
            done = user_stop;
        }

        if (user_stop || finish)
        {
            inited = FALSE;
            done = TRUE;
        }
        else
        {
            /*ulong sz = output_bytes < bks ? output_bytes : bks;
            Buffer *b = output()->recycler()->get();

            memcpy(b->data, output_buf, sz);
            if (sz != bks) memset(b->data + sz, 0, bks - sz);

            b->nbytes = bks;
            b->rate = bitrate;
            output_size += b->nbytes;
            output()->recycler()->add();

            output_bytes -= sz;
            memmove(output_buf, output_buf + sz, output_bytes);*/
            output_bytes -= produceSound(output_buf, output_bytes, bitrate, chan);
            output_size += bks;
            output_at = output_bytes;
        }

        if (output()->recycler()->full())
        {
            output()->recycler()->cond()->wakeOne();
        }

        output()->recycler()->mutex()->unlock();
    }
}


bool AmeSystemSound::VorbisDecoder::initialize()
{
    qDebug("VorbisDecoder: begin initialize");

    inited = user_stop = done = finish = false;
	qDebug("VorbisDecoder: initialize - step 0");
    len = freq = bitrate = 0;
    state = NoneState;
	chan = 0;
    output_size = 0;
    seekTime = -1.0;
    totalTime = 0.0;
	qDebug("VorbisDecoder: initialize - step 0.5");
	bks = blksize;
	
	qDebug("VorbisDecoder: initialize - step 1");
	
    if (! input())
    {
        qDebug("VorbisDecoder: cannot initialize.  No input");

        return FALSE;
    }

    if (! output_buf)
        output_buf = new char[BUFFER_SIZE];
	
    output_at = 0;
    output_bytes = 0;

    if (! input()->isOpen())
    {
        if (! input()->open(QIODevice::ReadOnly))
        {
            qWarning(qPrintable("VorbisDecoder: failed to open input. " +
                                input()->errorString () + "."));
            return FALSE;
        }
    }
	qDebug("VorbisDecoder: open input done");

    ov_callbacks oggcb =
        {
            oggread,
            oggseek,
            oggclose,
            oggtell
        };
    if (ov_open_callbacks(this, &oggfile, NULL, 0, oggcb) < 0)
    {
        qWarning("VorbisDecoder: cannot open stream");

        return FALSE;
    }

    freq = 0;
    bitrate = ov_bitrate(&oggfile, -1) / 1000;
    chan = 0;

    totalTime = long(ov_time_total(&oggfile, 0));
    totalTime = totalTime < 0 ? 0 : totalTime;

    vorbis_info *ogginfo = ov_info(&oggfile, -1);
    if (ogginfo)
    {
        freq = ogginfo->rate;
        chan = ogginfo->channels;
    }

    configure(freq, chan, 16, bitrate);

    inited = TRUE;
    return TRUE;
}

void AmeSystemSound::VorbisDecoder::configure(long freq, int channels, int prec, int bitrate)
{
	if (m_output)
		m_output->configure(freq, channels, prec, bitrate);
}

double AmeSystemSound::VorbisDecoder::lengthInSeconds()
{
    if (! inited)
        return 0;

    return totalTime;
}


void AmeSystemSound::VorbisDecoder::seek(double pos)
{
    seekTime = pos;
}


void AmeSystemSound::VorbisDecoder::deinit()
{
    if (inited)
        ov_clear(&oggfile);
    inited = user_stop = done = finish = FALSE;
    len = freq = bitrate = 0;
    state = NoneState;
	chan = 0;
    output_size = 0;
}

void AmeSystemSound::VorbisDecoder::run()
{
	mutex()->lock ();
	if (!inited) {
		mutex()->unlock();
		return;
	}
	
	state = Decoding;
	mutex()->unlock();

	// REMOVED
	//dispatch(state);

    int section = 0;
    int last_section = -1;

    while (! done && ! finish)
    {
        mutex()->lock ();
		
        if (seekTime >= 0.0)
        {
            ov_time_seek(&oggfile, double(seekTime));
            seekTime = -1.0;

            output_size = long(ov_time_tell(&oggfile)) * long(freq * chan * 2);
        }
        len = -1;
        while (len < 0)
        {
            len = ov_read(&oggfile, (char *) (output_buf + output_at), bks, 0, 2, 1,
                          &section);
        }
		// REMOVED
		//if (section != last_section)
		//	updateTags();
        last_section = section;

        if (len > 0)
        {
            bitrate = ov_bitrate_instant(&oggfile) / 1000;

            output_at += len;
            output_bytes += len;

            if (output())
                flush();
        }
        else if (len == 0)
        {
            flush(TRUE);

            if (output())
            {
                output()->recycler()->mutex()->lock ();
                // end of stream
                while (! output()->recycler()->empty() && ! user_stop)
                {
                    output()->recycler()->cond()->wakeOne();
                    mutex()->unlock();
                    output()->recycler()->cond()->wait(output()->recycler()->mutex());
                    mutex()->lock ();
                }
                output()->recycler()->mutex()->unlock();
            }

            done = TRUE;
            if (! user_stop)
            {
                finish = TRUE;
            }
        }
        else
        {
            // error in read
            qDebug("DecoderVorbis: Error while decoding stream, File appears to be "
                  "corrupted");

            finish = TRUE;
        }

        mutex()->unlock();
    }

    mutex()->lock ();

    if (finish)
        state = Finished;
    else if (user_stop)
        state = Stopped;

    mutex()->unlock();
	
	// REMOVED
	//dispatch(state);

    deinit();
}

ulong AmeSystemSound::VorbisDecoder::produceSound(char *data, ulong output_bytes, ulong bitrate, int nch)
{
	ulong sz = output_bytes < blksize ? output_bytes : blksize;
		
	// REMOVED
	//if (m_useVolume) {
	//	changeVolume(data, sz, nch);
	//}
    char *out_data = data;
    char *prev_data = data;
    ulong w = sz;

    Buffer *b = output()->recycler()->get(w);

    memcpy(b->data, out_data, w);

    if (data != out_data)
        delete out_data;

    if (w < blksize + b->exceeding)
        memset(b->data + w, 0, blksize + b->exceeding - w);

    b->nbytes = w;// blksize;
    b->rate = bitrate;

    output()->recycler()->add();

    output_bytes -= sz;
    memmove(data, data + sz, output_bytes);
    return sz;
}

QIODevice *AmeSystemSound::VorbisDecoder::input()
{
	return m_input;
}

AmeSystemSound::SoundOutput *AmeSystemSound::VorbisDecoder::output()
{
	return m_output;
}

QMutex *AmeSystemSound::VorbisDecoder::mutex()
{
	return &mtx;
}

QWaitCondition *AmeSystemSound::VorbisDecoder::cond()
{
	return &cnd;
}
#endif
