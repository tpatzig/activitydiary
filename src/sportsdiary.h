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
#include "wizarddialog.h"



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
  void slotShowCalendar(bool);
  void slotShowCalendarWidget(bool);
  void slotShowTrackProperties(bool);
  void slotSetDiagramWidgetVisibility(bool);
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
  void slotCustomTrackChanged(Track*);
  void slotStartWizard();

private slots:
  void slotWizardFinished(QString, QString, QString);
  void slotWizardCancelled();
  void slotLeftDiagramChanged(const QString&);
  void slotRightDiagramChanged(const QString&);

private:

  void drawGraph( Waypoint* ,Waypoint* );
  void enableDisableDiagram(bool, DiagramCurve*, QString,int);
  void writeSettings();
  void readSettings();
  void clearTrackInfos();
  void closeEvent(QCloseEvent *event);
  void enableRating(int);
  void disableRating(int);
  void setActualDiagramContent(DiagramCurve*, DiagramCurve*,QString, QString);
  DiagramCurve* getDiagramFromText(QString);
  QString getDiagramTextFromName(QString);


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

  ADWizard* adWizard;
  QDateTime manualStart;
  QDateTime manualEnd;
  QString HRM_File;

};

#endif

