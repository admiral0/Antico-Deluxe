//////////////////////////////////////////////////////////////////
//  File      : vorbisdecoder.h                                 //
//  Written by: ludmiloff@gmail.com                             //
//  Copyright : GPL2                                            //
//	Some parts of the implementation                        //
//  are taken/inspired from Ogg123 code	and 			//
//	QMMP project http://qmmp.ylsoftware.com/index_en.php    //
//  THE Ogg123 SOURCE CODE IS (C) COPYRIGHT 2000-2001           //
//  by Stan Seibert <volsung@xiph.org> AND OTHER 		//
//  CONTRIBUTORS, http://www.xiph.org/				//
//								//
//  Some modifications & stripping made by me: 			//
//  <ludmiloff@gmail.com>					//
//////////////////////////////////////////////////////////////////

#ifndef __VORBISDECODER_H
#define __VORBISDECODER_H

#include <QtDebug>
#include <vorbis/vorbisfile.h>
#include <ao/ao.h>

class VorbisDecoder : public QObject
{
//	Q_OBJECT
public:

        enum DecoderState { NoneState, Ready, Decoding, Stopped, Finished, Info, Error };

        VorbisDecoder(QIODevice *i, QObject *parent = 0);
        virtual ~VorbisDecoder();

        // Standard Decoder API
        void initialize();
        void reinitialize(QIODevice *i);
        int read(ao_sample_format *format);
        char *buffer();
        void begin();

        int state;
        QIODevice *input();

//signals:
//	void decoderState(int newState);

private:
        // output buffer
        char *outputBuf;
        char *bufPtr;
        int outputBytes;
        int section;
        bool beginSection;

        //
        QIODevice *mInput;

        // OggVorbis file
        OggVorbis_File oggFile;
};

#endif
