TEMPLATE = app
CONFIG += qt release

INCLUDEPATH += /usr/include/ame/
LIBS += -L/usr/lib -lame

HEADERS += testsound.h	

SOURCES += 	testsound.cpp 

TARGET = testsound
