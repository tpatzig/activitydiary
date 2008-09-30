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

    signals:
        void pointSelected( Waypoint* );
        void centerChanged ( QPointF );
        void zoomChanged ( int );
        void downloadState( int );
        void startEndPointsMoved(Waypoint*,Waypoint*);


    private:
        void paintEvent( QPaintEvent *event );
        void mouseMoveEvent ( QMouseEvent * event );
        void mousePressEvent ( QMouseEvent * event );
        void wheelEvent(QWheelEvent* event);


        ImageLoader* _imageLoader;
        QPixmap getMapTile(QPointF p, int zoom);

        void drawMap();
        void drawPins();
        void drawPin( QPointF pin, QString pixmapName);
        void drawTrack();
        void drawInfos();

        QPoint _center;
        QPoint _temp;

        Waypoint* _startWaypoint;
        Waypoint* _endWaypoint;

        int _tilewidth;
        int _tileheight;
        int _zoom;

        Track* _track;

        Waypoint* findWaypointNear(QPoint mpos);


    public slots:
            void slotNewTileDownloaded(QString filename); 
        void slotUpdateDownloadStaus(int queue);
        void slotAbortDownload();


        void slotSetStartPoint();
        void slotSetEndPoint();
        void slotResetStartEndPoint();

};


#endif

