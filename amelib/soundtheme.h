#ifndef SOUNDTHEME_H
#define SOUNDTHEME_H

#include <Global>
#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class AME_EXPORT AmeSoundThemeItem
{
public:
	AmeSoundThemeItem(const QString &aname, const QString &afileName, int id = 0, bool aenabled = true);
	~AmeSoundThemeItem();

	QString name;
	QString fileName;
	bool enabled;
	int soundId;
};

typedef QMap<int, AmeSoundThemeItem *> SoundList;
typedef QList<AmeSoundThemeItem *> Sounds;

class AME_EXPORT AmeSoundTheme : public QObject
{
public:

	enum SystemSounds {
		Click, Error1, Error2, Clap,
		Maximize, Minimize, Popup,
		Question, Background,
		RestoreDown, RestoreUp,
		Lock, Shade,
		//
		LastSoundId
	};

	AmeSoundTheme(QObject *parent = 0);
	~AmeSoundTheme();

	AmeSoundThemeItem *getSound(int soundId);
	bool setSound(int soundId, AmeSoundThemeItem *sound);
	Sounds getAll() const;

	static AmeSoundTheme *global();

private:
	SoundList *list;

	void initEmbedSounds();
	void readThemeSettings();
	void saveThemeSettings();
};

#endif // SOUNDTHEME_H
