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
#include <qwt_legend.h>


#include "ui_activitydiary.h"





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
  TrackList tracks;
  GPXParser* parser;

  QwtLegend* legend;

  void drawGraph( Waypoint* ,Waypoint* );
  QString roundNumberAsString(double f);

  DiagramCurve* altitudeDiagram;
  DiagramCurve* speedDiagram;

  void enableDisableDiagram(bool, DiagramCurve*, QString);
  QSettings* settings;

  void writeSettings();
  void readSettings();



};

#endif

