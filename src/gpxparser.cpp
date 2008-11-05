#include "gpxparser.h"
#include "waypoint.h"
#include "Track.h"

GPXParser::GPXParser(QString filename) {
    qDebug() << "parsing the gpx file";
    QFile file(filename);
    if (open_file(file))
        parse_file(file);
    else
        qDebug() <<"Can't open " << filename << " for reading.";
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

                QString date_time_string =  trkpt.firstChildElement("time").text();
                QStringList date_list = date_time_string.split('T')[0].split('-');
                QStringList time_list = date_time_string.split('T')[1].split('Z')[0].split(':');

                QDate date(date_list[0].toInt(),date_list[1].toInt(),date_list[2].toInt());
                QTime time(time_list[0].toInt(),time_list[1].toInt(),time_list[2].toInt());
                QDateTime date_time(date,time);

                int sat = trkpt.firstChildElement("sat").text().toInt();
                float speed =  trkpt.firstChildElement("extensions").firstChildElement("rmc:speed").text().toFloat();
                float course = trkpt.firstChildElement("extensions").firstChildElement("rmc:course").text().toFloat();

                Waypoint *wpt = new Waypoint( lat,lon,sat,alt,speed,course,date_time );
                points.append( wpt );
                allPoints.append(wpt);
            }
        }
        if (points.size() > 0 ) {
            Track *track = new Track( points );

            track->set_max_north( maxNorth );
            qDebug() << "MAX North with Latitude: " << maxNorth.y();
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

