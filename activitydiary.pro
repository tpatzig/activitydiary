######################################################################
# Automatically generated by qmake (2.01a) So Feb 8 13:21:40 2009
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . src
macx {
    INCLUDEPATH += /usr/local/qwt/include
    LIBS += /usr/lib/libqwt.dylib
}
else:unix {
    INCLUDEPATH += /usr/include/qwt
    LIBS += /usr/lib/libqwt.so
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


QT += xml network
