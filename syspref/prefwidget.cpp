#include "prefwidget.h"

PrefWidget::PrefWidget(QWidget *parent)
	: QWidget(parent)
{
	state = Normal;
	moduleName = "";
	snd = new AmeSystemSound(this);
	snd->setSound(AmeSystemSound::ShadeUp);
	connect(this, SIGNAL(moduleIsUnlocked(bool)), this, SLOT(onUnlock(bool)));
}

PrefWidget::~PrefWidget()
{
	//snd->quit();
	delete snd;
}

void PrefWidget::execFunction(int fEnumerator, int param)
{
	// do nothing here
}

bool PrefWidget::aboutToClose()
{
	saveSettings();
	return true;
}

void PrefWidget::onUnlock(bool)
{
	snd->play();
}
