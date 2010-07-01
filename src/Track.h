/*
 *    ActivityDiary - The smart way to manage your GPS tracks
 *
 *    Copyright (C) 2009
 *
 *                - Tom Patzig <tom@activitydiary.org>
 *                - Thomas Goettlicher <thomas@activitydiary.org>
 *
 *    This file is part of ActivityDiary.
 *
 *    ActivityDiary is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    ActivityDiary is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with ActivityDiary.  If not, see <http://www.gnu.org/licenses/>.
*/

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

        float get_overall_avg_speed();
        float get_wp_avg_speed(Waypoint*, Waypoint*);

        float get_overall_avg_altitude();
        float get_wp_avg_altitude(Waypoint*,Waypoint*);

        QDate get_start_date();
        QTime get_start_time();

        Waypoint* at(int);
        int indexOf(Waypoint*);
        Waypoint* first();
        Waypoint* last();

        bool is_custom_track();
        void set_custom_track(bool);

    private:

        WaypointList waypoint_list;
	

        Waypoint* wpt;

        QPointF maxNorth;
        QPointF maxSouth;
        QPointF maxWest;
        QPointF maxEast;

        QPen _trackPen;

        bool custom_track;

};

// ################################################################################

class TrackList : public QList<Track*>
{
    public:
        TrackList();
        ~TrackList();
};

#endif

