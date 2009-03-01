#ifndef __SOUNDVOLUME_H
#define __SOUNDVOLUME_H

#include "button.h"
#include <Sound>

class Panel;
	
class SliderWidget : public QWidget
{
	Q_OBJECT
public:
	SliderWidget(QWidget *parent = 0);
	~SliderWidget();

public slots:
	void onSlider();
	void onSystemVolume(qreal);

public:
	QSlider *slider;
	AmeSystemSound *snd;
private:
	bool changing;
};

class VolumeCtrl : public GenericButton
{
public:
	VolumeCtrl(Panel *p, QWidget *parent = 0);
	~VolumeCtrl();

	virtual void activate();
	virtual void deactivate();

	//virtual void mousePressEvent(QMouseEvent *e);

	SliderWidget *volumeSlider;
};

#endif

