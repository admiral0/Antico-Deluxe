#ifndef __PANEL_H
#define __PANEL_H

#include "defs.h"
#include <PixmapWidget>

#define TOP_PANEL_HEIGHT 23

class Adx;
class Client;
class GenericButton;
class CurrentApp;
class ShowDesktop;
class SystemMenu;

class Panel : public AmePixmapWidget
{

public:
	explicit Panel(Adx *a, QWidget *parent=0);
	virtual ~Panel(void);

	// various helper routines for Antico WM integration
	bool isFocused();
	void grabFocus();
	bool unfocusChilds();

	void deactivateLast(GenericButton *c);
	void setCurrentWidget(GenericButton *c, WId id);
	void clearCurrentWidget();
	WId getActiveWidgetId();

	void iconIsRemoved(Client *client);
	void windowIsIconified(Client *client);

	// Panel widgets - public for speedup access
	SystemMenu *sysBtn;
	ShowDesktop *desktopBtn;
	CurrentApp *currentApp;

private:

	QRect screen(int indx=-1);
	int calcWidth(void);
	void setupGui(void);

protected:
	// The design goals is to provide only 
	// top horizontal panel
	QHBoxLayout *layout;

	bool m_Focus;
	GenericButton *currentWidget;
	WId activeWidgetId;

	virtual void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event);

	Adx *app;
};


#endif

