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

#include <QList>

#include "waypoint.h"
#include "Track.h"

Track::Track(WaypointList wpList)
{
    if (wpList.size())
        set_waypoint_list(wpList);

    // default pen (a yellow highlighter)
//    QColor color( 200, 255, 0 ) ;
    QColor color( 0, 100, 255 ) ;
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
    return ( get_wp_distance(this->first(),this->last()) );
}

float Track::get_wp_distance(Waypoint* wp1,Waypoint* wp2) {

    Waypoint* tmp = 0;
    float distance = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        if (tmp)
            distance += tmp->get_dist_to_wp_in_km(at(i));
        tmp = at(i);
    }
    return distance;
}

float Track::get_overall_time() {
    float time_in_sec = 0;
    Waypoint* tmp = 0;

    foreach(Waypoint* wp,waypoint_list) {
        if (wp->has_date_time()) {

            if (tmp) 
                time_in_sec += ( tmp->get_msecs_to_wp(wp) / 1000 );
            
            tmp = wp;
        }
    }
    return (time_in_sec > 0 ? time_in_sec / 60 : time_in_sec);
}

float Track::get_wp_time(Waypoint* wp1,Waypoint* wp2) {
    Waypoint* tmp = 0;
    float time_in_msec = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        if (at(i)->has_date_time()) {
            if (tmp) {
                time_in_msec += tmp->get_msecs_to_wp(at(i));
	    }
            tmp = at(i);
        }
    }
    return (time_in_msec > 0 ? time_in_msec / 1000 / 60 : time_in_msec);
}

float Track::get_overall_avg_speed()
{
    float avg_speed = 0;
    Waypoint* tmp = 0;

    foreach(Waypoint* wp,waypoint_list) {
        if (tmp) {
            avg_speed += tmp->get_speed_to_wp(wp);
        }
        tmp = wp;
    }
    return (avg_speed > 0 ? avg_speed / count_waypoints() : avg_speed) ;
}

float Track::get_wp_avg_speed(Waypoint* wp1, Waypoint* wp2)
{
    float avg_speed = 0;
    Waypoint* tmp = 0;

    for(int i = indexOf(wp1); i <= indexOf(wp2); i++) {
        if (tmp) {
            avg_speed += tmp->get_speed_to_wp(at(i));
        }
        tmp = at(i);
    }
    int count = indexOf(wp2) - indexOf(wp1);
    return (avg_speed > 0 ? avg_speed / count : avg_speed) ;
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

    return avg_alt > 0 ? avg_alt / count : avg_alt;
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

QTime Track::get_start_time()
{
    return waypoint_list.at(0)->get_time();
}

bool Track::is_custom_track()
{
    return custom_track;
}

void Track::set_custom_track(bool val)
{
    custom_track = val;
}

// ################################################################################

TrackList::TrackList() : QList<Track*>()
{

}

TrackList::~TrackList()
{

}
