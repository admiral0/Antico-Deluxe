///////////////////////////////////////////////////
//  File      : desktop      			         //
//  Originally Written by: g_cigala@virgilio.it  //
//  Modified by: ludmiloff@gmail.com			 //
//  Copyright : GPL                   			 //
///////////////////////////////////////////////////

#include <adx.h>
#include "desktop.h"

Desktop::Desktop(Adx *a, QWidget *parent) : AmePixmapWidget(parent)
{
	app = a;
	layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

Desktop::~Desktop()
{
}

void Desktop::init(int top_margin)
{
	app->stg->beginGroup("Desktop");
	QString wall_pix = app->stg->value("wallpaper", "").toString();

	setGeometry(0, top_margin, QApplication::desktop()->width(), QApplication::desktop()->height() - top_margin);
	// TODO: check is wallpaper file exists, if not - fill background with default blue
	if (!wall_pix.isEmpty()) {
		setBackground(QPixmap::fromImage(QImage(wall_pix)));
	} else
		setSolidColor(QColor(0x20, 0x60, 0xB0));
}


void Desktop::loadWallpaper(const QString &path)
{
	setBackground(QPixmap::fromImage(QImage(path)));
}

void Desktop::runMenu(QAction *)
{
}
