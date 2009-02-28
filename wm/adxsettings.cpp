//////////////////////////////////////////////////////////
//  File       : settings.cpp          					//
//  Written by : ludmiloff@gmail.com 					//
//  Some copy/paste code from original Antico project	//
//  Copyright  : GPL                   					//
//////////////////////////////////////////////////////////

#include "adx.h"

void Adx::readAdxSettings(void)
{
	// default path = $HOME/.config/AnticoDeluxe/AnticoDeluxe.conf"
	stg = new QSettings();

	/////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Dockbar");
	stg->setValue("dock_pix", QCoreApplication::applicationDirPath() + "/../share/themes/antico/default/dockbar.png");
	stg->value("dock_factor", 100).toInt();
	stg->value("dock_autohide", false).toBool();
	stg->endGroup(); //Dockbar
	/////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Dockicon");
	stg->setValue("d_icon_pix", QCoreApplication::applicationDirPath() + "/../share/themes/antico/default/dockicon.png");
	stg->setValue("title_color", "#ffffff"); //white
	stg->endGroup(); //Dockicon
	/////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Deskicon");
	stg->setValue("d_icon_pix", QCoreApplication::applicationDirPath() + "/../share/themes/antico/default/deskicon.png");
	stg->setValue("name_color", "#ffffff"); //white
	stg->endGroup(); //Deskicon
	/////////////////////////////////////////////////////////////////////////
	stg->beginGroup("Appearance");
	// Highlight color defaults to our AquaBlue
	QColor lastColor = QColor(stg->value("last_highlight", QColor(0, 0, 255).name()).toString());
	setHighlightColor(lastColor);
	minimizeDblClick = stg->value("minimize_dbl_click", false).toBool();

	stg->endGroup();
	/////////////////////////////////////////////////////////////////////////
	stg->sync();
	
	if (stg->status() == QSettings::AccessError)
		qDebug () << "Error on setting AnticoDeluxe.conf";
}
