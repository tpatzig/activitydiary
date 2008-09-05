#ifndef _CALC_H
#define _CALC_H

#define _USE_MATH_DEFINES

#define EARTHRADIUS 6378137
#define EARTHCIRCUM ( (double) EARTHRADIUS * 2.0 * M_PI )


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
	static float distance_in_km(float lat1, float lon1, float lat2, float lon2);
	static float acos(float f);
	static float abso(float x);
};


#endif

