#ifndef GPXPARSER_H
#define GPXPARSER_H

#include <QObject>

#include <QXmlInputSource>
#include <QtGui>
#include <QDomDocument>
#include <qdebug.h>

#include "waypoint.h"
#include "gpxparser.h"
#include "Track.h"

class Track;

class GPXParser : public QObject
{

    public:

        GPXParser(QString filename);
        ~GPXParser();

        TrackList tracks() { return mTracks; }
    private: 
        TrackList mTracks;

        void parse_file(QFile&);
        bool open_file(QFile&);
};

#endif

