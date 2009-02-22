#ifndef SPORTSDIARY_H
#define SPORTSDIARY_H 

#include <QMainWindow> 
#include <QtGui> 
#include <QObject>
#include <qwt_legend.h>



#include "mapview.h" 
#include "gpxparser.h"
#include "Track.h" 
#include "waypoint.h"
#include "diagramcurve.h"
#include "adxparser.h"
#include "Physical.h"
#include "hrmparser.h"



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
  void slotImportPhysical();
  void slotImportPhysical(QString);
  void slotSetZoom(int);
  void slotSetSliderValue(int);
  void slotWaypointSelected(Waypoint*);
  void slotStartEndPointsChanged(Waypoint* start,Waypoint* end);
  void slotSelectCurrentTrack( int );
  void slotAltitudeCheck(bool);
  void slotSpeedCheck(bool);
  void slotHrCheck(bool checked);
  void slotShowCalendar(bool);
  void slotShowCalendarWidget(bool);
  void slotShowTrackProperties(bool);
  void slotSetDiagramWidgetVisibility(bool);
//  void slotSetInfoWidgetVisibility(bool);
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
  void slotRatingClicked(const QString&);

private:

  void drawGraph( Waypoint* ,Waypoint* );
  void enableDisableDiagram(bool, DiagramCurve*, QString);
  void writeSettings();
  void readSettings();
  void clearTrackInfos();
  void closeEvent(QCloseEvent *event);
  void enableRating(int);
  void disableRating(int);


  Track *mCurrentTrack;
  TrackList tracks;
  Physical _physical;
  GPXParser* parser;
  QwtLegend* legend;
  QString roundNumberAsString(double f);
  DiagramCurve* altitudeDiagram;
  DiagramCurve* speedDiagram;
  DiagramCurve* hrDiagram;
  QSettings* settings;
  QString currentAdx;
  QString nextAvailAdx;
  QString previousAvailAdx;
  QString iconDir;
  QMap<QString,QVariant> iconMap;
  int trackRating;

};

#endif

