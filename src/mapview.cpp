#include "mapview.h"

#define MAXZOOM 16
#define MINZOOM 4
#define MAXDOUBLE 1.79769313486231570e+308


#include <QMenu>

MapView::MapView( QWidget* parent) : QFrame(parent)
    , _center (0,0)
    , _startWaypoint(0)
    , _endWaypoint(0)
    , _tilewidth(256)
    , _tileheight(256)
    , _zoom(16)
      , _track(0)

{
    _imageLoader = new ImageLoader();
    connect(_imageLoader,SIGNAL(download_finished(QString)),this,SLOT(slotNewTileDownloaded(QString)));
    connect(_imageLoader,SIGNAL(download_status(int)),this,SLOT(slotUpdateDownloadStaus(int)));


    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setLineWidth(2);
    QSettings settings;
    iconDir = settings.value("ActivityIconDir").toString();

}


MapView::~MapView()
{
    if (_imageLoader->getQueueCount() > 0)
        slotAbortDownload();
    delete _imageLoader;

}


void MapView::drawMap()
{
    QPainter painter(this);

    QPoint centerMove = QPoint( width() / 2 , height() /2);
    QPoint move ( ( _center.x() - centerMove.x()) % _tilewidth,  ( _center.y() - centerMove.y()) % _tileheight );

    for ( int tx = -1 * _tilewidth - centerMove.x();  tx <= width()  +  1 * _tilewidth - centerMove.x();  tx += _tilewidth )
        for ( int ty = -1 * _tileheight - centerMove.y(); ty <= height() +  1 * _tileheight - centerMove.y(); ty += _tileheight )
        {
            QPoint t (tx,ty);
            QPoint imagepos = t - move + centerMove;
            QPointF imagecoordinates = QPointF( Calc::xAtZoomToLongitude ( _center.x() + t.x() - move.x() , _zoom),
                    Calc::yAtZoomToLatitude  ( _center.y() + t.y() - move.y() , _zoom));

            painter.drawPixmap( imagepos, getMapTile( imagecoordinates, _zoom ));
        }
    painter.end();
}


void MapView::drawPin( QPointF pin, QString pixmapName)
{
    QPainter painter(this);

    QPoint centerMove = QPoint( width() / 2 , height() /2);
    centerMove -= QPoint (11,44); // move pixmap because pin in the corner of the pixmap

    QPoint point (Calc::longitudeToXAtZoom(pin.x(), _zoom), Calc::latitudeToYAtZoom(pin.y(), _zoom ));

    painter.drawPixmap( point - _center + centerMove  , QPixmap(pixmapName));
    painter.end();
}


void MapView::drawPins()
{
    if (_startWaypoint)
        drawPin(QPointF (_startWaypoint->get_longitude(), _startWaypoint->get_latitude()), QString (iconDir + "start.gif"));

    if (_endWaypoint)
        drawPin(QPointF (_endWaypoint->get_longitude(), _endWaypoint->get_latitude()), QString (iconDir + "end.gif"));

}


void MapView::drawInfos()
{
    QPainter painter(this);
    painter.setPen( QColor( 0, 0, 100 ) );
    painter.drawText(16,16, "Zoom: " + QString::number(_zoom));
    painter.drawText(16,32, "Center: " + QString::number(center().y(), 'f', 9)+ " | " + QString::number(center().x(),'f',9 ));
    painter.end();
}


void MapView::drawTrack()
{
    if (!_track)
        return;
    if (! _track->count_waypoints() > 0) {
        qDebug() << "No Waypoints to draw in Track found.";
        return;
    }

    Waypoint* fromPoint;
    Waypoint* toPoint;

    QPoint centerMove = QPoint( width() / 2 , height() /2);

    QPainter painter(this);
    for (int i=0; i < _track->count_waypoints() -1 ; i ++)
    {
        fromPoint=_track->wayPoint(i);
        toPoint=_track->wayPoint(i+1);

        QPen pen = _track->pen();
        pen.setWidth(_zoom / 2);	// adapt pen width to zoom level

        if ( fromPoint->lt(_startWaypoint) || toPoint->gt(_endWaypoint))
        {
            QColor color = pen.color();
            color.setAlpha(50);
            pen.setColor ( color );

        }

        painter.setPen(pen);

        QPoint drawFrom (Calc::longitudeToXAtZoom(fromPoint->get_longitude(), _zoom), Calc::latitudeToYAtZoom(fromPoint->get_latitude(), _zoom ));
        QPoint drawTo   (Calc::longitudeToXAtZoom(toPoint->get_longitude(), _zoom), Calc::latitudeToYAtZoom(toPoint->get_latitude(), _zoom ));


        painter.drawLine (drawFrom - _center + centerMove , drawTo - _center + centerMove);

    }
    painter.end();
}


QPixmap MapView::getMapTile(QPointF p, int zoom)
{
    QPoint point = Calc::calcTileXY(p.y(), p.x(), zoom);
    return(_imageLoader->load_tile(zoom ,point.x(), point.y()));
}


void MapView::paintEvent(QPaintEvent *event)
{
    drawMap();
    drawTrack();
    drawInfos();
    drawPins();

    QFrame::paintEvent(event);
    return;
}


void MapView::mousePressEvent ( QMouseEvent * event )
{
    // moving the map starts
    if (event->button() == Qt::LeftButton)
        _temp = event->pos();

    // popup menu
    if (event->button() == Qt::RightButton)
    {
        _temp = event->pos();

        QMenu menu( this );
        menu.addAction ( "set waypoint as start", this, SLOT(slotSetStartPoint() ) );
        menu.addAction ( "set waypoint as end", this, SLOT(slotSetEndPoint() ) );
        menu.addAction ( "reset start and end", this, SLOT(slotResetStartEndPoint() ) );
        menu.exec(event->globalPos());
    }
}

void MapView::slotResetStartEndPoint()
{
    if (!_track)
        return;

    _startWaypoint = _track->at( 0 );
    _endWaypoint   = _track->at( _track->count_waypoints() -1 );
    repaint();
    emit startEndPointsMoved(_startWaypoint,_endWaypoint);
}

void MapView::slotSetStartPoint()
{
    _startWaypoint = findWaypointNear(_temp);
    repaint();
    if (_startWaypoint && _endWaypoint)
        emit startEndPointsMoved(_startWaypoint,_endWaypoint);
}

void MapView::slotSetEndPoint()
{
    _endWaypoint = findWaypointNear(_temp);
    repaint();
    if (_startWaypoint && _endWaypoint)
        emit startEndPointsMoved(_startWaypoint,_endWaypoint);

}


Waypoint* MapView::findWaypointNear(QPoint mpos)
{
    if ( !_track )
        return NULL;

    QPoint centerMove = QPoint( width() / 2 , height() /2);

    mpos -= centerMove;
    mpos += _center;


    QPointF mouse( Calc::xAtZoomToLongitude ( mpos.x(), _zoom ), Calc::yAtZoomToLatitude  ( mpos.y(), _zoom ) );

    double distance=MAXDOUBLE;

    QPointF bestpoint;
    QPointF tmppoint;
    Waypoint* bestWP = 0;

    for (int i=0; i < _track->count_waypoints() ; i ++)
    {
        tmppoint= QPointF(_track->wayPoint(i)->get_longitude(), _track->wayPoint(i)->get_latitude());
        QPointF dis = tmppoint - mouse;
        double dist = sqrt( dis.x() * dis.x() + dis.y() * dis.y() );

        if (dist < distance)
        {
            distance = dist;
            bestpoint = tmppoint;
            bestWP = _track->wayPoint(i);
        }
    }

    return bestWP;
}



void MapView::mouseMoveEvent ( QMouseEvent * event )
{
    // map is moved

    _center -= event->pos() - _temp;

    _temp = event->pos();

    repaint();

    emit centerChanged (QPoint (
                Calc::xAtZoomToLongitude ( _center.x(), _zoom ),
                Calc::yAtZoomToLatitude  ( _center.y(), _zoom ))
            );
}


void MapView::wheelEvent( QWheelEvent* event )
{
    QPointF oldCenter = center();

    if ( event->delta() == -120 && !(zoom() <= MINZOOM) )
        setZoom(zoom() -1);
    else if ( event->delta() == 120 && !(zoom() >= MAXZOOM) )
        setZoom(zoom() +1);

    setCenter(oldCenter);
}


void MapView::setTrack (Track* t)
{
    _track = t;
    if (_track) {
        _startWaypoint = _track->wayPoint( 0 );
        _endWaypoint   = _track->wayPoint( _track->count_waypoints() -1 );

        QPointF n = _track->max_north();
        QPointF s = _track->max_south();
        QPointF w = _track->max_west();
        QPointF e = _track->max_east();

        // calculate zoom
        QPointF p1( w.x(), n.y() );
        QPointF p2( e.x(), s.y() );


        int fzoom = MAXZOOM;
        for (int z = MAXZOOM ; z >= MINZOOM; z--)
        {
            int tmpwidth = Calc::longitudeToXAtZoom(p2.x(), z) - Calc::longitudeToXAtZoom(p1.x(), z) ;
            int tmpheight = Calc::latitudeToYAtZoom( p2.y(), z) - Calc::latitudeToYAtZoom( p1.y(), z) ;

            if ( tmpwidth < width() && tmpheight < height() )
            {
                fzoom = z;
                break;
            }
        }
        setZoom(fzoom);

        // calculate center
        QPointF c( w.x() + ((e.x() - w.x()) /2.0) , ( n.y() + (( s.y() - n.y()) / 2.0   )    ));
        setCenter(c);
    }

    repaint();
}


void MapView::setCenter (QPointF c)
{
    _center = QPoint( Calc::longitudeToXAtZoom(c.x(), _zoom), Calc::latitudeToYAtZoom( c.y(), _zoom) );

    emit centerChanged ( c );
    repaint();
}


void MapView::setZoom (int z)
{
    _zoom = z;
    repaint();

    emit zoomChanged ( _zoom );
}


QPointF MapView::center ()
{
    return QPointF (
            Calc::xAtZoomToLongitude ( _center.x(), _zoom ),
            Calc::yAtZoomToLatitude  ( _center.y(), _zoom ));
}


int MapView::zoom ()
{
    return _zoom;
}


void MapView::slotNewTileDownloaded(QString filename)
{
    qDebug() << "New Tile ready " << filename;
    repaint();
}

void MapView::slotUpdateDownloadStaus(int queue_size)
{
    qDebug() << "QueueSize: " << queue_size;
    emit downloadState(queue_size);

}

void MapView::slotAbortDownload()
{

    _imageLoader->abort_downloads();

}

void MapView::clearMap()
{
    setTrack(0);
    _startWaypoint = 0;
    _endWaypoint = 0;
    repaint();
}
