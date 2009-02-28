RESOURCES = theme/std.qrc

UI_HEADERS_DIR = forms
FORMS +=	forms/frm_quit.ui \
			forms/frm_about.ui	
			
HEADERS = defs.h \
	atoms.h \
	netwm.h \
	adx.h \
	dbusadaptor.h \
	decor.h \
	client.h \
	desktop.h \
	dockicon.h \
	dockbar.h \
	button.h \
	menu.h \
	currentapp.h \
	sysmenu.h \
	showdesktop.h \
	clock.h \
	volumectrl.h \
	panel.h \
	aboutdlg.h \
	quitdlg.h \
	alttab.h \

SOURCES = main.cpp \
	netwm.cpp \
	atoms.cpp \
    adx.cpp \
	adxsettings.cpp \
	keybindings.cpp \
	dbusadaptor.cpp \
	events.cpp \
	decor.cpp \
	client.cpp \
	x11management.cpp \
	desktop.cpp \
	dockicon.cpp \
	dockbar.cpp \
	button.cpp \
	menu.cpp \
	currentapp.cpp \
	sysmenu.cpp \
	showdesktop.cpp \
	clock.cpp \
	volumectrl.cpp \
	panel.cpp \
	aboutdlg.cpp \
	quitdlg.cpp \
	alttab.cpp

INCLUDEPATH += ../amelib /usr/include/ame/ 
LIBS += -L/usr/lib -L../amelib -lame
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include forms/
QMAKE_CLEAN += antico-deluxe
TEMPLATE = app
QT += dbus
CONFIG += qt release

TARGET = antico-deluxe
target.path=/usr/bin

theme_files.files += theme/default/ame-logo.png \
	theme/default/ame-logo-active.png \
	theme/default/ame-logo1.png \
	theme/default/dockbar.png \
	theme/default/dockicon.png \
	theme/default/application.png \
	theme/default/header.png \
	theme/default/show-desk.png \
	theme/default/show-desk-active.png
theme_files.path=/usr/share/themes/antico/default

session_files.files += antico-deluxe.desktop
session_files.path=/usr/share/xsessions

exec_files.files += antico-deluxe-session
exec_files.path=/usr/bin

INSTALLS += target exec_files theme_files session_files
