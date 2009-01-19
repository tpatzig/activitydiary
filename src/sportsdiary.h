#ifndef SPORTSDIARY_H
#define SPORTSDIARY_H 

#include <QMainWindow> 
#include <QtGui> 
#include <klocalizedstring.h> 
#include <QObject>
#include <qwt_legend.h>



#include "mapview.h" 
#include "gpxparser.h"
#include "Track.h" 
#include "waypoint.h"
#include "diagramcurve.h"
#include "adxparser.h"



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
  void slotImportTrack(QString);
  void slotSetZoom(int);
  void slotSetSliderValue(int);
  void slotWaypointSelected(Waypoint*);
  void slotStartEndPointsChanged(Waypoint* start,Waypoint* end);
  void slotSelectCurrentTrack( int );
  void slotAltitudeCheck(bool);
  void slotSpeedCheck(bool);
  void slotShowCalendar(bool);
  void slotShowCalendarWidget(bool);
  void slotShowTrackProperties(bool);
  void slotSetDiagramWidgetVisibility(bool);
  void slotSetInfoWidgetVisibility(bool);
  void slotSaveTrackInfos();
  void slotCalendarUpdated(const QDate&);
  void slotLoadSavedTrack(const QString&);
  void slotSetWindowModified(const QString &);
  void slotSetWindowModifiedDesc();
  void slotSetWindowTitle(const QString &);
  void slotLoadNextActivityDay();
  void slotLoadPrevActivityDay();
  void slotRemoveTrack();
  void slotClearAll();

private:

  void drawGraph( Waypoint* ,Waypoint* );
  void enableDisableDiagram(bool, DiagramCurve*, QString);
  void writeSettings();
  void readSettings();
  void clearTrackInfos();
  void closeEvent(QCloseEvent *event);

  Track *mCurrentTrack;
  TrackList tracks;
  GPXParser* parser;
  QwtLegend* legend;
  QString roundNumberAsString(double f);
  DiagramCurve* altitudeDiagram;
  DiagramCurve* speedDiagram;
  QSettings* settings;
  QString currentAdx;
  QString nextAvailAdx;
  QString previousAvailAdx;



};

#endif

