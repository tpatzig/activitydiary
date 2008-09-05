#ifndef SPORTSDIARY_H
#define SPORTSDIARY_H 
 
#include <QMainWindow> 
#include <QtGui> 
#include <klocalizedstring.h> 
#include <QObject>
 
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

	void drawGraph(Track* track);
	int roundNumber(double f);

public slots:
	
	void slotUpdateDownloadState(int);
	void slotImportTrack();
	void slotSetZoom(int);
	void slotSetSliderValue(int);
	void slotWaypointSelected(Waypoint*);



};

#endif

