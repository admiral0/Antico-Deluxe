//////////////////////////////////////////
//  File       : panel.cpp		//
//  Written by : ludmiloff@gmail.com	//
//  Copyright  : GPL2			//
//////////////////////////////////////////

#include "panel.h"
#include "menu.h"
#include "sysmenu.h"
#include "currentapp.h"
#include "showdesktop.h"
#include "clock.h"
#include "volumectrl.h"
#include "adx.h"
#include "client.h"

Panel::Panel(Adx *a, QWidget *parent) 
	: AmePixmapWidget(parent)
{
	app = a;
	currentWidget = 0;
	activeWidgetId = 0;
	m_Focus = false;
	setupGui();
}

Panel::~Panel(void)
{
	currentApp->close();
}

QRect Panel::screen(int indx)
{
	int si;
	QDesktopWidget *d = QApplication::desktop();
	if (indx == -1)
		si = d->primaryScreen();
	else
		si = indx;
	return d->screenGeometry(si);
}

int Panel::calcWidth(void) 
{
	return screen().width();
}

void Panel::setupGui(void)
{
	app->stg->beginGroup("Topbar");

	layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	// setup the components
	sysBtn = new SystemMenu(app, this);
	layout->addWidget(sysBtn);
	//layout->addSpacing(1);

	MenuButton *menuBtn = new MenuButton(this);
	layout->addWidget(menuBtn);
	//layout->addSpacing(1);

	currentApp = new CurrentApp(app, this);
	layout->addWidget(currentApp);
	//layout->addSpacing(1);
	
	//QSpacerItem *spacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	//layout->addItem(spacer);
	layout->addStretch();	
	//layout->addSpacing(width()-280);
	desktopBtn = new ShowDesktop(app, this);
	layout->addWidget(desktopBtn);
	layout->addSpacing(8);

	volume = new VolumeCtrl(this);
	volume->enableFeedback(app->stg->value("volume_feedback", true).toBool());
	layout->addWidget(volume);
	if (app->stg->value("show_volume_ctrl", true).toBool()) {
		volume->show();
	} else {
		volume->hide();
	}
	layout->addSpacing(8);

	ClockWidget *clock = new ClockWidget();
	layout->addWidget(clock);
	clock->show();
	
	setLayout(layout);

	setFixedHeight(TOP_PANEL_HEIGHT);
	resize(calcWidth(), TOP_PANEL_HEIGHT);
	move(QPoint(0,0));

	app->stg->endGroup();
}

bool Panel::isFocused()
{
	return m_Focus;
}

void Panel::grabFocus()
{
	m_Focus = true;
}

bool Panel::unfocusChilds() 
{
	m_Focus = false;
	if (currentWidget) currentWidget->deactivate();
	return true;
}


void Panel::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();

	QColor base = palette().button().color();
	int r = base.red();
	int g = base.green();
	int b = base.blue();

	//painter.fillRect(0, rect.bottom() - 2, width(), 3, QColor(r, g, b).darker(230));
	QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));
	linearGrad.setColorAt(0, QColor(r, g, b));
	linearGrad.setColorAt(1, QColor(r, g, b).darker(200));	
	painter.fillRect(0, rect.top(), width(), height(), linearGrad);
}

void Panel::mousePressEvent(QMouseEvent *event)
{		
	//qDebug() << "Panel button event";
	if(event->button() == Qt::LeftButton) {
		//m_Focus = true;
	}
}

void Panel::deactivateLast(GenericButton *c)
{
	if (currentWidget != c && currentWidget)
		currentWidget->deactivate();
}

void Panel::setCurrentWidget(GenericButton *c, WId id)
{
	currentWidget = c;
	activeWidgetId = id;
}

void Panel::clearCurrentWidget()
{
	currentWidget = 0;
	activeWidgetId = 0;
}

WId Panel::getActiveWidgetId()
{
	return activeWidgetId;
}

void Panel::iconIsRemoved(Client *client)
{
	desktopBtn->deactivate();
	currentApp->setCurrent(client->appName, client);
	clearCurrentWidget();
}

void Panel::windowIsIconified(Client *client)
{
	currentApp->setIconified(client->appName, client);
}

// DBus callable functions

void Panel::enableSoundVolumeFeedback(bool enable)
{
	volume->enableFeedback(enable);
}

void Panel::showSoundVolumeCtrl(bool visible)
{
	if (visible)
		volume->show();
	else
		volume->hide();
}

void Panel::changeSoundDevices(const QString &card, const QString &mixer)
{
	volume->snd->reinitMixer(card, mixer);
}
