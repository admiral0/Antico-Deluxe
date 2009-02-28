#include "sysprefwindow.h"
#include "prefwidget.h"
#include "appearance.h"
#include "wallpaper.h"
#include "dock.h"
#include "datetime.h"
#include "dbusadapt.h"

SysPref::SysPref(int module)
{
	setWindowTitle("System Preferences");
	setWindowIcon(QIcon(":/icons/object/system-preferences.png"));
	stack = new QStackedWidget;
	personal = new AmeStaticIconList("Personal", AmeStaticIconList::Odd);
	system = new AmeStaticIconList("System", AmeStaticIconList::Even);

	browser = new Browser;
	stack->addWidget(browser);
	createToolbar();

	setupLaunchers();
	setCentralWidget(stack);
	prevWidget = NULL;
	nextWidget = NULL;
	
	WMCtrl = new DBusAdaptor(this, &obj);
	QDBusConnection::sessionBus().registerObject("/", &obj);
	if (!QDBusConnection::sessionBus().registerService("org.freedesktop.AnticoPref")) {
		qDebug() << "UNABLE TO REGISTER DBUS SERVICE";
		exit(1);
	}
	
	resize(QSize(600, 350));
	
	switch (module) {
		case 1:
			onAppearance();
			break;
		case 2:
			onWallpaper();
			break;
		case 3:
			onDock();
			break;
		case 4:
			onDateTime();
			break;
	}
}

SysPref::~SysPref()
{
}

void SysPref::remoteFunction(int fEnumerator, int param)
{
	if (stack->currentIndex() == 0) return;
	PrefWidget *widget = (PrefWidget*) stack->widget(1);
	widget->execFunction(fEnumerator, param); 
}

void SysPref::onShowMain()
{
	if (stack->currentIndex() == 0) return;
	PrefWidget *widget = (PrefWidget*) stack->widget(1);
	widget->aboutToClose();
	stack->removeWidget(stack->widget(1));
	nextWidget = widget;
	if (prevWidget != NULL)
		delete prevWidget;
	prevWidget = NULL;
	stack->setCurrentIndex(0);
	resize(QSize(600, 350));
	setWindowTitle("System Preferences");
}

void SysPref::onShowPrev()
{
	if (prevWidget != NULL) {
	} else
		onShowMain();
}

void SysPref::onShowNext()
{
	if (nextWidget != NULL) {
		stack->addWidget(nextWidget);
		nextWidget->show();
		stack->setCurrentIndex(1);
		if (prevWidget != NULL) {
			delete prevWidget;
		}
		setWindowTitle(nextWidget->moduleName);
		prevWidget = NULL;
		nextWidget = NULL;
	}
}

void SysPref::createToolbar()
{
	toolbar = addToolBar("Main");
	prevAct = new QAction(QIcon(":/icons/tool/go-previous.png"), "Previous", this);
	connect(prevAct, SIGNAL(triggered()), this, SLOT(onShowPrev()));
	nextAct = new QAction(QIcon(":/icons/tool/go-next.png"),"Next", this);
	connect(nextAct, SIGNAL(triggered()), this, SLOT(onShowNext()));
	showAllAct = new QAction("Show All", this);
	connect(showAllAct, SIGNAL(triggered()), this, SLOT(onShowMain()));


	toolbar->addAction(prevAct);
	toolbar->addAction(nextAct);
	toolbar->addSeparator();
	toolbar->addAction(showAllAct);
}

void SysPref::onWallpaper()
{
	Wallpaper *frm = new Wallpaper();
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	prevWidget = NULL;
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);
}

void SysPref::onAppearance()
{
	Appearance *frm = new Appearance();
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	prevWidget = NULL;
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);	
}

void SysPref::onDock()
{
	DockPref *frm = new DockPref();
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	prevWidget = NULL;
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);	
}

void SysPref::onDateTime()
{
	DateTimePref *frm = new DateTimePref();
	stack->addWidget(frm);
	frm->show();
	stack->setCurrentIndex(1);
	prevWidget = NULL;
	nextWidget = NULL;
	setWindowTitle(frm->moduleName);	
}


void SysPref::addModule()
{
}

void SysPref::setupLaunchers()
{
	// Personal section
	qDebug() << "PERSONAL SECTION";

	AmeIconLink *appearance = new AmeIconLink("Appearance", ":/icons/object/appearance.png", "appearance", "");
	connect(appearance, SIGNAL(clicked()), this, SLOT(onAppearance()));

	AmeIconLink *desktop = new AmeIconLink("Desktop\nWallpaper", ":/icons/object/desktop.png", "desktop", "");
	connect(desktop, SIGNAL(clicked()), this, SLOT(onWallpaper()));

	//AmeIconLink *screensaver = new AmeIconLink("Screen\nSaver", path+"screensaver.png", "screensaver", "");
	
	AmeIconLink *dock = new AmeIconLink("Dock", ":/icons/object/dock.png", "dock", "");
	connect(dock, SIGNAL(clicked()), this, SLOT(onDock()));

	personal->addIcon(appearance);
	personal->addIcon(desktop);
	//personal->addIcon(screensaver);
	personal->addIcon(dock);

	browser->addSection(personal);

	qDebug() << "SYSTEM SECTION";
	AmeIconLink *aclock = new AmeIconLink("Date &\nTime", ":/icons/object/date-time.png", "dock", "");
	system->addIcon(aclock);
	connect(aclock, SIGNAL(clicked()), this, SLOT(onDateTime()));

	browser->addSection(system);

	browser->addStretch();
}

void SysPref::closeEvent(QCloseEvent *event)
{
	//writeSettings();
	event->accept();
}
