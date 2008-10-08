#include "sportsdiary.h"

#include <QComboBox>

SportsDiary::SportsDiary(QObject* parent)
  :parser( 0 )
{
    Q_UNUSED(parent);

    setupUi(this);


    connect(mapFrame,SIGNAL(downloadState(int)),this,SLOT(slotUpdateDownloadState(int)));
    connect(abortButton,SIGNAL(clicked()),mapFrame,SLOT(slotAbortDownload()));
    connect(actionImport,SIGNAL(triggered()),this,SLOT(slotImportTrack()));
    connect(zoomSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSetZoom(int)));
    connect(mapFrame,SIGNAL(zoomChanged(int)),this,SLOT(slotSetSliderValue(int)));
    connect(mapFrame,SIGNAL(startEndPointsMoved(Waypoint*,Waypoint*)),this,SLOT(slotStartEndPointsChanged(Waypoint*,Waypoint*)));

    iconLabel->setPixmap(QPixmap("kompassberg.png"));
    abortButton->setEnabled(false);
    zoomSlider->setValue(mapFrame->zoom());
    cadenceLabel->setText("");
    heartrateLabel->setText("");
    // tempratureLabel->setText("");
    speedLabel->setText("");
    altitudeLabel->setText("");
    timeLabel->setText("");
    distanceLabel->setText("");

    mTrackCombo->setSizeAdjustPolicy( QComboBox::AdjustToContents );
    connect( mTrackCombo, SIGNAL( activated( int ) ),
             this,  SLOT( slotSelectCurrentTrack( int ) ) );

    parser = 0;
    curve1 = 0;
    curve2 = 0;

}

SportsDiary::~SportsDiary()
{
    if (curve1) {
        delete curve1;
        curve1 = 0;
    }
    if (curve2) {
        delete curve2;
        curve2 = 0;
    }
}

void SportsDiary::slotUpdateDownloadState(int queue)
{
    if (queue > 0) {
        loaderStatusLabel->setText("Tiles to Load: " + QString::number(queue));
        abortButton->setEnabled(true);
    } else {
        loaderStatusLabel->setText("Loading finished");
        abortButton->setEnabled(false);
    }

}

void SportsDiary::slotImportTrack()
{
  qDebug() << "importing track";
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open GPX File"), "./", tr("GPX Files (*.gpx)"));
  if (!fileName.isEmpty()) {
    if ( parser ) delete parser;

    parser = new GPXParser(fileName);
    qDebug() << "Read file with " << parser->tracks().count() << " tracks";

    TrackList tracks = parser->tracks();
    QListIterator<Track*> it( tracks );
    int cnt = 0;
    mTrackCombo->clear();

    while ( it.hasNext() ) {
      mTrackCombo->addItem( i18n( "Track No. %1" ).arg( cnt++ ) );
      it.next();
    }

    slotSelectCurrentTrack( 0 );
  } else
    qDebug() << "no track found";
}

void SportsDiary::slotSelectCurrentTrack( int num )
{
  TrackList tracks = parser->tracks();

  if ( num >= 0 && num < tracks.count() ) {
    qDebug() << "Selecting track " << num;
    mCurrentTrack = tracks[num];

    mDateLabel->setText( mCurrentTrack->at(0)->get_date().toString() );
    distanceLabel->setText(roundNumberAsString(mCurrentTrack->get_overall_distance()) + " km");
    timeLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_time()) + " min");
    speedLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_avg_speed() * 3.6 ) + " km/h");
    altitudeLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_avg_altitude()) + " m");

    mapFrame->setTrack( mCurrentTrack);

    drawGraph(mCurrentTrack->first(),mCurrentTrack->last());
  }
}

void SportsDiary::drawGraph( Waypoint* start, Waypoint* end)
{

    // Diagramm with GraphicsView //
    /*
       scene = new QGraphicsScene(diagramGraphicsView);
       int trackLength = 0;
    //      float maxHeight = 0.0;
    //      float minHeight = track->get_waypoint_list()[0]->get_altitude();
    Waypoint* tmp = 0;


    //      scene->setSceneRect(0,0,trackSecs,maxHeight-minHeight);

    tmp = 0;
    trackLength = 0;
    int trackLength0 = 0;
    foreach(Waypoint* wp,track->get_waypoint_list()) {
    if (tmp) {

    scene->addLine(trackLength,tmp->get_altitude() * -1,trackLength+1,wp->get_altitude()*-1,QPen(QColor(255,0,0)));
    }
    tmp = wp;
    trackLength++;
    }

    diagramGraphicsView->setScene(scene);
    diagramGraphicsView->show();
     */

    // diagramm with Qwt //


    //      diagramm->clear();
    if (curve1) {
        curve1->detach();
        delete curve1;
        curve1 = 0;
    }
    if (curve2) {
        curve2->detach();
        delete curve2;
        curve2 = 0;
    }

    diagramm->enableAxis(QwtPlot::yRight);
    diagramm->setAxisTitle(QwtPlot::yRight,"Speed in km/h");
    diagramm->setAxisTitle(QwtPlot::yLeft,"Altitude in m");
    diagramm->setAxisTitle(QwtPlot::xBottom,"Time in min");

    QwtArray<double> timeValues;
    QwtArray<double> altitudeValues;
    QwtArray<double> speedValues;
    int tmpTime = 0;
    int startIndex = mCurrentTrack->indexOf(start);

    Waypoint* it = 0;

    for(int i = startIndex; i <= mCurrentTrack->indexOf(end); i++) {
        it = mCurrentTrack->at(i);
        // only take wp from every full minute, to have a smooth curve in the diagramm //
        if (start->get_time().secsTo(it->get_time()) >= tmpTime) {

//          Altitude 0 should be a valid value, otherwise the gpx is incorrect
//          if ( curr_alt > 0 ) {
            altitudeValues << it->get_altitude();
            if (mCurrentTrack->indexOf(it) < mCurrentTrack->count_waypoints() -1)
                speedValues << mCurrentTrack->get_wp_speed(it,mCurrentTrack->at(mCurrentTrack->indexOf(it)+1)) * 3.6;
            timeValues << start->get_time().secsTo(it->get_time()) / 60;
            tmpTime += 60;
        }
    }

    curve1 = new QwtPlotCurve("Altitude");
    //      curve1->setBrush(Qt::red);
    curve1->setPen(QPen(QColor(255,0,0)));
    //curve1->setCurveAttribute(QwtPlotCurve::Fitted);
    curve1->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
    curve1->setData(timeValues,altitudeValues);
    curve1->attach(diagramm);

    curve2 = new QwtPlotCurve("Speed");
    //      curve2->setBrush(Qt::green);
    curve2->setPen(QPen(QColor(0,255,0)));
    //curve2->setCurveAttribute(QwtPlotCurve::Fitted);
    curve2->setAxis(QwtPlot::xBottom,QwtPlot::yRight);
    curve2->setData(timeValues,speedValues);
    curve2->attach(diagramm);


    diagramm->replot();
}

void SportsDiary::slotSetZoom(int zoom)
{
    QPointF old = mapFrame->center();
    mapFrame->setZoom(zoom);
    mapFrame->setCenter(old);
}

void SportsDiary::slotSetSliderValue(int zoom)
{
    zoomSlider->setValue(zoom);
}

void SportsDiary::slotWaypointSelected(Waypoint* wp)
{
    if (wp) {
        speedLabel->setText(QString::number( wp->get_speed() * 3.6 ) + " km/h");
        altitudeLabel->setText(QString::number(wp->get_altitude()) + " m");
        timeLabel->setText(wp->get_time().toString());
        // rounded value //
        // distanceLabel->setText(QString::number(roundNumber(mCurrentTrack->get_wp_distance(mCurrentTrack->at(0),wp)))+ " km");
        distanceLabel->setText(QString::number(mCurrentTrack->get_wp_distance(mCurrentTrack->at(0),wp))+ " km");
        // qDebug() << "Distance from Start to WP: " << mCurrentTrack->get_wp_distance(mCurrentTrack->at(0),wp);

    }
}

QString SportsDiary::roundNumberAsString(double x)
{
    QString val;
    val.setNum(x,'g',3);
    return val;
}

void SportsDiary::slotStartEndPointsChanged(Waypoint* start,Waypoint* end)
{
    drawGraph(start,end);
    distanceLabel->setText(roundNumberAsString(mCurrentTrack->get_wp_distance(start,end)) + " km");
    timeLabel->setText( roundNumberAsString(mCurrentTrack->get_wp_time(start,end)) + " min");
    speedLabel->setText( roundNumberAsString(mCurrentTrack->get_wp_avg_speed(start,end) * 3.6 ) + " km/h");
    altitudeLabel->setText( roundNumberAsString(mCurrentTrack->get_wp_avg_altitude(start,end)) + " m");
}

