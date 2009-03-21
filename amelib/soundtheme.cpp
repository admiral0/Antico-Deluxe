#include "soundtheme.h"

AmeSoundThemeItem::AmeSoundThemeItem(const QString &aname, const QString &afileName, int id, bool aenabled)
{
	name = aname;
	fileName = afileName;
	soundId = id;
	enabled = aenabled;
}

AmeSoundThemeItem::~AmeSoundThemeItem()
{
}


AmeSoundTheme::AmeSoundTheme(QObject *parent)
	: QObject(parent)
{
	list = new SoundList;
	initEmbedSounds();
	readThemeSettings();
}

AmeSoundTheme::~AmeSoundTheme()
{
	saveThemeSettings();
	delete list;
}

AmeSoundThemeItem *AmeSoundTheme::getSound(int soundId)
{
	return list->value(soundId);
}

bool AmeSoundTheme::setSound(int soundId, AmeSoundThemeItem *sound)
{
	if (soundId > AmeSoundTheme::LastSoundId) {
		list->insert(soundId, sound);
		return true;
	} else
		return false;
}

Sounds AmeSoundTheme::getAll() const
{
	return list->values();
}



void AmeSoundTheme::initEmbedSounds()
{
	list->insert(AmeSoundTheme::Click, new AmeSoundThemeItem("Click", ":/common/sounds/Click1.ogg"));
	list->insert(AmeSoundTheme::Error1, new AmeSoundThemeItem("Application Error", ":/common/sounds/Error1.ogg"));
	list->insert(AmeSoundTheme::Error2, new AmeSoundThemeItem("System Error", ":/common/sounds/Error2.ogg"));
	list->insert(AmeSoundTheme::Clap, new AmeSoundThemeItem("Clap", ":/common/sounds/Kopete_status.ogg"));
	list->insert(AmeSoundTheme::Maximize, new AmeSoundThemeItem("Maximize", ":/common/sounds/Maximize.ogg"));
	list->insert(AmeSoundTheme::Minimize, new AmeSoundThemeItem("Minimize", ":/common/sounds/Minimize.ogg"));
	list->insert(AmeSoundTheme::Popup, new AmeSoundThemeItem("Notification popup", ":/common/sounds/Popup.ogg"));
	list->insert(AmeSoundTheme::Question, new AmeSoundThemeItem("Question", ":/common/sounds/Question.ogg"));
	list->insert(AmeSoundTheme::Background, new AmeSoundThemeItem("Information", ":/common/sounds/Question_background.ogg"));
	list->insert(AmeSoundTheme::RestoreDown, new AmeSoundThemeItem("Restore down", ":/common/sounds/Restore_down.ogg"));
	list->insert(AmeSoundTheme::RestoreUp, new AmeSoundThemeItem("Restore up", ":/common/sounds/Restore_up.ogg"));
	list->insert(AmeSoundTheme::Lock, new AmeSoundThemeItem("Lock/Unlock item", ":/common/sounds/Shade_down.ogg"));
	list->insert(AmeSoundTheme::Shade, new AmeSoundThemeItem("Shade/Withdraw", ":/common/sounds/Shade_up.ogg"));
}

void AmeSoundTheme::readThemeSettings()
{
}

void AmeSoundTheme::saveThemeSettings()
{
}

AME_GLOBAL_STATIC(AmeSoundTheme, gAmeSoundTheme)

AmeSoundTheme *AmeSoundTheme::global()
{
	return gAmeSoundTheme;
}
