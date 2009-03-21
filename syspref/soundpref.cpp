//////////////////////////////////////////
//  File       	: soundpref.cpp			//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2					//
//////////////////////////////////////////

#include <AmeDirs>
#include <SoundTheme>
#include "soundpref.h"

SoundPref::SoundPref(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);

	ui.soundsLst->setColumnCount(2);
	ui.soundsLst->setColumnWidth(0, 420);

	QStringList hdr;
	hdr << "Name" << "Enabled";
	ui.soundsLst->setHeaderLabels(hdr);
	Sounds lst = AmeSoundTheme::global()->getAll();
	for (int i=0; i<lst.size(); i++) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui.soundsLst);
		it->setText(0, lst.at(i)->name);
		qDebug() << lst.at(i)->name;
	}

	readSettings();

	moduleName = "Sound";
}

SoundPref::~SoundPref()
{
	saveSettings();
}

void SoundPref::readSettings()
{
	AmeDirs *ame = AmeDirs::global();
	stg = new QSettings(ame->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
	
	mixerCard = stg->value("Mixer/mixer_card", "").toString();
	if (mixerCard.isEmpty()) {
		mixerCard = "hw:0";
		stg->setValue("Mixer/mixer_card", mixerCard);
	}
	mixerDevice = stg->value("Mixer/mixer_device", "").toString();
	if (mixerDevice.isEmpty()) {
		mixerDevice = "PCM";
		stg->setValue("Mixer/mixer_device", mixerDevice);
	}
}

bool SoundPref::saveSettings()
{
	stg->sync();
}
