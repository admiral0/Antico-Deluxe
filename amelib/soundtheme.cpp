#include <AmeDirs>
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
	settings = new AmeSettings(AmeDirs::global()->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
	initEmbedSounds();
	readThemeSettings();
}

AmeSoundTheme::~AmeSoundTheme()
{
	saveThemeSettings();
	delete list;
	// FIXME:
	settings->sync();
	delete settings;
}

AmeSoundThemeItem *AmeSoundTheme::getSound(int soundId)
{
	return list->value(soundId);
}

void AmeSoundTheme::setEnabled(int soundId, bool enabled)
{
	AmeSoundThemeItem *it;
	if ((it = list->value(soundId)) != NULL) {
		it->enabled = enabled;
		// save this permanently
		saveThemeSettings();
	}
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
	list->insert(AmeSoundTheme::Click, new AmeSoundThemeItem("Click", ":/common/sounds/Click1.ogg", AmeSoundTheme::Click));
	list->insert(AmeSoundTheme::Error1, new AmeSoundThemeItem("Application Error", ":/common/sounds/Error1.ogg", AmeSoundTheme::Error1));
	list->insert(AmeSoundTheme::Error2, new AmeSoundThemeItem("System Error", ":/common/sounds/Error2.ogg", AmeSoundTheme::Error2));
	list->insert(AmeSoundTheme::Clap, new AmeSoundThemeItem("Clap", ":/common/sounds/Kopete_status.ogg", AmeSoundTheme::Clap));
	list->insert(AmeSoundTheme::Maximize, new AmeSoundThemeItem("Maximize", ":/common/sounds/Maximize.ogg", AmeSoundTheme::Maximize));
	list->insert(AmeSoundTheme::Minimize, new AmeSoundThemeItem("Minimize", ":/common/sounds/Minimize4.ogg", AmeSoundTheme::Minimize));
	list->insert(AmeSoundTheme::Popup, new AmeSoundThemeItem("Notification popup", ":/common/sounds/Popup.ogg", AmeSoundTheme::Popup));
	list->insert(AmeSoundTheme::Question, new AmeSoundThemeItem("Question", ":/common/sounds/Question.ogg", AmeSoundTheme::Question));
	list->insert(AmeSoundTheme::Background, new AmeSoundThemeItem("Information", ":/common/sounds/Question_background.ogg", AmeSoundTheme::Background));
	list->insert(AmeSoundTheme::RestoreDown, new AmeSoundThemeItem("Restore down", ":/common/sounds/Restore_down.ogg", AmeSoundTheme::RestoreDown));
	list->insert(AmeSoundTheme::RestoreUp, new AmeSoundThemeItem("Restore up", ":/common/sounds/Restore_up.ogg", AmeSoundTheme::RestoreUp));
	list->insert(AmeSoundTheme::Lock, new AmeSoundThemeItem("Lock/Unlock item", ":/common/sounds/Shade_down.ogg", AmeSoundTheme::Lock));
	list->insert(AmeSoundTheme::Shade, new AmeSoundThemeItem("Shade/Withdraw", ":/common/sounds/Shade_up.ogg", AmeSoundTheme::Shade));
}

void AmeSoundTheme::readThemeSettings()
{
	QStringList e = settings->listValue("sounds_enabled");
	if (e.isEmpty()) {
		// save defaults
		saveThemeSettings();
		return;
	}
	for (int i=0; i < e.size(); i++) {
		if (e.at(i) == "1")
			setEnabled(i, true);
		else
			setEnabled(i, false);
	}
}

void AmeSoundTheme::saveThemeSettings()
{
	QStringList e;
	Sounds lst = getAll();
	for (int i=0; i < lst.size(); i++) {
		e << (lst.at(i)->enabled ? "1" : "0");
	}
	settings->setListValue("sounds_enabled", e);
}

AME_GLOBAL_STATIC(AmeSoundTheme, gAmeSoundTheme)

AmeSoundTheme *AmeSoundTheme::global()
{
	return gAmeSoundTheme;
}
