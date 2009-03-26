#include "dock.h"
#include "ui_dock.h"
#include <QtDBus>

DockPref::DockPref(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);
	moduleName = "Dock";
	readSettings();

	connect(ui.dockSizeSld, SIGNAL(sliderMoved(int)), this, SLOT(onChangeDockSize(int)));
	connect(ui.gbchkAutoHide,SIGNAL(clicked(bool)), this, SLOT(onShowHideDock(bool)));
	connect(ui.sldrSpeed, SIGNAL(sliderMoved(int)), this, SLOT(onChangeAnimSpeed(int)));
	connect(ui.sldrDelay, SIGNAL(sliderMoved(int)), this, SLOT(onChangeSHDelay(int)));
}

DockPref::~DockPref()
{
}

// DBus control functions
void DockPref::execFunction(int fEnumerator, int param)
{
	if (fEnumerator == 30) {
	ui.gbchkAutoHide->setChecked(param == 1);
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
	ui.gbchkAutoHide->setChecked(false);
	saveSettings();
}

void DockPref::onChangeAnimSpeed(int value)
{
  // Notify AnticoDeluxe WM for changing animation speed
  QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
		"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
		QDBusConnection::sessionBus(), this);
	if (!iface->isValid())
		qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
  else
    iface->call("changeAnimSpeed", value);
  saveSettings();
}

void DockPref::onChangeSHDelay(int value)
{
  // Notify AnticoDeluxe WM for changing show/hide delay
  QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe",
                                             "/", "org.freedesktop.AnticoDeluxe.WMCtrl",
                                             QDBusConnection::sessionBus(), this);
  if (!iface->isValid())
    qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
  else
    iface->call("changeSHDelay", value);
  saveSettings();
}

void DockPref::readSettings()
{
	// read AnticoDeluxe settings
	antico->beginGroup("Dockbar");
	int h = antico->value("dock_factor", 100).toInt();
	ui.dockSizeSld->setValue(h);
	bool f = antico->value("dock_autohide", false).toBool();
  ui.gbchkAutoHide->setChecked(f);
  int spd = -(antico->value("dock_animspeed", -10).toInt());
  ui.sldrSpeed->setValue(spd);
  int shd = antico->value("dock_shdelay", 2000).toInt();
  ui.sldrDelay->setValue(shd);
	antico->endGroup();
}

bool DockPref::saveSettings()
{
  // save AnticoDeluxe settings
  antico->beginGroup("Dockbar");
  antico->setValue("dock_factor", ui.dockSizeSld->value());
  antico->setValue("dock_autohide", ui.gbchkAutoHide->isChecked());
  antico->setValue("dock_animspeed", -(ui.sldrSpeed->value()));
  antico->setValue("dock_shdelay", ui.sldrDelay->value());
  antico->endGroup();
  antico->sync();
  return true;
}
