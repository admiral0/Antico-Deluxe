TEMPLATE = app
QT += xml dbus
CONFIG += qt release

RESOURCES = syspref.qrc

UI_HEADERS_DIR = forms

FORMS += forms/wallpaper.ui \
		forms/appearance.ui \
		forms/dock.ui \
		forms/datetime.ui \
		forms/sound.ui

HEADERS += dbusadapt.h \
		sysprefwindow.h \
		browser.h \
		prefwidget.h \
		appearance.h \
		wallpaper.h	\
		dock.h \
		clockwidget.h \
		datetime.h \
		soundpref.h
		   

SOURCES += dbusadapt.cpp \
		sysprefwindow.cpp \
		browser.cpp \
		prefwidget.cpp \
		appearance.cpp \
		wallpaper.cpp \
		dock.cpp \
		clockwidget.cpp \
		datetime.cpp \
		soundpref.cpp \
		sysprefmain.cpp 

INCLUDEPATH += ../amelib /usr/include/ame/ 
LIBS += -L/usr/lib -L../amelib -lame
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include forms/
QMAKE_CLEAN += antico-pref

TARGET = syspref.app
target.path=/usr/bin

INSTALLS += target

