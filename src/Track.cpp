#include "waypoint.h"
#include "Track.h"

Track::Track(QList<Waypoint*> wpList)
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
	// gpxparser already cares about that data structure -> no double delete !//

//  qDeleteAll(waypoint_list.begin(),waypoint_list.end());
//  waypoint_list.clear();
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

void Track::set_waypoint_list(QList<Waypoint*> w)
{
  waypoint_list = w;
}

QList<Waypoint*> Track::get_waypoint_list() {
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
