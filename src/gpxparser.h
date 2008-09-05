#ifndef GPXPARSER_H
#define GPXPARSER_H

#include <QObject>

#include <QXmlInputSource>
#include <QtGui>
#include <QDomDocument>
#include <qdebug.h>

#include "waypoint.h"
#include "gpxparser.h"

class GPXParser : public QObject
{

public:

    GPXParser(QString filename);
    ~GPXParser();
    
    int count_waypoints();
    
    QPointF max_east();
    QPointF max_west();
    QPointF max_north();
    QPointF max_south();

    void add_waypoint(Waypoint*);

    QList<Waypoint*> get_waypoint_list();
    QList<Waypoint*> waypoint_list;
    
private:

    void parse_file(QFile&);
    bool open_file(QFile&);
    Waypoint* wpt;
  
    double maxLat;
    double maxLon;
    double minLat;
    double minLon;
    
    QPointF maxNorth;
    QPointF maxSouth;
    QPointF maxWest;
    QPointF maxEast;

};

#endif

