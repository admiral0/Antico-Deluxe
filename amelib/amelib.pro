TEMPLATE = lib
VERSION = 0.2

QT += xml dbus
CONFIG += warn_on \
    qt \
    thread \
    dll \
    x11 \
    release \
    link_pkgconfig

PKGCONFIG += alsa \
    ogg \
    vorbisfile \
    vorbis \
    ao

RESOURCES = amelib.qrc
UI_HEADERS_DIR = forms
FORMS += forms/password.ui
OBJECTS_DIR += ../build
MOC_DIR += ../build
QMAKE_INCDIR += /usr/include \
    forms/
INCLUDEPATH += 

MAIN_HEADERS += \
        global.h \
        utils.h \
        mime.h \
        soundtheme.h \
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
        helpbutton.h \
        rectbutton.h \
        gradbutton.h \
        toolbar.h \
        pixmapwidget.h \
        systemicon.h \
        icontheme.h \
        icon.h \
        staticiconlist.h \
        passworddlg.h \
        app.h

EXTRA_HEADERS += vorbisdecoder.h

HEADERS += $$MAIN_HEADERS \
    $$EXTRA_HEADERS

FLAT_HEADERS += \
        Global \
        Utils \
        Mime \
        SoundTheme \
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
        HelpButton \
        RectButton \
        GradientButton \
        ToolBar \
        PixmapWidget \
        SystemIcon \
        Icon \
        StaticIconList \
        PasswordDlg \
        IconTheme \
        AmeApp

SOURCES += \
        utils.cpp \
        mime.cpp \
        vorbisdecoder.cpp \
        soundtheme.cpp \
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
        helpbutton.cpp \
        rectbutton.cpp \
        gradbutton.cpp \
        toolbar.cpp \
        pixmapwidget.cpp \
        systemicon.cpp \
        icontheme.cpp \
        icon.cpp \
        staticiconlist.cpp \
        passworddlg.cpp \
        app.cpp

TARGET = ame
target.path = /usr/lib
flat_headers.files += $$MAIN_HEADERS
flat_headers.files += $$FLAT_HEADERS
flat_headers.path = /usr/include/ame
INSTALLS += target
INSTALLS += flat_headers
