#ifndef TRACK_H
#define TRACK_H

#include "waypoint.h"
#include "Calc.h"

#include <QPointF>
#include <QPen>

#include "waypoint.h"

class Track 
{

    public:

        Track( WaypointList wpList = WaypointList() ) ;
        ~Track();

        QPointF max_east();
        QPointF max_west();
        QPointF max_north();
        QPointF max_south();

        void set_max_east(QPointF);
        void set_max_west(QPointF);
        void set_max_north(QPointF);
        void set_max_south(QPointF);


        void set_waypoint_list( const WaypointList& );

        int count_waypoints();

        Waypoint* wayPoint(int);

        WaypointList get_waypoint_list();

        QPen pen();
        void setPen();

        float get_overall_distance();
        float get_wp_distance(Waypoint*,Waypoint*);

        float get_overall_time();
        float get_wp_time(Waypoint*,Waypoint*);

        float get_wp_speed(Waypoint*, Waypoint*);
        float get_overall_avg_speed();
        float get_wp_avg_speed(Waypoint*, Waypoint*);

        float get_overall_avg_altitude();
        float get_wp_avg_altitude(Waypoint*,Waypoint*);

        QDate get_start_date();

        Waypoint* at(int);
        int indexOf(Waypoint*);
        Waypoint* first();
        Waypoint* last();

    private:

        WaypointList waypoint_list;

        Waypoint* wpt;

        QPointF maxNorth;
        QPointF maxSouth;
        QPointF maxWest;
        QPointF maxEast;

        QPen _trackPen;

};

// ################################################################################

class TrackList : public QList<Track*>
{
    public:
        TrackList();
        ~TrackList();
};

#endif

