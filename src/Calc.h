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

#ifndef _CALC_H
#define _CALC_H

#define _USE_MATH_DEFINES

#define EARTHRADIUS 6378137
#define EARTHCIRCUM ( (double) EARTHRADIUS * 2.0 * M_PI )
#define MILES_PER_KM 1.609344
#define MILES_PER_M 1609.344


#include <QRect>
#include <QPointF>
#include <math.h>


class Calc 
{

    public:
        Calc();
        ~Calc();

    public:

        static int    latitudeToYAtZoom(double lat, int zoom );
        static double degToRad(double d);
        static double radToDeg(double r);
        static int    longitudeToXAtZoom(double lon, int zoom );
        static QPoint calcTileXY(float lat, float lon, long zoom);
        static QRectF calcLatLonFromTileXY(QPoint p, long zoom);
        static float  projectF(float lat);
        static float  projectMercToLat(float v);
        static double yAtZoomToLatitude(int x, int zoom );
        static double xAtZoomToLongitude(int x, int zoom );
        static float distance_in_miles(float lat1, float lon1, float lat2, float lon2);
        static float distance_in_km(float lat1, float lon1, float lat2, float lon2);
        static float distance_in_meter(float lat1, float lon1, float lat2, float lon2);
//      static float acos(float f);
        static float abso(float x);
};


#endif

