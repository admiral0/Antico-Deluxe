/////////////////////////////////////////
//  File       : dockbar.cpp           //
//  Written by : g_cigala@virgilio.it  //
//  Modified by: ludmiloff@gmail.com   //
//  Copyright  : GPL                   //
/////////////////////////////////////////

// create the dockbar on bottom of the screen

#ifndef DOCKBAR_H
#define DOCKBAR_H

#include "defs.h"

#define MAX_DOCK_HEIGHT 48

class Client;
class DockIcon;
class Adx;

typedef QList<DockIcon *> DockIconsList;

class Dockbar : public QLabel
{
    Q_OBJECT

public:

	enum DockState {
		Normal,
		Hidden,
		Hiding,
		Showing,
		AboutToShow
	};

	Dockbar(Adx *a, QWidget *parent=0);
	~Dockbar();

	// Show / Hide flags
	void setAutoHide(bool active=true);
	int dockState;
	bool autoHide;
	
	// Client management functions
	void addClient(Client *);
	bool removeClient(Client *);
	bool removeAll();
	
	void setSizeFactor(int factor, bool doSave=true);
	void updateSize();
	void readSettings();
	void saveSettings();
	
	int actualHeight();

public slots:
	void removeIcon(DockIcon *);

	// Show / Hide functions
	void hideShowDock();
	void animateHide();
	void animateShow();
	void hide1Step();
	void show1Step();

signals:
	void clientRemoved(Client *);

private:
	QHBoxLayout *dockLayout;
	QHBoxLayout *iconLayout;
	QFrame *dockFrame;
	Adx *app;

	DockIconsList *iconsList;
	int dockFactor; 	// Dockbar scale factor (0,100) percents
	QString dockPix;	//
    int dockAnimSpeed;  //Speed of hiding/showing.
    int dockSHDelay;    //Wait this amount of time before hiding/showing.
    
	QTimer *timer, *hideTimer;
	bool checkCursor();
};
#endif
 
