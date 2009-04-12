#ifndef __SYSPREF_H
#define __SYSPREF_H

#include <QtGui>
#include <StaticIconList>
#include "browser.h"

class PrefWidget;
class DBusAdaptor;

class SysPref : public QMainWindow
{
	Q_OBJECT
public:
	SysPref(int module);
	~SysPref();

	void createToolbar();
	void setupLaunchers();
	void addModule();
	void discardPrevious();
	void discardNext();

	void remoteFunction(int fEnumerator, int param);

public slots:
	void onShowMain();
	void onShowPrev();
	void onShowNext();
	void onWallpaper();
	void onAppearance();
	void onDock();
	void onDateTime();
	void onSound();

protected:
	void closeEvent(QCloseEvent *event);

private:
	AmeStaticIconList *personal, *hardware, *network, *system;
	Browser *browser;
	QToolBar *toolbar;
	QAction *prevAct, *nextAct, *showAllAct;
	QStackedWidget *stack;
	PrefWidget *prevWidget, *nextWidget;
	// DBus control
	QObject obj;
	DBusAdaptor *WMCtrl;
};

#endif
