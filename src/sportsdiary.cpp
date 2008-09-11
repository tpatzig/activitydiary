#include "sportsdiary.h"

SportsDiary::SportsDiary(QObject* parent)
{
	Q_UNUSED(parent);

	setupUi(this);

	
	connect(mapFrame,SIGNAL(downloadState(int)),this,SLOT(slotUpdateDownloadState(int)));
	connect(abortButton,SIGNAL(clicked()),mapFrame,SLOT(slotAbortDownload()));
	connect(actionImport,SIGNAL(triggered()),this,SLOT(slotImportTrack()));
	connect(zoomSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSetZoom(int)));
	connect(mapFrame,SIGNAL(zoomChanged(int)),this,SLOT(slotSetSliderValue(int)));
	connect(mapFrame,SIGNAL(startEndPointsMoved(Waypoint*,Waypoint*)),this,SLOT(slotStartEndPointsChanged(Waypoint*,Waypoint*)));
	 
	abortButton->setEnabled(false);
	zoomSlider->setValue(mapFrame->zoom());
	cadenceLabel->setText("");
	heartrateLabel->setText("");
	tempratureLabel->setText("");
	
	track = 0;
	parser = 0;
	curve1 = 0;
	curve2 = 0;

}

SportsDiary::~SportsDiary()
{
	foreach(Track* track,trackHash.keys()) {
		delete trackHash[track];
		trackHash.take(track);
		delete track;
	}	
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
			parser = new GPXParser(fileName);
			track = new Track(parser->get_waypoint_list());
			dateLabel->setText( track->at(0)->get_date().toString() );
			distanceLabel->setText(roundNumberAsString(track->get_overall_distance()) + " km");

			track->set_max_north(parser->max_north());
			track->set_max_south(parser->max_south());
			track->set_max_west(parser->max_west());
			track->set_max_east(parser->max_east());

			mapFrame->setTrack(track);
			trackHash[track] = parser;

			drawGraph(track->first(),track->last());
	}
}


void SportsDiary::drawGraph(Waypoint* start, Waypoint* end)
{

	// Diagramm with GraphicsView //
/*
	scene = new QGraphicsScene(diagramGraphicsView);
	int trackLength = 0;
//	float maxHeight = 0.0;
//	float minHeight = track->get_waypoint_list()[0]->get_altitude();
	Waypoint* tmp = 0;


//	scene->setSceneRect(0,0,trackSecs,maxHeight-minHeight);

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

	
//	diagramm->clear();
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
	int startIndex = track->indexOf(start);
	for(int i = startIndex; i <= track->indexOf(end); i++) {
		// only take wp from every full minute, to have a smooth curve in the diagramm //
		if (track->at(startIndex)->get_time().secsTo(track->at(i)->get_time()) >= tmpTime) {
			timeValues << track->at(startIndex)->get_time().secsTo(track->at(i)->get_time()) / 60;
			tmpTime += 60;
			altitudeValues << track->at(i)->get_altitude();
			speedValues << track->at(i)->get_speed() * 3.6;
		}
        }

	curve1 = new QwtPlotCurve("Altitude");
//	curve1->setBrush(Qt::red);
	curve1->setPen(QPen(QColor(255,0,0)));
	curve1->setCurveAttribute(QwtPlotCurve::Fitted);
	curve1->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
	curve1->setData(timeValues,altitudeValues);
     	curve1->attach(diagramm);

	curve2 = new QwtPlotCurve("Speed");
//	curve2->setBrush(Qt::green);
	curve2->setPen(QPen(QColor(0,255,0)));
	curve2->setCurveAttribute(QwtPlotCurve::Fitted);
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
//	 	distanceLabel->setText(QString::number(roundNumber(track->get_wp_distance(track->at(0),wp)))+ " km");
	 	distanceLabel->setText(QString::number(track->get_wp_distance(track->at(0),wp))+ " km");
	 	qDebug() << "Distance from Start to WP: " << track->get_wp_distance(track->at(0),wp);
	 	
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
	distanceLabel->setText(roundNumberAsString(track->get_wp_distance(start,end)) + " km");
}
