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

#ifndef WAYPOINT_H
#define WAYPOINT_H

#define _USE_MATH_DEFINES 

#include <QObject>
#include <QString>
#include <QDate>
#include <math.h>
#include <qdebug.h>


class Waypoint : public QObject
{

    public:  
        Waypoint(double lat,double lon,int sat,float alt,float spd,float cs,QDateTime& time);
        ~Waypoint();

        double get_latitude();
        double get_longitude();

        double get_rel_x(int zoom);
        double get_rel_y(int zoom);

        int get_satelites();
        float get_altitude();

        float get_speed();
        int get_course();
        QDate get_date();
        QTime get_time();
        QString get_tile_path(int zoom);

        int get_floor_y(int zoom);
        int get_floor_x(int zoom);


        bool ge (Waypoint* w) const;
        bool le (Waypoint* w) const;
        bool gt (Waypoint* w) const;
        bool lt (Waypoint* w) const;

        void set_date_time(QDateTime);
        bool has_date_time();

    private: 

        double latitude;
        double longitude;

        int satelites;
        float altitude;
        float speed;
        float course;
        QDateTime date_time;

};

class WaypointList : public QList<Waypoint*>
{
    public:
        WaypointList();
        ~WaypointList();

};

#endif
