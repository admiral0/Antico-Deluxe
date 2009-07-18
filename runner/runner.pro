TEMPLATE = app
QT += xml dbus
CONFIG += qt release

RESOURCES = 
UI_HEADERS_DIR = forms
FORMS += 
HEADERS += runnermain.h
SOURCES += runnermain.cpp

INCLUDEPATH += ../amelib /usr/include/ame/
LIBS += -L/usr/lib -L../amelib -lame
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include forms/

QMAKE_CLEAN += runner.app
TARGET = runner.app
target.path = /usr/bin
INSTALLS += target
