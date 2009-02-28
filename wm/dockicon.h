////////////////////////////////////////
//  File      : dockicon.h            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// app iconify added on dockbar

#ifndef DOCKICON_H
#define DOCKICON_H

#include "client.h"

class DockIcon : public QLabel
{
    Q_OBJECT

public:
    DockIcon(Client *c, QWidget *parent=0);
    ~DockIcon();
    void readSettings();
	void showClient();
	Client *getClient();

protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

public slots:
    void runMenu(QAction *);

signals:
    void showIcon(DockIcon *);
    void destroyIcon(DockIcon *);

private:
    Client *client;
    int dockHeight;
    QString titleColor;
    QPixmap pix;

};

#endif
