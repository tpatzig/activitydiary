#ifndef SPORTSDIARY_H
#define SPORTSDIARY_H 

#include <QMainWindow> 
#include <QtGui> 
#include <klocalizedstring.h> 
#include <QObject>



#include "mapview.h" 
#include "gpxparser.h"
#include "Track.h" 
#include "waypoint.h"
#include "diagramcurve.h"

#include "ui_sportnew3.h"





class SportsDiary : public QMainWindow, 
                    private Ui_SportsDiary 
{ 
  Q_OBJECT

public:
  SportsDiary(QObject* parent = 0);
  ~SportsDiary();

public slots:

  void slotUpdateDownloadState(int);
  void slotImportTrack();
  void slotSetZoom(int);
  void slotSetSliderValue(int);
  void slotWaypointSelected(Waypoint*);
  void slotStartEndPointsChanged(Waypoint* start,Waypoint* end);
  void slotSelectCurrentTrack( int );
  void slotAltitudeCheck(bool);
  void slotSpeedCheck(bool);

private:
  Track *mCurrentTrack;
  GPXParser* parser;

  QGraphicsScene* scene;

  void drawGraph( Waypoint* ,Waypoint* );
  QString roundNumberAsString(double f);

  DiagramCurve* altitudeDiagram;
  DiagramCurve* speedDiagram;

  void enableDisableDiagram(bool, DiagramCurve*, QString);
  QSettings* settings;



};

#endif

