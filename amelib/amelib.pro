TEMPLATE = lib
VERSION = 0.0.1

QT += xml
CONFIG += warn_on qt thread dll x11 debug link_pkgconfig
PKGCONFIG += alsa ogg vorbisfile vorbis

RESOURCES = amelib.qrc

UI_HEADERS_DIR = forms
FORMS += forms/password.ui

OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include forms/

INCLUDEPATH +=

MAIN_HEADERS += global.h \
			utils.h \
			mime.h \
			sound.h \
			user.h \
			simplerc.h \
			settings.h \
			desktopfile.h \
			amedirs.h \
			clickablelabel.h \
			selectablewidget.h \
			navbaritem.h \
			navbargroup.h \
			navbar.h \
			pixmapwidget.h \
			icontheme.h \
			icon.h \
			staticiconlist.h \
			passworddlg.h \
			app.h
			
EXTRA_HEADERS += vorbisdecoder.h soundoutput.h

HEADERS += $$MAIN_HEADERS $$EXTRA_HEADERS 

FLAT_HEADERS += Global \
				Utils \
				Mime \
				Sound \
				User \
				SimpleRC \
				Settings \
				DesktopFile \
				AmeDirs \
				ClickableLabel \
				SelectableWidget \
				NavBarItem \
				NavBarGroup \
				NavBar \
				PixmapWidget \
				Icon \
				StaticIconList \
				PasswordDlg \
				IconTheme				
			
SOURCES += 	utils.cpp \
			mime.cpp \
			vorbisdecoder.cpp \
			soundoutput.cpp \
			sound.cpp \
			user.cpp \
			simplerc.cpp \
			settings.cpp \
			desktopfile.cpp \
			amedirs.cpp \
			clickablelabel.cpp \
			selectablewidget.cpp \
			navbaritem.cpp \
			navbargroup.cpp \
			navbar.cpp \
			pixmapwidget.cpp \
			icontheme.cpp \
			icon.cpp \
			staticiconlist.cpp \
			passworddlg.cpp \
			app.cpp

TARGET = ame

target.path=/usr/lib

flat_headers.files += $$MAIN_HEADERS
flat_headers.files += $$FLAT_HEADERS
flat_headers.path=/usr/include/ame

INSTALLS += target
INSTALLS += flat_headers

