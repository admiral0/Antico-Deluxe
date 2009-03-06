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
	if (snd->isMuted()) {
		v = 0;
		slider->setValue(0);
	} else {
		v = snd->volume();
		slider->setValue(v);
	}
	
	connect(slider, SIGNAL(sliderReleased()), this, SLOT(onSlider()));
	connect(snd, SIGNAL(systemVolumeChanged(int)), this, SLOT(onSystemVolume(int)));

	changing = false;
}

void SliderWidget::onSlider()
{
	changing = true;
	int v = slider->value();
	snd->setVolume(v);
	changing = false;
	snd->play();
	qDebug() << "VOLUME = " << snd->volume();
	emit volumeChanged(v);
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
	setIconSize(QSize(19, 18));	
	volumeSlider = new SliderWidget();
	
	connect(volumeSlider, SIGNAL(volumeChanged(int)), this, SLOT(updateIcon(int)));
	updateIcon(volumeSlider->v);
	//setImages(QPixmap(":/default/volume0.png"), QPixmap(":/default/volume0-active.png"));
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

void VolumeCtrl::updateIcon(int v)
{
	if (v >= 0 && v <= 10) {
		// volume0
		setImages(QPixmap(":/default/volume0.png"), QPixmap(":/default/volume0-active.png"));
	} else if (v > 10 && v <= 33) {
		// volume1
		setImages(QPixmap(":/default/volume1.png"), QPixmap(":/default/volume1-active.png"));
	} else if (v > 33 && v <= 75) {
		setImages(QPixmap(":/default/volume2.png"), QPixmap(":/default/volume2-active.png"));
		// volume2
	} else if (v > 76) {
		// volume3
		setImages(QPixmap(":/default/volume3.png"), QPixmap(":/default/volume3-active.png"));
	}
	update();
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

