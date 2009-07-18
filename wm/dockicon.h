////////////////////////////////////////
//  File      : dockicon.h            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// app iconify added on dockbar

#ifndef DOCKICON_H
#define DOCKICON_H

#include <DesktopFile>
#include "client.h"

class DockIcon : public QLabel
{
	Q_OBJECT

public:
	enum IconType {Task, Launcher, Applet};
	enum IconState {Running, Stopped, Inactive, Prelight};

	DockIcon(Client *c, int t=Task, QWidget *parent=0);
	DockIcon(const AmeDesktopFile &file, QWidget *parent=0);
	~DockIcon();

	void readSettings();
	void showClient();
	void updateSize(int w, int h);

	Client *client;
	int type;

protected:
	void mousePressEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);

public slots:
	void runMenu(QAction *);

signals:
	void showIcon(DockIcon *);
	void destroyIcon(DockIcon *);

private:
	int dockHeight;
	QString titleColor;
	QPixmap pix;
};

#endif
