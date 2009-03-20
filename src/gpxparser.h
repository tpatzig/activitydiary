#ifndef GPXPARSER_H
#define GPXPARSER_H

#include <QObject>

#include <QXmlInputSource>
#include <QtGui>
#include <QDomDocument>
#include <qdebug.h>

#include "waypoint.h"
#include "Track.h"

class Track;

class GPXParser : public QObject
{

    public:

        GPXParser(QString filename);
        ~GPXParser();

        TrackList getTracks() { return singleTracks; }
        TrackList getAllInOneTrack() { return allInOneTrack; }
        QString getFileName() { return mFileName; }
        static void writeGPX(QString, Track*);
    private: 
        TrackList singleTracks;
        TrackList allInOneTrack;

        void parse_file(QFile&);
        bool open_file(QFile&);
        QString mFileName;
};

#endif

