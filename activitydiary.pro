# #####################################################################
# Automatically generated by qmake (2.01a) So Feb 8 13:21:40 2009
# #####################################################################
TEMPLATE = app
TARGET = activitydiary
DEPENDPATH += . \
    src
INCLUDEPATH += . \
    src
macx { 
    INCLUDEPATH += /usr/local/qwt/include
    INCLUDEPATH += /usr/include/qwt
    LIBS += /usr/lib/libqwt.dylib
}
else:unix { 
    INCLUDEPATH += /usr/include/qwt \
            /usr/include/qwt-qt4

    exists( /usr/lib/libqwt-qt4* ) {
        LIBS += -L/usr/lib -lqwt-qt4
    }
    exists( /usr/lib64/libqwt-qt4* ) {
        LIBS += -L/usr/lib64 -lqwt-qt4
    }
    exists( /usr/lib/libqwt.* ) {   
        LIBS += -L/usr/lib -lqwt
    }
    exists( /usr/lib64/libqwt.* ) {   
        LIBS += -L/usr/lib64 -lqwt
    }
}

# Input
HEADERS += src/adxparser.h \
    src/Calc.h \
    src/calendar.h \
    src/diagramcurve.h \
    src/gpxparser.h \
    src/hrmparser.h \
    src/imageloader.h \
    src/loader.h \
    src/mainwindow.h \
    src/mapview.h \
    src/Physical.h \
    src/sportsdiary.h \
    src/Track.h \
    src/waypoint.h \
    src/wizarddialog.h \
    src/wizardpage.h
FORMS += src/activitydiary.ui
SOURCES += src/adxparser.cpp \
    src/Calc.cpp \
    src/calendar.cpp \
    src/diagramcurve.cpp \
    src/gpxparser.cpp \
    src/hrmparser.cpp \
    src/imageloader.cpp \
    src/loader.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mapview.cpp \
    src/Physical.cpp \
    src/sportsdiary.cpp \
    src/Track.cpp \
    src/waypoint.cpp \
    src/wizarddialog.cpp \
    src/wizardpage.cpp
icons.path = /usr/share/activitydiary/icons
icons.files = icons/*.png
icons.files += icons/*.gif
target.path = /usr/bin/
ts.path = /usr/share/activitydiary/ts
ts.files = ts/activitydiary*.ts
ts.files += ts/activitydiary*.qm
!macx and unix { 
    icon128.path = /usr/share/icons/hicolor/128x128/apps
    icon128.files = icons/128x128/*
    icon64.path = /usr/share/icons/hicolor/64x64/apps
    icon64.files = icons/64x64/*
    icon48.path = /usr/share/icons/hicolor/48x48/apps
    icon48.files = icons/48x48/*
    icon32.path = /usr/share/icons/hicolor/32x32/apps
    icon32.files = icons/32x32/*
    desktopfile.path = /usr/share/applications/kde4
    desktopfile.files = src/activitydiary.desktop
    INSTALLS += icon128 \
        icon64 \
        icon48 \
        icon32 \
        desktopfile
}
INSTALLS += target
INSTALLS += icons
INSTALLS += ts
QT += xml \
    network
TRANSLATIONS += activitydiary_de_DE.ts
