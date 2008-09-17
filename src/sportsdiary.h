#ifndef SPORTSDIARY_H
#define SPORTSDIARY_H 
 
#include <QMainWindow> 
#include <QtGui> 
#include <klocalizedstring.h> 
#include <QObject>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_curve_fitter.h>

 
#include "mapview.h" 
#include "gpxparser.h"
#include "Track.h" 
#include "ui_sportsdiary.h" 

 
 
 
class SportsDiary : public QMainWindow, 
                          private Ui_SportsDiary 
{ 
Q_OBJECT

public:
	SportsDiary(QObject* parent = 0);
	~SportsDiary();

private:

	Track* 	track;
	GPXParser* parser;

	QHash<Track*,GPXParser*> trackHash;
	QGraphicsScene* scene;

	void drawGraph(Waypoint* start,Waypoint* end);
	QString roundNumberAsString(double f);

	QwtPlotCurve* curve1;
	QwtPlotCurve* curve2;

	

public slots:
	
	void slotUpdateDownloadState(int);
	void slotImportTrack();
	void slotSetZoom(int);
	void slotSetSliderValue(int);
	void slotWaypointSelected(Waypoint*);
	void slotStartEndPointsChanged(Waypoint* start,Waypoint* end);


};

#endif

