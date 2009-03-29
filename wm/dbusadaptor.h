//////////////////////////////////////////
//  File       	: dbusadaptor.h		//
//  Written by	: ludmiloff@gmail.com	//
//  Copyright  	: GPL2			//
//////////////////////////////////////////

#ifndef __DBUSADAPTOR_H
#define __DBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>

class Desktop;
class Adx;
class Dockbar;

class DBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.freedesktop.AnticoDeluxe.WMCtrl")

public:
	DBusAdaptor(Adx *a, Desktop *d, Dockbar *dck, QObject *obj);
	~DBusAdaptor();

public slots:
	Q_NOREPLY void changeWallpaper(const QString &path);
	Q_NOREPLY void changeHighlight(int r, int g, int b);
	Q_NOREPLY void changeStyle(const QString &style);
	Q_NOREPLY void changeDockSize(int factor);
	Q_NOREPLY void minimizeDblClick(bool active);
	Q_NOREPLY void setDockAutoHide(bool active);
	Q_NOREPLY void showDockFast();
	Q_NOREPLY void changeDockSpeed(int animFactor);

protected:
	Desktop *desktop;
	Adx *app;
	Dockbar *dock;	
};

#endif

