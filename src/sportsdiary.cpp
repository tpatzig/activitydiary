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
    connect(altitudeCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotAltitudeCheck(bool)));
    connect(speedCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotSpeedCheck(bool)));
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

    altitudeCheckBox->setChecked(true);
    speedCheckBox->setChecked(true);
    altitudeCheckBox->setEnabled(false);
    speedCheckBox->setEnabled(false);

    parser = 0;
    altitudeDiagram = 0;
    speedDiagram = 0;

}

SportsDiary::~SportsDiary()
{
    if (altitudeDiagram) {
        delete altitudeDiagram;
        altitudeDiagram = 0;
    }
    if (speedDiagram) {
        delete speedDiagram;
        speedDiagram = 0;
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

    QVector<double> timeValues;
    QVector<double> altitudeValues;
    QVector<double> speedValues;
    int tmpTime = 0;
    int startIndex = mCurrentTrack->indexOf(start);

    Waypoint* it = 0;
    Waypoint* tmpWp = 0;

    for(int i = startIndex; i <= mCurrentTrack->indexOf(end); i++) {
        it = mCurrentTrack->at(i);
        // only take wp from every full minute, to have a smooth curve in the diagramm //
        if (start->get_time().secsTo(it->get_time()) >= tmpTime ) {
            if (tmpWp) {

//          Altitude 0 should be a valid value, otherwise the gpx is incorrect
//          if ( curr_alt > 0 ) {
                 altitudeValues << it->get_altitude();

                 speedValues << mCurrentTrack->get_wp_speed(tmpWp,it) * 3.6;

                 timeValues << (start->get_time().secsTo(it->get_time())) / 60;
                 tmpTime += 60;
            }
            tmpWp = it;
        }
    }

    if (!altitudeDiagram) {
        altitudeDiagram = new DiagramCurve(diagram,"Altitude");
        altitudeDiagram->setColor(QColor(255,0,0));
        slotAltitudeCheck(altitudeCheckBox->isChecked());
    }
    if (!speedDiagram) {
        speedDiagram = new DiagramCurve(diagram,"Speed");
        speedDiagram->setColor(QColor(0,255,0));
        slotSpeedCheck(speedCheckBox->isChecked());
    }

    diagram->setAxisTitle(QwtPlot::xBottom,"Time in min");

    altitudeDiagram->setValues(timeValues,altitudeValues);
    speedDiagram->setValues(timeValues,speedValues);


    diagram->replot();

    altitudeCheckBox->setEnabled(true);
    speedCheckBox->setEnabled(true);

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

void SportsDiary::slotAltitudeCheck(bool checked)
{
    enableDisableDiagram(checked,altitudeDiagram,"Altitude in m");
}

void SportsDiary::slotSpeedCheck(bool checked)
{
    enableDisableDiagram(checked,speedDiagram,"Speed in km/h");

}

void SportsDiary::enableDisableDiagram(bool check, DiagramCurve* curve, QString axText)
{
    if (check) {

        qDebug() << "enable " << axText << " Diagram";
        if (diagram->itemList().size() == 1) {
            diagram->enableAxis(QwtPlot::yRight);
            curve->setAxis(QwtPlot::xBottom,QwtPlot::yRight);
            diagram->setAxisTitle(QwtPlot::yRight,axText);
        } else if (diagram->itemList().size() == 0) {
            curve->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
            diagram->setAxisTitle(QwtPlot::yLeft,axText);
        }
        curve->attachToDiagram(diagram);

    } else {
        curve->detach();

        qDebug() << "disable " << axText << " Diagram";
        if (curve->yAxis() == QwtPlot::yRight) {
            diagram->enableAxis(QwtPlot::yRight,false);

        } else if (diagram->itemList().size() > 0) {
            diagram->itemList()[0]->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
            diagram->setAxisTitle(QwtPlot::yLeft,diagram->axisTitle(QwtPlot::yRight ).text());
            diagram->enableAxis(QwtPlot::yRight,false);
            
        } else {
            diagram->setAxisTitle(QwtPlot::yLeft,"");
        }
    }
    diagram->replot();
}

