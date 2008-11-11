#include <QList>

#include "waypoint.h"
#include "Track.h"

Track::Track(WaypointList wpList)
{
    if (wpList.size())
        set_waypoint_list(wpList);

    // default pen (a yellow highlighter)
    QColor color( 200, 255, 0 ) ;
    color.setAlpha(140);
    _trackPen.setColor ( color );
    _trackPen.setWidth( 5 );
    _trackPen.setCapStyle ( Qt::FlatCap );
    _trackPen.setJoinStyle( Qt::RoundJoin );

}

Track::~Track()
{
    if (count_waypoints() > 0) {
         qDeleteAll(waypoint_list.begin(),waypoint_list.end());
         waypoint_list.clear();
    }
}


int Track::count_waypoints() {
    return waypoint_list.count();
}

Waypoint* Track::wayPoint(int i)
{
    return waypoint_list[i];
}



QPointF Track::max_east() {
    return maxEast;
}

QPointF Track::max_west() {
    return maxWest;
}

QPointF Track::max_north() {
    return maxNorth;
}

QPointF Track::max_south() {
    return maxSouth;
}

void Track::set_waypoint_list( const WaypointList& w)
{
    waypoint_list = w;
}

WaypointList Track::get_waypoint_list() {
    return waypoint_list;
}

QPen Track::pen()
{
    return _trackPen;
}


void Track::set_max_east(QPointF p)
{
    maxEast = p;
}
void Track::set_max_west(QPointF p)
{
    maxWest = p;
}
void Track::set_max_north(QPointF p)
{
    maxNorth = p;
}
void Track::set_max_south(QPointF p)
{
    maxSouth = p;
}

float Track::get_overall_distance() {
    float distance = 0;
    Waypoint* tmp = 0;

    foreach(Waypoint* wp,waypoint_list) {
        if (tmp) {
            distance += Calc::distance_in_km(tmp->get_latitude(),tmp->get_longitude(),wp->get_latitude(),wp->get_longitude());
        }
        tmp = wp;
    }
    return distance;
}

float Track::get_wp_distance(Waypoint* wp1,Waypoint* wp2) {

    Waypoint* tmp = 0;
    float distance = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        if (tmp)
            distance += Calc::distance_in_km(tmp->get_latitude(),tmp->get_longitude(),at(i)->get_latitude(),at(i)->get_longitude());
        tmp = at(i);
    }
    return distance;
}

float Track::get_overall_time() {
    float time_in_sec = 0;
    Waypoint* tmp = 0;

    foreach(Waypoint* wp,waypoint_list) {
        if (tmp) {
            time_in_sec += tmp->get_time().secsTo(wp->get_time());
        }
        tmp = wp;
    }
    return time_in_sec / 60;
}

float Track::get_wp_time(Waypoint* wp1,Waypoint* wp2) {
    Waypoint* tmp = 0;
    float time_in_sec = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        if (tmp)
            time_in_sec += tmp->get_time().secsTo(at(i)->get_time());
        tmp = at(i);
    }
    return time_in_sec / 60;
}

float Track::get_wp_speed(Waypoint* wp1,Waypoint* wp2) {
    float time = get_wp_time(wp1,wp2) * 60;
    float distance = get_wp_distance(wp1,wp2) * 1000;

    // return speed in m/s
    return distance/time;
}

float Track::get_overall_avg_speed()
{
    float avg_speed = 0;
    Waypoint* tmp = 0;

    foreach(Waypoint* wp,waypoint_list) {
        if (tmp) {
            avg_speed += get_wp_speed(tmp,wp);
        }
        tmp = wp;
    }
    return avg_speed / count_waypoints();

}

float Track::get_wp_avg_speed(Waypoint* wp1, Waypoint* wp2)
{
    float avg_speed = 0;
    Waypoint* tmp = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        if (tmp) {
            avg_speed += get_wp_speed(tmp,at(i));
        }
        tmp = at(i);
    }
    int count = indexOf(wp2) - indexOf(wp1);
    return avg_speed / count;

}

float Track::get_overall_avg_altitude()
{
    float avg_alt = 0;

    foreach(Waypoint* wp,waypoint_list) {
        avg_alt += wp->get_altitude();
    }

    return avg_alt / count_waypoints();
}

float Track::get_wp_avg_altitude(Waypoint* wp1, Waypoint* wp2)
{
    float avg_alt = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        avg_alt += at(i)->get_altitude();
    }
    int count = indexOf(wp2) - indexOf(wp1);

    return avg_alt / count;
}


Waypoint* Track::at(int i) {
    return waypoint_list.at(i);
}

int Track::indexOf(Waypoint* w) {
    return waypoint_list.indexOf(w);
}

Waypoint* Track::first() {
    return waypoint_list.at(0);
}

Waypoint* Track::last() {
    return waypoint_list.at(count_waypoints()-1);
}

QDate Track::get_start_date()
{
    return waypoint_list.at(0)->get_date();
}

// ################################################################################

TrackList::TrackList() : QList<Track*>()
{

}

TrackList::~TrackList()
{

}
