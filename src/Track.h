#ifndef TRACK_H
#define TRACK_H

#include "waypoint.h"
#include "Calc.h"

#include <QPointF>
#include <QPen>

class Track 
{

public:

    Track( QList<Waypoint*> wpList = QList<Waypoint*>() ) ;
    ~Track();
    
    QPointF max_east();
    QPointF max_west();
    QPointF max_north();
    QPointF max_south();

    void set_max_east(QPointF);
    void set_max_west(QPointF);
    void set_max_north(QPointF);
    void set_max_south(QPointF);


    void set_waypoint_list(QList<Waypoint*> w);

    int count_waypoints();

    Waypoint* wayPoint(int);

    QList<Waypoint*> get_waypoint_list();

    QPen pen();
    void setPen();

    float get_overall_distance();
    float get_wp_distance(Waypoint*,Waypoint*);
    
    Waypoint* at(int);

private:

    QList<Waypoint*> waypoint_list;

    Waypoint* wpt;
  
    QPointF maxNorth;
    QPointF maxSouth;
    QPointF maxWest;
    QPointF maxEast;

    QPen _trackPen;

};

#endif

