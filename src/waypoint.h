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
