//////////////////////////////////////////////////////////
//  File      : vorbisdecoder.h							//
//  Written by: ludmiloff@gmail.com						//
//  Copyright : GPL2									//
//	Some parts of the implementation					//
//  are taken/inspired from Ogg123 code	and 			//
//	QMMP project http://qmmp.ylsoftware.com/index_en.php//
//  THE Ogg123 SOURCE CODE IS (C) COPYRIGHT 2000-2001	//
//  by Stan Seibert <volsung@xiph.org> AND OTHER 		//
//  CONTRIBUTORS, http://www.xiph.org/					//
//														//
//  Some modifications & stripping made by me: 			//
//  <ludmiloff@gmail.com>								//
//////////////////////////////////////////////////////////

 
#include "vorbisdecoder.h"

#define BUFFER_SIZE 65536

static size_t oggread (void *buf, size_t size, size_t nmemb, void *src)
{
	if (!src) return 0;
	
	VorbisDecoder *decoder = (VorbisDecoder *) src;
	int len = decoder->input()->read((char *) buf, (size * nmemb));
	return len / size;
}

static int oggseek(void *src, int64_t offset, int whence)
{
	VorbisDecoder *decoder = (VorbisDecoder *) src;
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

static int oggclose(void *src)
{
    VorbisDecoder *decoder = (VorbisDecoder *) src;
    decoder->input()->close();
    return 0;
}

static long oggtell(void *src)
{
    VorbisDecoder *decoder = (VorbisDecoder *) src;
    long t = decoder->input()->pos();
    return t;
}


// Decoder class
VorbisDecoder::VorbisDecoder(QIODevice *i, QObject *parent)
	: QObject(parent), outputBuf(0), bufPtr(0), mInput(i)
{
	initialize();
}


VorbisDecoder::~VorbisDecoder()
{
	ov_clear(&oggFile);
	delete outputBuf;
}

void VorbisDecoder::initialize()
{
	state = Error;
	
	if (!input()) {
		qDebug("VorbisDecoder: cannot initialize.  No input");
		return;
    }
	
    if (!outputBuf) {
		outputBuf = new char[BUFFER_SIZE];
		bufPtr = outputBuf;
	}
	
	if (!input()->isOpen()) {
		if (!input()->open(QIODevice::ReadOnly)) {
			qWarning(qPrintable("VorbisDecoder: failed to open input. " + input()->errorString () + "."));
			return;
		}
	}

    ov_callbacks oggcb =
        {
            oggread,
            oggseek,
            oggclose,
            oggtell
        };
    if (ov_open_callbacks(this, &oggFile, NULL, 0, oggcb) < 0)
    {
        qWarning("VorbisDecoder: cannot open stream");
        return;
    }
		
	section = -1;
	beginSection = true;
	state = Ready;
    return;
}

void VorbisDecoder::reinitialize(QIODevice *i)
{
	mInput = i;
	if (outputBuf) {
		delete outputBuf;
		outputBuf = bufPtr = 0;
	}
	ov_clear(&oggFile);
	initialize();
	
}

int VorbisDecoder::read(ao_sample_format *format)
{	
	state = Decoding;
	int nbytes = BUFFER_SIZE;
    int lastSection;
	outputBytes = 0;
	bufPtr = outputBuf;
	
	if (beginSection) {
		vorbis_info *ogginfo = ov_info(&oggFile, -1);
		if (ogginfo) {
			format->rate = ogginfo->rate;
			format->channels = ogginfo->channels;
		}
		// just skip tags
		ov_comment (&oggFile, -1);
		beginSection = false;
	}
	
	while (nbytes >= format->channels * 2) {
		lastSection = section;
		int len = ov_read(&oggFile, bufPtr, nbytes, 0, 2, 1, &section);
		
		if (len == 0) {
			state = Finished;
			break;
		} else if (len == OV_HOLE) {
			qDebug() << "VorbisDecoder: hole in the stream";
		} else if (len < 0) {
			qDebug() << "VorbisDecoder: error in the stream";
			state = Error;
			break;
		} else { // len > 0
			outputBytes += len;
			bufPtr = (char *) ((unsigned char *) bufPtr + len);
			nbytes -= len;
			
			if (lastSection != section && lastSection != -1) {
				state = Finished;
				beginSection = true;
				break;
			}
		}
	}
	return outputBytes; 
}

QIODevice *VorbisDecoder::input()
{
	return mInput;
}

char *VorbisDecoder::buffer()
{
	return outputBuf;
}
