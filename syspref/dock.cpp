#include "dock.h"
#include <QtDBus>

DockPref::DockPref(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);
	moduleName = "Dock";
	readSettings();
	connect(ui.dockSizeSld, SIGNAL(sliderMoved(int)), this, SLOT(onChangeDockSize(int)));
	connect(ui.showHideChk, SIGNAL(clicked(bool)), this, SLOT(onShowHideDock(bool)));
}

DockPref::~DockPref()
{
}

// DBus control functions
void DockPref::execFunction(int fEnumerator, int param)
{
	if (fEnumerator == 30) {
		ui.showHideChk->setChecked(param == 1);
	}
}

void DockPref::onShowHideDock(bool active)
{
	// Notify AnticoDeluxe WM for changing dock size
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe", 
		"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
		QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else 
		iface->call("setDockAutoHide", active);	
}

void DockPref::onChangeDockSize(int value)
{
	// Notify AnticoDeluxe WM for changing dock size
	QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe", 
		"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
		QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
	else 
		iface->call("changeDockSize", value);
	ui.showHideChk->setChecked(false);
	saveSettings();
}

void DockPref::readSettings()
{
	// read AnticoDeluxe settings
	antico = new QSettings("AnticoDeluxe", "AnticoDeluxe");
	antico->beginGroup("Dockbar");
	int h = antico->value("dock_factor", 100).toInt();
	ui.dockSizeSld->setValue(h);
	bool f = antico->value("dock_autohide", false).toBool();
	ui.showHideChk->setChecked(f);
	antico->endGroup();
}

bool DockPref::saveSettings()
{
	// save AnticoDeluxe settings
	antico->beginGroup("Dockbar");
	antico->setValue("dock_factor", ui.dockSizeSld->value());
	antico->endGroup();
	antico->sync();
	return true;
}
