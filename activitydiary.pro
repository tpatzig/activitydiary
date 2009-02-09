######################################################################
# Automatically generated by qmake (2.01a) So Feb 8 13:21:40 2009
######################################################################

TEMPLATE = app
TARGET = activitydiary 
DEPENDPATH += . src
INCLUDEPATH += . src
macx {
    INCLUDEPATH += /usr/local/qwt/include
    LIBS += /usr/lib/libqwt.dylib
}
else:unix {
    INCLUDEPATH += /usr/include/qwt
    LIBS += -L/usr/lib -L/usr/lib64 -lqwt
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
           src/waypoint.h
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

icons.path = /usr/share/activitydiary/icons
icons.files = icons/*.png

icon128.path = /usr/share/icons/hicolor/128x128/apps
icon128.files = icons/128x128/*

icon64.path = /usr/share/icons/hicolor/64x64/apps
icon64.files = icons/64x64/*

icon48.path = /usr/share/icons/hicolor/48x48/apps
icon48.files = icons/48x48/*

icon32.path = /usr/share/icons/hicolor/32x32/apps
icon32.files = icons/32x32/*

target.path = /usr/bin/

INSTALLS += target
INSTALLS += icons \
            icon128 \
            icon64 \
            icon48 \
            icon32

QT += xml network

