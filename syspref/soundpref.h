//////////////////////////////////////////
//  File       	: soundpref.h			//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2					//
//////////////////////////////////////////

#ifndef __SOUNDPREF_H
#define __SOUNDPREF_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_sound.h"

class SoundPref : public PrefWidget
{
	Q_OBJECT
public:
	SoundPref(QWidget *parent = 0);
	~SoundPref();

	virtual void readSettings();
	virtual bool saveSettings();

private:
	Ui::soundFrm ui;

	QString mixerCard, mixerDevice;
};

#endif
