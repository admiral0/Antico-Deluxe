#ifndef __AMESOUND_H
#define __AMESOUND_H

#include <Global>
#include <QFile>

class SoundOutput;
class VorbisDecoder;

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
	
	int volume();
	void setVolume(int);
	bool isMuted();
	
public slots:
	void stopPlaying();
	void onQuit();
	void onVolumeChanged(int);

signals:
	void systemVolumeChanged(int);
	
protected slots:
	void onDecoderState(int);

private:
	bool blocked;
	bool paused;

	QString source;
	QFile *input;

	SoundOutput *output;
	VorbisDecoder *decoder;
};

#endif
