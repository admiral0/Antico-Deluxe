#include "volumectrl.h"
#include "panel.h"

SliderWidget::SliderWidget(QWidget *parent)
	:  QWidget(parent)
{	
	QFont f = QFont(font());
	//f.setBold(true);
	f.setWeight(75);
	f.setPointSize(14);
	setFont(f);
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(5, 2, 5, 5);
	
	QLabel *plus = new QLabel("+");
	plus->setFont(f);
	plus->setAlignment(Qt::AlignHCenter);
	layout->addWidget(plus);
	
	slider = new QSlider(Qt::Vertical);
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->setSpacing(0);
	hlayout->setContentsMargins(0, 0, 0, 0);
	hlayout->addWidget(slider);
	layout->addLayout(hlayout);
	
	QLabel *minus = new QLabel("-");
	minus->setAlignment(Qt::AlignHCenter);
	minus->setFont(f);
	layout->addWidget(minus);
	
	setLayout(layout);
	setFixedWidth(28);
	
	snd = new AmeSystemSound();
	snd->setEmbedSound(AmeSystemSound::Popup);
	
	slider->setMinimum(0);
	slider->setMaximum(100);
	if (snd->isMuted())
		slider->setValue(0);
	else
		slider->setValue(snd->volume());
	
	connect(slider, SIGNAL(sliderReleased()), this, SLOT(onSlider()));
	connect(snd, SIGNAL(systemVolumeChanged(int)), this, SLOT(onSystemVolume(int)));
	
	changing = false;
}

void SliderWidget::onSlider()
{
	changing = true;
	snd->setVolume(slider->value());
	changing = false;
	snd->play();
	qDebug() << "VOLUME = " << snd->volume();
}

void SliderWidget::onSystemVolume(int volume)
{
	if (!changing) {
		snd->setVolume(volume);
	}
}

SliderWidget::~SliderWidget()
{
}

VolumeCtrl::VolumeCtrl(Panel *p, QWidget *parent)
	: GenericButton(p, parent)
{
	can_Activated = false;
	setFixedWidth(28);
	setIconSize(QSize(17, 16));
	
	setImages(QPixmap(":/default/volume0.png"), QPixmap(":/default/volume0-active.png"));
	
	volumeSlider = new SliderWidget();
}

VolumeCtrl::~VolumeCtrl()
{
}

void VolumeCtrl::activate()
{
	if (!(activated) ) {
		panel->deactivateLast(this);
		volumeSlider->move(this->mapToGlobal(QPoint(0, height())));
		volumeSlider->show();
		activated = true;
		panel->setCurrentWidget(this, volumeSlider->winId());
	}
}

void VolumeCtrl::deactivate()
{
	if (activated) {
		volumeSlider->hide();
		activated = false;
		panel->unfocusChilds();
		panel->clearCurrentWidget();
		m_Focus = false;
		update();
	}
}

/*
void VolumeCtrl::mousePressEvent(QMouseEvent *)
{
	panel->grabFocus();
	if (!activated) {
		activated = true;
		m_Focus = true;
	} else {
		activated = false;
		m_Focus = false;
	}
	panel->unfocusChilds();
	update();
	
}
*/

