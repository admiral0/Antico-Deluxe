#ifndef RUNNERMAIN_H
#define RUNNERMAIN_H

#include <QtGui>

class RunnerApp : public QApplication
{
	Q_OBJECT

public:
	RunnerApp();
	~RunnerApp();

	bool isRunning();

};

#endif // RUNNERMAIN_H
