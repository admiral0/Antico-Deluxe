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
		it->setText(1, "");
		if (lst.at(i)->enabled)
			it->setCheckState(1, Qt::Checked);
		else
			it->setCheckState(1, Qt::Unchecked);
		it->setData(1, Qt::UserRole, QVariant(lst.at(i)->soundId));
	}

	connect(ui.soundsLst, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onSoundItemChange(QTreeWidgetItem *, int)));
	connect(ui.soundsLst, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(onSoundItem(QTreeWidgetItem *, int)));

	snd = new AmeSystemSound();

	ui.volumeSldr->setMinimum(0);
	ui.volumeSldr->setMaximum(100);
	connect(ui.volumeSldr, SIGNAL(sliderMoved(int)), this, SLOT(onSlider(int)));

	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(checkVolume()));
	timer->start(100);

	refresh();
	readSettings();

	moduleName = "Sound";
}

SoundPref::~SoundPref()
{
	timer->stop();
	saveSettings();
}

void SoundPref::refresh()
{
	if (snd->isMuted()) {
		ui.volumeSldr->setValue(0);
		ui.muteChk->setChecked(true);
	} else {
		ui.volumeSldr->setValue(snd->volume());
		ui.muteChk->setChecked(false);
	}
}

void SoundPref::checkVolume()
{
	ui.volumeSldr->setValue(snd->volume());
}

void SoundPref::onSlider(int value)
{
	snd->setVolume(value);
	if (value > 0) {
		ui.muteChk->setChecked(false);
	}
}

void SoundPref::onSoundItemChange(QTreeWidgetItem *it, int col)
{
	if (col == 1) {
		// sound item enabled/disabled by user
		AmeSoundTheme::global()->setEnabled(it->data(1, Qt::UserRole).toInt(), (it->checkState(col) == Qt::Checked));
	}
}

void SoundPref::onSoundItem(QTreeWidgetItem *it, int col) {
	snd->setEmbedSound(it->data(1, Qt::UserRole).toInt(), true);
	snd->play();
}

void SoundPref::readSettings()
{
	AmeDirs *ame = AmeDirs::global();
	stg1 = new AmeSettings(ame->stdDir(AmeDirs::Configs) + "/Sound", QSettings::IniFormat);
	
	mixerCard = stg1->value("Mixer/mixer_card", "").toString();
	if (mixerCard.isEmpty()) {
		mixerCard = "hw:0";
		stg1->setValue("Mixer/mixer_card", mixerCard);
	}
	mixerDevice = stg1->value("Mixer/mixer_device", "").toString();
	if (mixerDevice.isEmpty()) {
		mixerDevice = "PCM";
		stg1->setValue("Mixer/mixer_device", mixerDevice);
	}
}

bool SoundPref::saveSettings()
{
	stg->sync();
	return true;
}
