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

#include "gpxparser.h"
#include "waypoint.h"
#include "Track.h"

GPXParser::GPXParser(QString filename) {
    qDebug() << "parsing the gpx file";
    mFileName = filename;
    QFile file(filename);
    if (open_file(file))
        parse_file(file);
    else
        qDebug() <<"Can't open file" << filename << " for reading.";
}

GPXParser::~GPXParser()
{
    if (singleTracks.size() > 0) {
        qDeleteAll(singleTracks.begin(),singleTracks.end());
    }
    singleTracks.clear();
    if (allInOneTrack.size() > 0) {
        allInOneTrack[0]->set_waypoint_list(WaypointList());
        delete allInOneTrack[0];
    }
}

void GPXParser::parse_file(QFile &file)
{

    QDomDocument document;
    document.setContent(&file);
    QDomElement mainElement = document.documentElement();

    WaypointList allPoints;
    QPointF allMaxNorth(0,0);
    QPointF allMaxSouth(0,0);
    QPointF allMaxWest(0,0);
    QPointF allMaxEast(0,0);
   

    QDomElement trk = mainElement.firstChildElement("trk");
    for ( ; !trk.isNull(); trk = trk.nextSiblingElement( "trk" ) ) {
        WaypointList points;
        Waypoint* prev_wpt = 0;

        double maxLat = 0.0;
        double maxLon = 0.0;
        double minLat = 0.0;
        double minLon = 0.0;

        QPointF maxNorth;
        QPointF maxSouth;
        QPointF maxWest;
        QPointF maxEast;

        QDomElement trkseg = trk.firstChildElement("trkseg");
        for ( ; !trkseg.isNull(); trkseg = trkseg.nextSiblingElement( "trkseg" ) ) {
            QDomElement trkpt = trkseg.firstChildElement("trkpt");
            for (; !trkpt.isNull(); trkpt = trkpt.nextSiblingElement("trkpt")) {

                double lat = trkpt.attributeNode("lat").value().toDouble();
                double lon = trkpt.attributeNode("lon").value().toDouble();
                float alt = trkpt.firstChildElement("ele").text().toFloat();

              

                QString date_time_string =  trkpt.firstChildElement("time").text();

                QDateTime date_time;

                if (!date_time_string.isEmpty()) {
                    QStringList date_list = date_time_string.split('T')[0].split('-');
                    QString sTime = date_time_string.split('T')[1].split('Z')[0];

					if(sTime.split('.').size() == 1) {
						sTime += ".0";
					}

                    QDate date(date_list[0].toInt(),date_list[1].toInt(),date_list[2].toInt());
                    QTime time = QTime::fromString(sTime, "hh:mm:ss.z");
                    date_time.setDate(date);
                    date_time.setTime(time);
                }

                int sat = trkpt.firstChildElement("sat").text().toInt();
                float speed =  trkpt.firstChildElement("extensions").firstChildElement("rmc:speed").text().toFloat();
                float course = trkpt.firstChildElement("extensions").firstChildElement("rmc:course").text().toFloat();

                Waypoint *wpt = new Waypoint( lat,lon,sat,alt,speed,course,date_time );

                if (prev_wpt) {
		    float wp_speed = prev_wpt->get_speed_to_wp(wpt);
                    if ( (wp_speed * 3.6) > SPEED_LIMIT ) {
                        continue;
                    }
                }
                
                points.append( wpt );
                allPoints.append(wpt);

                prev_wpt = wpt;

                // Search the most east, west, north and south point //
                if (lat > maxLat) {
                    maxLat = lat;
                    maxNorth.setY(lat);
                    maxNorth.setX(lon);
                }

                if (lon > maxLon) {
                    maxLon = lon;
                    maxEast.setY(lat);
                    maxEast.setX(lon);
                }

                if (minLat == 0 || lat < minLat) {
                    minLat = lat;
                    maxSouth.setY(lat);
                    maxSouth.setX(lon);
                }

                if (minLon == 0 || lon < minLon) {
                    minLon = lon;
                    maxWest.setY(lat);
                    maxWest.setX(lon);
                }
            }
        }
        if (points.size() > 0 ) {
            Track *track = new Track( points );
            track->set_custom_track(false);

            track->set_max_north( maxNorth );
            if (maxNorth.y() > allMaxNorth.y()) {
                allMaxNorth.setY(maxNorth.y());
                allMaxNorth.setX(maxNorth.x());
            }

            track->set_max_east( maxEast );
            if (maxEast.x() > allMaxEast.x()) {
                allMaxEast.setY(maxEast.y());
                allMaxEast.setX(maxEast.x());
            }

            track->set_max_south( maxSouth );
            if (allMaxSouth.y() == 0 || maxSouth.y() < allMaxSouth.y()) {
                allMaxSouth.setY(maxSouth.y());
                allMaxSouth.setX(maxSouth.x());
            }

            track->set_max_west( maxWest );
            if (allMaxWest.x() == 0 || maxWest.x() < allMaxWest.x()) {
                allMaxWest.setY(maxWest.y());
                allMaxWest.setX(maxWest.x());
            }

            singleTracks.append( track );
        }
    }
    Track *allOverTrack = new Track(allPoints);
    allOverTrack->set_custom_track(false);
    allOverTrack->set_max_north(allMaxNorth);
    allOverTrack->set_max_south(allMaxSouth);
    allOverTrack->set_max_west(allMaxWest);
    allOverTrack->set_max_east(allMaxEast);
    allInOneTrack.append(allOverTrack);
}

bool GPXParser::open_file(QFile &file) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    return true;
}


void GPXParser::writeGPX(QString filename, Track* track, QDateTime start, QDateTime end)
{
    track->at(0)->set_date_time(start);
    track->at(track->count_waypoints() - 1 )->set_date_time(end);

    QDomDocument doc;
    QDomElement gpx_string = doc.createElement("gpx");
    gpx_string.setAttribute("version","1.1");
    gpx_string.setAttribute("creator","ActivityDiary");
    doc.appendChild(gpx_string);
    QDomElement trk = doc.createElement("trk");
    gpx_string.appendChild(trk);
    QDomElement trkseg = doc.createElement("trkseg");
    trk.appendChild(trkseg);

    QFile file(filename);
    QFileInfo info(filename);
    QDir dir;
    if (!info.absoluteDir().exists())
        dir.mkpath(info.path());

    WaypointList waypoints = track->get_waypoint_list();
    foreach(Waypoint* wpt, waypoints) {
        QDomElement element = doc.createElement("trkpt");
        element.setAttribute("lat",QString::number(wpt->get_latitude()) );
        element.setAttribute("lon",QString::number(wpt->get_longitude()) );
        trkseg.appendChild(element);

        if (wpt->has_date_time()) {
            QDomElement dateElem = doc.createElement("time");
            QDomText t = doc.createTextNode(wpt->get_date().toString("yyyy-MM-dd") + "T" + wpt->get_time().toString() + "Z");
            dateElem.appendChild(t);
            element.appendChild(dateElem);

        }
    }


    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        qDebug() << "Could not open " << filename;
    else {
        QTextStream out(&file);
        out << doc.toString();
        file.flush();
        file.close();
    }
}
