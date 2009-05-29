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

#include "Calc.h"
#include <QDebug>


Calc::Calc()
{
}

Calc::~Calc()
{
}


float Calc::projectF(float lat)
{
    //??
    lat = degToRad(lat);
    return log(tan(lat) + (1.0 / cos(lat)));
}


float Calc::projectMercToLat(float v) 
{
    //??
    return 180.0 / M_PI * atan(sinh(v));
}


QRectF Calc::calcLatLonFromTileXY(QPoint p, long zoom)
{
    //??
    float unit = 1.0 / pow(2 , zoom);

    double relY1 = p.y() * unit;
    double relY2 = relY1 + unit;
    double LimitY = projectF(85.0511);
    double RangeY = 2.0 * LimitY;
    relY1 = LimitY - RangeY * relY1;
    relY2 = LimitY - RangeY * relY2;
    float Lat1 = projectMercToLat(relY1);
    float Lat2 = projectMercToLat(relY2);
    unit = 360.0 / pow(2, zoom);
    float Lon1 = -180.0 + p.x() * unit;
    float Lon2 = Lon1 + unit;

    // Lat2, Long1, Lat1, Long1 + $Unit)); # S,W,N,E

    return QRectF(Lat1,Lon1,Lat2-Lat1,Lon2-Lon1);
}


QPoint Calc::calcTileXY(float lat, float lon, long zoom)
{
    //??

    float xf = (lon + 180.0) / 360.0 * pow (2.0, zoom);
    float yf = (1.0 - log(tan(lat * M_PI / 180.0) + 1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0 * pow(2, zoom);

    return QPoint(xf,yf);
}



double Calc::degToRad(double d)
{
    //OK
    return d / 180.0 * M_PI;
}

double Calc::radToDeg(double r)
{
    //OK
    return (r * 180.0) / M_PI;
}



// helper function - converts a latitude at a certain zoom into a y pixel
int Calc::latitudeToYAtZoom(double lat, int zoom ) 
{
    //OK
    double arc = (double) EARTHCIRCUM / ((1 << zoom) * 256.0);
    double sinLat = sin(degToRad(lat));
    double metersY = (double) EARTHRADIUS / 2.0 * log((1.0 + sinLat) / (1.0 - sinLat));
    return round (( ((double) EARTHCIRCUM/2.0)  - metersY ) / arc) ;
}

// helper function - converts a y pixel at a certain zoom to a latitude
double Calc::yAtZoomToLatitude(int y, int zoom ) 

{
    //OK
    double arc = (double) EARTHCIRCUM / ((1 << zoom) * 256.0);

    double arg = exp ( ( (EARTHCIRCUM/2.0) - ( y * arc ) ) / (EARTHRADIUS / 2.0) );

    double sinlat = ( (arg - 1.0 ) / (arg + 1.0) );

    return ( radToDeg ( asin( sinlat )));
}

//helper function - converts a longitude at a certain zoom into a x pixel
int Calc::longitudeToXAtZoom(double lon, int zoom )
{
    //OK
    double arc = (double) EARTHCIRCUM / ((1 << zoom) * 256.0);
    double metersX = (double) EARTHRADIUS * degToRad(lon);
    return round ((((double) EARTHCIRCUM/2.0)  + metersX) / arc);
}

//helper function - converts x pixel at a cetrain zoom  to a longitude 
double Calc::xAtZoomToLongitude(int x, int zoom )
{
    //OK
    double arc = (double) EARTHCIRCUM / ((1 << zoom) * 256.0);

    double lon = (( (double) x * arc ) - (EARTHCIRCUM/2) ) / (double) EARTHRADIUS;
    return radToDeg(lon);
}

float Calc::distance_in_km(float lat1, float lon1, float lat2, float lon2)
{


//  float distance = (acos(sin(lat1/180*M_PI)*sin(lat2/180*M_PI) + cos(lat1/180*M_PI)*cos(lat2/180*M_PI)*cos(lon1/180*M_PI-lon2/180*M_PI))*180*60/M_PI);


    float distance = (acos(sin(degToRad(lat1)) * sin(degToRad(lat2)) + 
                    cos(degToRad(lat1)) * cos(degToRad(lat2)) * cos(degToRad(lon1)-degToRad(lon2))) * 180*60/M_PI);

    // miles in km
    return (distance * MILES_PER_KM);

}
/*
float Calc::acos(float f)
{
    return (atan2(sqrt(abso(1 - pow(f,2))), f) );
}
*/

float Calc::abso(float x)
{
    return x >= 0 ? x : -x;
}
