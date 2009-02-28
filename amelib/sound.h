#ifndef __AMESOUND_H
#define __AMESOUND_H

#include <Global>
#include <QFile>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

class AME_EXPORT AmeSystemSound : public QObject
{
	Q_OBJECT
public:
	enum SystemSounds {
		Click, Error1, Error2, Clap, 
		Maximize, Minimize, Popup,
		Question, Background, 
		RestoreDown, RestoreUp,
		ShadeDown, ShadeUp
	};
	
	AmeSystemSound(QObject *parent = 0);
	~AmeSystemSound();
	
	void setSound(int);
	bool play();
	virtual bool decode();
	
	qreal volume();
	void setVolume(qreal);
	bool isMuted();
	
public slots:
	void stopPlaying();
	void onQuit();
	void onVolumeChanged(qreal);

signals:
	void systemVolumeChanged(qreal);
	
protected:
	Phonon::MediaObject *media;
	Phonon::AudioOutput *audio;

private:
	bool blocked;
	bool paused;

	QString source;
	QFile *input;

	class SoundOutput;
	class VorbisDecoder;
	SoundOutput *output;
	VorbisDecoder *decoder;
};

#endif
