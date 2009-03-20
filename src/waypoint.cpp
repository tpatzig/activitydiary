#include "waypoint.h"

#include <QDebug>

Waypoint::Waypoint(double lat,double lon,int sat,float alt,float spd,float cs,QDateTime& time) {

    latitude = lat;
    longitude = lon;

    // number of satelites used to calculate this waypoint //
    satelites = sat;

    altitude = alt;


    // in meters per second at that point//
    speed = spd;

    // course in degrees at that point //
    course = cs;

    // in UTC, not localtime //
    date_time = time;

}


Waypoint::~Waypoint() {

}

// Some Getter Methods //

double Waypoint::get_latitude() {
    return latitude;
}

double Waypoint::get_longitude() {
    return longitude;
}

int Waypoint::get_satelites() {
    return satelites;
}

float Waypoint::get_speed() {
    return speed;
}

QDate Waypoint::get_date() {
    return date_time.date();
}

QTime Waypoint::get_time() {
    return date_time.time();
}

float Waypoint::get_altitude() {
    return altitude;
}

int Waypoint::get_course() {
    return course;
}

// Calculation of X and Y coordinates //

double Waypoint::get_rel_x(int zoom) {

    double x = (longitude + 180) / 360 * pow(2,zoom);
    return x;
}

double Waypoint::get_rel_y(int zoom) {

    double y = (1- log(tan(latitude * M_PI / 180) + 1 / cos(latitude * M_PI / 180)) / M_PI) / 2 * pow(2,zoom);
    return y;
}

// Calculation of rounded down X and Y coordinates //

int Waypoint::get_floor_x(int zoom) {

    int x = floor(get_rel_x(zoom));
    return x;
}

int Waypoint::get_floor_y(int zoom) {

    int y = floor(get_rel_y(zoom));
    return y;
}

// Path to the osm png //
QString Waypoint::get_tile_path(int zoom)
{
    int x = get_floor_x(zoom);
    int y = get_floor_y(zoom);
    QString path(QString::number(zoom) + "/" + QString::number(x) + "/" + QString::number(y));
    return path;
}


bool Waypoint::ge (Waypoint* w) const
{
    if (!w)
        return false;
    if (!w->has_date_time() || !date_time.isValid())
        return false;
       
    return date_time >= w->date_time;
}

bool Waypoint::le (Waypoint* w) const
{
    if (!w)
        return false;
    if (!w->has_date_time() || !date_time.isValid())
        return false;
       
    return date_time <= w->date_time;
}

bool Waypoint::gt (Waypoint* w) const
{
    if (!w)
        return false;
    if (!w->has_date_time() || !date_time.isValid())
        return false;
       
    return date_time > w->date_time;
}

bool Waypoint::lt (Waypoint* w) const
{
    if (!w)
        return false;
    if (!w->has_date_time() || !date_time.isValid())
        return false;

    return date_time < w->date_time;
}

void Waypoint::set_date_time(QDateTime dt)
{
    date_time = dt;
}

bool Waypoint::has_date_time()
{
    return date_time.isValid();
}

// ################################################################################

WaypointList::WaypointList() : QList<Waypoint*>()
{

}

WaypointList::~WaypointList()
{

}

