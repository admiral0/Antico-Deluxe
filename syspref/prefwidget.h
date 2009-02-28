#ifndef __PREFWIDGET_H
#define __PREFWIDGET_H

#include <QWidget>
#include <QSettings>
#include <Sound>

class PrefWidget : public QWidget
{
	Q_OBJECT
public:

	enum State {
		Initializing,
		Normal,
		Closing,
	};

	PrefWidget(QWidget *parent = 0);
	virtual ~PrefWidget();

	virtual bool aboutToClose();
	virtual void readSettings() = 0;
	virtual bool saveSettings() = 0;
	
	virtual void execFunction(int, int);
	
	QString moduleName;
	
public slots:
	void onUnlock(bool);

signals:
	void moduleIsUnlocked(bool);

protected:
	QSettings *stg, *antico;
	int state;
	AmeSystemSound *snd;
};

#endif
