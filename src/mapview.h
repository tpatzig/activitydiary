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

#ifndef _MAPVIEW_H
#define _MAPVIEW_H

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QObject>
#include <QFrame>
#include <QPointF>
#include <QPen>
#include <math.h>

#include "Calc.h"
#include "waypoint.h"
#include "Track.h"
#include "imageloader.h"

#define _USE_MATH_DEFINES


class MapView : public QFrame
{
    Q_OBJECT

    public:
        MapView(QWidget* parent=0);
        ~MapView();

        QPointF center ();
        int zoom ();

        void setTrack    ( Track* );
        void setCenter   ( QPointF );
        void setZoom     ( int );
        void clearMap();
        void setEditMode(bool);

    signals:
        void pointSelected( Waypoint* );
        void centerChanged ( QPointF );
        void zoomChanged ( int );
        void downloadState( int );
        void startEndPointsMoved(Waypoint*,Waypoint*);
        void customTrackChanged(Track*);


    private:
        void paintEvent( QPaintEvent *event );
        void mouseMoveEvent ( QMouseEvent * event );
        void mousePressEvent ( QMouseEvent * event );
        void mouseDoubleClickEvent( QMouseEvent * event );
        void wheelEvent(QWheelEvent* event);


        ImageLoader* _imageLoader;
        QPixmap getMapTile(QPointF p, int zoom);

        void drawMap();
        void drawPins();
        void drawPin( QPointF pin, QString pixmapName);
        void drawTrack();
        void drawInfos();
        void drawSinglePoint();

        QPoint _center;
        QPoint _temp;

        Waypoint* _startWaypoint;
        Waypoint* _endWaypoint;

        int _tilewidth;
        int _tileheight;
        int _zoom;

        Track* _track;
        Track* customTrack;
        WaypointList customWaypointList;

        Waypoint* findWaypointNear(QPoint mpos);

        QString iconDir;

        bool new_point_to_draw;
        bool editMode;


    public slots:
            void slotNewTileDownloaded(QString filename); 
        void slotUpdateDownloadStaus(int queue);
        void slotAbortDownload();


        void slotSetStartPoint();
        void slotSetEndPoint();
        void slotResetStartEndPoint();

};


#endif

