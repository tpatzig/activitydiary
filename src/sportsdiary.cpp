#include "sportsdiary.h"

#include <QComboBox>

SportsDiary::SportsDiary(QObject* parent)
  :parser( 0 )
{
    Q_UNUSED(parent);

    setupUi(this);

    settings = new QSettings();
    readSettings();
    qDebug() << iconDir;
    activitytype->addItem(QIcon(iconDir + iconMap["default"].toString()), "Select your Activity...");
    foreach (QString key, iconMap.keys()) {
        if (key == "default")
            break;
        else
            activitytype->addItem(QIcon(iconDir + iconMap[key].toString()), key);
    }

    setWindowIcon(QIcon(iconDir + "logo_64.png"));
    qDebug() << settings->fileName();

    legend = new QwtLegend;
    legend->setFrameStyle(QFrame::NoFrame|QFrame::Raised);
    diagram->insertLegend(legend, QwtPlot::RightLegend);
    
    calendarWidget->setFirstDayOfWeek(Qt::Monday);
    calToolButton->setIcon(QIcon(iconDir + "calendar.png"));
    propsButton->setIcon(QIcon(iconDir + "logo_32.png"));

    connect(mapFrame,SIGNAL(downloadState(int)),this,SLOT(slotUpdateDownloadState(int)));
    connect(mapFrame,SIGNAL(zoomChanged(int)),this,SLOT(slotSetSliderValue(int)));
    connect(mapFrame,SIGNAL(startEndPointsMoved(Waypoint*,Waypoint*)),this,SLOT(slotStartEndPointsChanged(Waypoint*,Waypoint*)));
    connect(mapFrame,SIGNAL(customTrackChanged(Track*)), this, SLOT(slotCustomTrackChanged(Track*)));

    connect(abortButton,SIGNAL(clicked()),mapFrame,SLOT(slotAbortDownload()));
    //connect(actionImport,SIGNAL(triggered()),this,SLOT(slotImportTrack()));
    //connect(actionImportPhysical,SIGNAL(triggered()),this,SLOT(slotImportPhysical()));
    connect(actionNewActivity,SIGNAL(triggered()),this,SLOT(slotStartWizard()));
    connect(zoomSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSetZoom(int)));
    connect(altitudeCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotAltitudeCheck(bool)));
    connect(speedCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotSpeedCheck(bool)));
    connect(hrCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotHrCheck(bool)));
    connect(mCalendarButton,SIGNAL(clicked(bool)),this,SLOT(slotShowCalendarWidget(bool)));
    connect(calToolButton,SIGNAL(clicked(bool)),this,SLOT(slotShowCalendar(bool)));
    connect(diagramDockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(slotSetDiagramWidgetVisibility(bool)));
//  connect(infoDockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(slotSetInfoWidgetVisibility(bool)));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(slotSaveTrackInfos()));
    connect(actionShow_Track_Settings,SIGNAL(toggled(bool)),leftGroupBox,SLOT(setVisible(bool)));
    connect(trackname,SIGNAL(textChanged(const QString&)),this,SLOT(slotSetWindowTitle(const QString &)));
    connect(propsButton,SIGNAL(clicked(bool)),this,SLOT(slotShowTrackProperties(bool)));
    connect(mNextDayButton,SIGNAL(clicked()),this,SLOT(slotLoadNextActivityDay()));
    connect(mPrevDayButton,SIGNAL(clicked()),this,SLOT(slotLoadPrevActivityDay()));
    connect(actionRemoveTrack,SIGNAL(triggered()),this,SLOT(slotRemoveTrack()));
    connect(actionClear,SIGNAL(triggered()),this,SLOT(slotClearAll()));
    connect( mTrackCombo, SIGNAL( activated( int ) ),this,SLOT( slotSelectCurrentTrack( int ) ) );

    connect(calendarWidget,SIGNAL(clicked(const QDate&)),calendar,SLOT(slotUpdateCurrentKW(const QDate&)));
    connect(calendar,SIGNAL(trackSelected(const QString&)),this,SLOT(slotLoadSavedTrack(const QString&)));
    connect(calendar,SIGNAL(calendarUpdated(const QDate&)),this,SLOT(slotCalendarUpdated(const QDate&)));

    connect(weather,SIGNAL(activated(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(quality,SIGNAL(activated(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(covering,SIGNAL(activated(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(profil,SIGNAL(activated(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(activitytype,SIGNAL(activated(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(trackname,SIGNAL(textEdited(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(temperature,SIGNAL(textEdited(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(mTrackCombo,SIGNAL(activated(const QString&)),this,SLOT(slotSetWindowModified(const QString &)));
    connect(descriptionTextBrowser,SIGNAL(textChanged()),this,SLOT(slotSetWindowModifiedDesc()));

    iconLabel->setPixmap(QPixmap(iconDir + "kompassberg_medium.png"));
    ratingLabel_1->setText(QString("<a href=\"enablerating_1\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));
    ratingLabel_2->setText(QString("<a href=\"enablerating_2\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));
    ratingLabel_3->setText(QString("<a href=\"enablerating_3\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));
    ratingLabel_4->setText(QString("<a href=\"enablerating_4\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));
    connect(ratingLabel_1,SIGNAL(linkActivated(const QString&)),this,SLOT(slotRatingClicked(const QString&)));
    connect(ratingLabel_2,SIGNAL(linkActivated(const QString&)),this,SLOT(slotRatingClicked(const QString&)));
    connect(ratingLabel_3,SIGNAL(linkActivated(const QString&)),this,SLOT(slotRatingClicked(const QString&)));
    connect(ratingLabel_4,SIGNAL(linkActivated(const QString&)),this,SLOT(slotRatingClicked(const QString&)));

    abortButton->setEnabled(false);
    zoomSlider->setValue(mapFrame->zoom());

    rightGroupBox->setHidden(true);
    kwLabel->setText( QString("%1. KW %2").arg(calendarWidget->selectedDate().weekNumber()).arg(calendarWidget->selectedDate().year()) );
    calendar->slotUpdateCurrentKW(calendarWidget->selectedDate());
    //calendarGroupBox->setHidden(true);

    mNextDayButton->setEnabled(false);
    mPrevDayButton->setEnabled(false);

    mapFrame->setEditMode(true);

    parser = 0;
    altitudeDiagram = 0;
    speedDiagram = 0;
    hrDiagram = 0;
    mCurrentTrack = 0;
    trackRating = 0;

    currentAdx = "";
    nextAvailAdx = "";
    previousAvailAdx = "";

    slotClearAll();
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
    if (hrDiagram) {
        delete hrDiagram;
        hrDiagram = 0;
    }


    if (parser)
        delete parser;
    
    delete legend;

    writeSettings();
    delete settings;
}

QString SportsDiary::roundNumberAsString(double x)
{
    QString val;
    val.setNum(x,'g',3);
    return val;
}

void SportsDiary::drawGraph( Waypoint* start, Waypoint* end)
{

    QVector<double> timeValues;
    QVector<double> altitudeValues;
    QVector<double> speedValues;
    QVector<double> hrValues;
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
		 if ( _physical.size() > 0 )
		 {
		    PhysicalElement* pe = _physical.atSec(start->get_time().secsTo(it->get_time()));
		    if (pe) {
			hrValues << pe->hr();
                        qDebug() << pe->hr();
		    } else
			hrValues << 0;
		 }

                 timeValues << (start->get_time().secsTo(it->get_time())) / 60;
                 tmpTime += 60;
            }
            tmpWp = it;
        }
    }

    if (!altitudeDiagram) {
        altitudeDiagram = new DiagramCurve(diagram,"Altitude");
        altitudeDiagram->setColor(QColor(0,0,255));
        slotAltitudeCheck(altitudeCheckBox->isChecked());
    }
    if (!speedDiagram) {
        speedDiagram = new DiagramCurve(diagram,"Speed");
        speedDiagram->setColor(QColor(0,255,0));
        slotSpeedCheck(speedCheckBox->isChecked());
    }
    if (!hrDiagram) {
        hrDiagram = new DiagramCurve(diagram,"Heart Rate");
        hrDiagram->setColor(QColor(255,0,0));
        slotHrCheck(hrCheckBox->isChecked());
    } 

    diagram->setAxisTitle(QwtPlot::xBottom,"Time in min");

    altitudeDiagram->setValues(timeValues,altitudeValues);
    speedDiagram->setValues(timeValues,speedValues);
    hrDiagram->setValues(timeValues,hrValues);


    diagram->replot();

    altitudeCheckBox->setEnabled(true);
    speedCheckBox->setEnabled(true);
    hrCheckBox->setEnabled(true);




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

void SportsDiary::writeSettings()
{
    settings->setValue("MapCenter",mapFrame->center());
    settings->setValue("MapZoom",mapFrame->zoom());

    settings->beginGroup("MainWindow");
    settings->setValue("size", size());
    settings->setValue("position", pos());
    settings->endGroup();
}

void SportsDiary::readSettings()
{
    mapFrame->setZoom(settings->value("MapZoom",int(16)).toInt());
    mapFrame->setCenter(settings->value("MapCenter",QPointF(-155.996910, 19.640134)).toPointF());
    if (settings->value("TilesDir").toString().isEmpty())
         settings->setValue("TilesDir",QDir::homePath() + "/ActivityDiary/tiles");
    if (settings->value("TracksDir").toString().isEmpty())
         settings->setValue("TracksDir",QDir::homePath() + "/ActivityDiary/tracks");
//  if (!settings->contains("ActivityImgMap")) {
         QMap<QString,QVariant> activities;
         activities["Cycling - Racing Bike"] = "cycling.png";
         activities["Cycling - Mountain Bike"] = "cycling.png";
         activities["Running"] = "running.png";
         activities["Hiking"] = "hiking.png";
         activities["Skiing"] = "skiing.png";
         activities["default"] = "kompassberg_small.png";

         settings->setValue("ActivityImgMap",activities);
         iconMap = activities;

//       if (!settings->contains("ActivityIconDir")) {
         if (QDir("/usr/share/activitydiary/icons/").exists()) {
            settings->setValue("ActivityIconDir",QString("/usr/share/activitydiary/icons/"));
         } else {
            QString local_icon_dir = QDir("./icons").absolutePath();
            settings->setValue("ActivityIconDir",QString(local_icon_dir + "/"));
         }
            
         iconDir = settings->value("ActivityIconDir").toString();
         qDebug() << iconDir;
//  }

    settings->beginGroup("MainWindow");
    resize(settings->value("size", QSize(800, 600)).toSize());
    move(settings->value("position", QPoint(200, 200)).toPoint());
    settings->endGroup();

}

void SportsDiary::clearTrackInfos()
{

    //sadly theres no signal from QTextBrowser to divide between manual and programmed text changes.
    disconnect(descriptionTextBrowser,SIGNAL(textChanged()),this,SLOT(slotSetWindowModifiedDesc()));

    weather->setCurrentIndex(0);
    profil->setCurrentIndex(0);
    quality->setCurrentIndex(0);
    covering->setCurrentIndex(0);
    activitytype->setCurrentIndex(0);
    trackname->setText("");
    descriptionTextBrowser->setText("");
    temperature->setText("");
    mTrackCombo->clear();
    disableRating(0);

    connect(descriptionTextBrowser,SIGNAL(textChanged()),this,SLOT(slotSetWindowModifiedDesc()));
}

void SportsDiary::closeEvent(QCloseEvent *event)
{
    if ( isWindowModified() ) {
        QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                        "Save Changes for Track \"" + trackname->text() + "\" ?",
                        QMessageBox::Yes | QMessageBox::No,this);
        switch (msgBox.exec()) {
         case QMessageBox::Yes: {
             slotSaveTrackInfos();
             event->accept();
             break; 
         }
         case QMessageBox::No:
             setWindowModified(false);
             event->accept();
             break;
         default:
             event->accept();
             break;
         }
    }
}

/*#################### SLOTS ##################################################################################*/
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

    if ( isWindowModified() ) {
        QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                "Save Changes for Track \"" + trackname->text() + "\" ?",
                QMessageBox::Yes | QMessageBox::No,this);
        switch (msgBox.exec()) {
            case QMessageBox::Yes: {
                slotSaveTrackInfos();
                break; 
            }
            case QMessageBox::No: {
                setWindowModified(false);
                break;
            }
            default:
                break;
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open GPX File"), "./", tr("GPX Files (*.gpx)"));

    if (!fileName.isEmpty()) {

        clearTrackInfos();
        QFileInfo info(fileName);
        trackname->setText(info.baseName());
        setWindowModified(true);

        slotImportTrack(fileName);
    }
}

void SportsDiary::slotImportPhysical()
{
    qDebug() << "importing physical data";

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open HRM File"), "./", tr("Phy Files (*.hrm)"));

    if (!fileName.isEmpty()) {
        slotImportPhysical(fileName);
    }
}

void SportsDiary::slotImportPhysical(QString fileName)
{
    _physical = HRMParser::read(fileName);
     
    qDebug() << _physical[2]->hr();

}


void SportsDiary::slotImportTrack(QString fileName)
{
    mapFrame->setEditMode(false);

    if ( parser ) delete parser;

    parser = new GPXParser(fileName);

    tracks = parser->getTracks();
    if (tracks.count() > 0) {

        qDebug() << "Read file with " << tracks.count() << " tracks";

        setWindowTitle(trackname->text() + "[*]" + " - ActivityDiary");

        actionSave->setEnabled(true);
        actionRemoveTrack->setEnabled(true);

        if (tracks.size() > 1) {
            if (! (!currentAdx.isEmpty() && AdxParser::hasSetting(currentAdx,"allInOneTrack"))) {
                qDebug() << "More than one track found.";
                QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                        "More than one tracks where found in the file.\nShould they be treated as one single track?",
                        QMessageBox::Yes | QMessageBox::No,this);
                switch (msgBox.exec()) {
                    case QMessageBox::Yes: {
                        tracks = parser->getAllInOneTrack();
                        break; 
                    }
                    case QMessageBox::No:
                        break;
                    default:
                        break;
                }
            } else {
                QString allInOneTrack = AdxParser::readSetting(currentAdx,"allInOneTrack");
                if (allInOneTrack == "true")
                    tracks = parser->getAllInOneTrack();
                else if (allInOneTrack == "false") {
                    int nr = AdxParser::readSetting(currentAdx,"selectedTrack").toInt();
                    TrackList list;
                    list.append(parser->getTracks()[nr]);
                    tracks = list;
                }
            }
        }

        int cnt = 0;
        mTrackCombo->clear();
        QListIterator<Track*> it( tracks );
        qDebug() << "Size TrackList: " << tracks.size();
        while ( it.hasNext() ) {
            mTrackCombo->addItem( QString( "Track No. %1" ).arg( cnt++ ) );
            it.next();
        }

        slotSelectCurrentTrack( 0 );
    } else
        qDebug() << "no track found";
}

void SportsDiary::slotSelectCurrentTrack( int num )
{
//TrackList tracks = parser->tracks();

  if ( num >= 0 && num < tracks.count() ) {
    qDebug() << "Selecting track " << num;
    mCurrentTrack = tracks[num];

    qDebug() << "Waypoints counter: " << mCurrentTrack->count_waypoints();

    mDateLabel->setText( mCurrentTrack->get_start_date().toString() );
    distanceLabel->setText(roundNumberAsString(mCurrentTrack->get_overall_distance()) + " km");
    if (mCurrentTrack->get_overall_time() > 60)
        timeLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_time() / 60) + " h");
    else
        timeLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_time()) + " min");
    speedLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_avg_speed() * 3.6 ) + " km/h");
    altitudeLabel->setText( roundNumberAsString(mCurrentTrack->get_overall_avg_altitude()) + " m");

    mapFrame->setTrack( mCurrentTrack);

    drawGraph(mCurrentTrack->first(),mCurrentTrack->last());

    nextAvailAdx = calendar->getNextActivityDay(mCurrentTrack->get_start_date());
    previousAvailAdx = calendar->getPrevActivityDay(mCurrentTrack->get_start_date());

    qDebug() << "next: " << nextAvailAdx << "prev: " << previousAvailAdx;

    mPrevDayButton->setEnabled(!previousAvailAdx.isEmpty());
    mNextDayButton->setEnabled(!nextAvailAdx.isEmpty());

    toolBox->setCurrentIndex(1);

  }
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

void SportsDiary::slotHrCheck(bool checked)
{
    enableDisableDiagram(checked,hrDiagram,"Heart Rate in 1/min");

}


void SportsDiary::slotShowCalendarWidget(bool /*check*/ )
{
       rightGroupBox->setVisible(!rightGroupBox->isVisible());
       actionShow_Calendar->setChecked(rightGroupBox->isVisible());
       if ( rightGroupBox->isVisible() )
           mCalendarButton->setToolTip("Hide Calendar Panel");
       else
           mCalendarButton->setToolTip("Show Calendar Panel");
}

void SportsDiary::slotShowTrackProperties(bool /*check*/ )
{
       leftGroupBox->setVisible(!leftGroupBox->isVisible());
       actionShow_Track_Settings->setChecked(leftGroupBox->isVisible());
       if (leftGroupBox->isVisible())
           propsButton->setToolTip("Hide Track Information Panel");
        else
           propsButton->setToolTip("Show Track Information Panel");

}

void SportsDiary::slotShowCalendar(bool /*check*/ )
{
    calendarGroupBox->setVisible(!calendarGroupBox->isVisible());
    if (calendarGroupBox->isVisible())
        calToolButton->setToolTip("Hide Day Selection");
    else
        calToolButton->setToolTip("Show Day Selection");
}

void SportsDiary::slotSetDiagramWidgetVisibility(bool /*state*/)
{
    actionShow_Diagram->setChecked(diagramDockWidget->isVisible());
}

/*
void SportsDiary::slotSetInfoWidgetVisibility(bool state)
{
    actionShow_TrackInfo->setChecked(infoDockWidget->isVisible());
}
*/

void SportsDiary::slotSaveTrackInfos()
{
    QDate startdate;
    QTime starttime;
    QString filename;

    if (!mCurrentTrack) {
        qDebug() << "No Track loaded";
    } else {
        if ( mCurrentTrack->is_custom_track() ) {
            startdate = QDate::currentDate();
            starttime = QTime::currentTime();
            filename = QString("%6/%1/%2/%1_%3_%4-%5.gpx").arg(startdate.year())
                                                            .arg(startdate.weekNumber())
                                                            .arg(startdate.toString("MM"))
                                                            .arg(startdate.toString("dd"))
                                                            .arg(starttime.toString("HHmmss"))
                                                            .arg(settings->value("TracksDir").toString());
            GPXParser::writeGPX(filename,mCurrentTrack);
        }

        if ( 6 ) {
        QMap<QString,QString> trackSettings;
        startdate = mCurrentTrack->at(0)->get_date();
        starttime = mCurrentTrack->at(0)->get_time();
        filename = QString("%6/%1/%2/%1_%3_%4-%5.adx").arg(startdate.year())
                                                            .arg(startdate.weekNumber())
                                                            .arg(startdate.toString("MM"))
                                                            .arg(startdate.toString("dd"))
                                                            .arg(starttime.toString("HHmmss"))
                                                            .arg(settings->value("TracksDir").toString());
        QFileInfo info(filename);                                                            

        trackSettings["trackfile"] = parser->getFileName();
        if (! trackname->text().isEmpty() )
            trackSettings["trackname"] = trackname->text();
        if (! activitytype->currentIndex() == 0) 
            trackSettings["activitytype"] = activitytype->currentText();
        trackSettings["totaltime"] = roundNumberAsString(mCurrentTrack->get_overall_time());
        trackSettings["distance"] = roundNumberAsString(mCurrentTrack->get_overall_distance());
        trackSettings["startdate"] = mCurrentTrack->get_start_date().toString();
        if (! weather->currentText().isEmpty() )
            trackSettings["weather"] = weather->currentText();
        if (! profil->currentText().isEmpty() )
            trackSettings["profil"] = profil->currentText();
        if (! quality->currentText().isEmpty() )
            trackSettings["quality"] = quality->currentText();
        if (! covering->currentText().isEmpty() )
            trackSettings["covering"] = covering->currentText();
        if (! descriptionTextBrowser->toPlainText().isEmpty() )
            trackSettings["description"] = descriptionTextBrowser->toPlainText();
        if (! temperature->text().isEmpty() )
            trackSettings["temperature"] = temperature->text();
        if (trackRating > 0 && trackRating < 5)
            trackSettings["rating"] = QString::number(trackRating);
        
        if (info.exists()) {
            trackSettings["selectedTrack"] = AdxParser::readSetting(filename,"selectedTrack");
            trackSettings["allInOneTrack"] = AdxParser::readSetting(filename,"allInOneTrack");
        } else { 

            if (parser->getTracks().size() > 1 && mTrackCombo->count() == 1)
                trackSettings["allInOneTrack"] = "true";
            else if (parser->getTracks().size() > 1 && mTrackCombo->count() > 1 )
                trackSettings["allInOneTrack"] = "false";
           
            if (! mTrackCombo->currentText().isEmpty() )
                trackSettings["selectedTrack"] = mTrackCombo->currentText().split("No. ")[1];
        }

        AdxParser::writeSettings(filename, trackSettings);

        currentAdx = filename;

        if (calendarWidget->selectedDate().weekNumber() == QDate::fromString(mDateLabel->text()).weekNumber())
            calendar->slotUpdateCurrentKW(QDate::fromString(mDateLabel->text()));

    }
        setWindowModified(false);
    }
}

void SportsDiary::slotRemoveTrack()
{
    if (mCurrentTrack) {
        QDate startdate = mCurrentTrack->at(0)->get_date();
        QTime starttime = mCurrentTrack->at(0)->get_time();
        QString filename = QString("%6/%1/%2/%1_%3_%4-%5.adx").arg(startdate.year())
                                                            .arg(startdate.weekNumber())
                                                            .arg(startdate.toString("MM"))
                                                            .arg(startdate.toString("dd"))
                                                            .arg(starttime.toString("HHmmss"))
                                                            .arg(settings->value("TracksDir").toString());
        if ( currentAdx == filename && QFile::exists(filename) ) {
            QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                    "Really remove Track \"" + trackname->text() + "\" from ActivityDiary ?",
                    QMessageBox::Yes | QMessageBox::No,this);
            switch (msgBox.exec()) {
                case QMessageBox::Yes: {
                    setWindowModified(false);
                    QFile adxfile(currentAdx);
                    QFile gpxfile(AdxParser::readSetting(currentAdx,"trackfile"));
                    adxfile.remove();
                    gpxfile.remove();
                    if (calendarWidget->selectedDate().weekNumber() == QDate::fromString(mDateLabel->text()).weekNumber())
                        calendar->slotUpdateCurrentKW(QDate::fromString(mDateLabel->text()));
                    slotClearAll();
                    break; 
                }
                case QMessageBox::No: {
                    break;
                }
                default:
                    break;
            }
        } else {
            setWindowModified(false);
            slotClearAll();
        }
    }
}

void SportsDiary::slotClearAll()
{
    if ( isWindowModified() ) {
        QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                "Save Changes for Track \"" + trackname->text() + "\" ?",
                QMessageBox::Yes | QMessageBox::No,this);
        switch (msgBox.exec()) {
            case QMessageBox::Yes: {
                slotSaveTrackInfos();
                break; 
            }
            case QMessageBox::No: {
                setWindowModified(false);
                break;
            }
            default:
                break;
        }
    }

    clearTrackInfos();
    mapFrame->clearMap();
    mDateLabel->setText(QDate::currentDate().toString());

    diagram->clear();

    if (altitudeDiagram) {
        altitudeDiagram->detach();
        delete altitudeDiagram;
        altitudeDiagram = 0;
    }
    if (speedDiagram) {
        speedDiagram->detach();
        delete speedDiagram;
        speedDiagram = 0;
    }
    if (hrDiagram) {
        hrDiagram->detach();
        delete hrDiagram;
        hrDiagram = 0;
    }



    cadenceLabel->setText("");
    heartrateLabel->setText("");
    speedLabel->setText("");
    altitudeLabel->setText("");
    timeLabel->setText("");
    distanceLabel->setText("");

    altitudeCheckBox->setChecked(true);
    speedCheckBox->setChecked(true);
    hrCheckBox->setChecked(true);
    altitudeCheckBox->setEnabled(false);
    speedCheckBox->setEnabled(false);
    hrCheckBox->setEnabled(false);

    actionSave->setEnabled(false);
    actionRemoveTrack->setEnabled(false);

    mCurrentTrack = 0;
    if (parser) {
        delete parser;
        parser = 0;
    }

    currentAdx = "";

    nextAvailAdx = calendar->getNextActivityDay(QDate::currentDate());
    previousAvailAdx = calendar->getPrevActivityDay(QDate::currentDate());
    mPrevDayButton->setEnabled(!previousAvailAdx.isEmpty());
    mNextDayButton->setEnabled(!nextAvailAdx.isEmpty());

}

void SportsDiary::slotCalendarUpdated(const QDate& date)
{

    kwLabel->setText( QString("%1. KW %2").arg(date.weekNumber()).arg(date.year()) );
    QMap<QString,double> summary = calendar->getWeekSummary(date);
    totalWeekDistanceLabel->setText(QString::number(summary["distance"]) + " km");

    if (summary["totaltime"] > 60) 
        totalWeekTimeLabel->setText(roundNumberAsString(summary["totaltime"]/60) + " h");
    else
        totalWeekTimeLabel->setText(roundNumberAsString(summary["totaltime"]) + " min");
}

void SportsDiary::slotLoadSavedTrack(const QString& filenameAdx)
{
    if (!filenameAdx.isEmpty()) {
        if ( isWindowModified() ) {
            QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                    "Save Changes for Track \"" + trackname->text() + "\" ?",
                    QMessageBox::Yes | QMessageBox::No,this);
            switch (msgBox.exec()) {
                case QMessageBox::Yes: {
                    slotSaveTrackInfos();
                    break; 
                    }
                case QMessageBox::No:
                    setWindowModified(false);
                    break;
                default:
                                       break;
            }
        }
        clearTrackInfos();

        currentAdx = filenameAdx;

        qDebug() << "Loading Adx: " << filenameAdx;
        QString filenameGpx = AdxParser::readSetting(filenameAdx,"trackfile");
        qDebug() << "Loading GPX" << filenameGpx;

        QMap<QString,QString> setts = AdxParser::readSettings(filenameAdx);

        if (! setts["trackname"].isEmpty() )
            trackname->setText( setts["trackname"]);
        if (! setts["weather"].isEmpty() )
            weather->setCurrentIndex( weather->findText(setts["weather"]));
        if (! setts["profil"].isEmpty() )
            profil->setCurrentIndex( profil->findText(setts["profil"]));
        if (! setts["quality"].isEmpty() )
            quality->setCurrentIndex( quality->findText(setts["quality"]));
        if (! setts["covering"].isEmpty() )
            covering->setCurrentIndex( covering->findText(setts["covering"]));
        if (! setts["activitytype"].isEmpty() )
            activitytype->setCurrentIndex( activitytype->findText(setts["activitytype"]));
        if (! setts["description"].isEmpty() )
            descriptionTextBrowser->setText( setts["description"]);
        if (! setts["temperature"].isEmpty() )
            temperature->setText( setts["temperature"]);
        if (! setts["rating"].isEmpty() )
            enableRating(setts["rating"].toInt());

        slotImportTrack(filenameGpx);

    }

}

void SportsDiary::slotSetWindowModified(const QString & /* val */)
{
    if (mCurrentTrack) {
        setWindowModified(true);
        actionSave->setEnabled(true);
        actionRemoveTrack->setEnabled(true);
    }

}

void SportsDiary::slotSetWindowModifiedDesc()
{
    if (! currentAdx.isEmpty()) {
        QString oldDesc = AdxParser::readSetting(currentAdx,"description");
        if (oldDesc != descriptionTextBrowser->toPlainText()) {
            qDebug() << "Description changed!";
            setWindowModified(true);
        }
    }
}

void SportsDiary::slotSetWindowTitle(const QString & trackname)
{
    if (trackname.isEmpty())
        setWindowTitle("ActivityDiary[*]");
    else
        setWindowTitle(trackname + "[*]" + " - ActivityDiary");
}

void SportsDiary::slotLoadNextActivityDay()
{

    qDebug() << "nextAvailAdx " << nextAvailAdx;
    slotLoadSavedTrack(nextAvailAdx);
}

void SportsDiary::slotLoadPrevActivityDay()
{
    qDebug() << "prevAvailAdx " << previousAvailAdx;
    slotLoadSavedTrack(previousAvailAdx);
}

void SportsDiary::slotRatingClicked(const QString& link)
{
    int star = link.split('_')[1].toInt();
    QString action = link.split('_')[0];
    if (action == "enablerating")
        enableRating(star);
    else
        disableRating(star);
}

void SportsDiary::slotCustomTrackChanged(Track* trk)
{
    toolBox->setCurrentIndex(1);
    distanceLabel->setText(roundNumberAsString(trk->get_overall_distance()) + " km");
    mCurrentTrack = trk;
    slotSetWindowModified("");
}

void SportsDiary::slotStartWizard()
{
    qDebug() << "New Activity Wizard called";

    adWizard = new ADWizard(this);
    connect(adWizard,SIGNAL(wizardFinished(QString,QString,QString)),this,SLOT(slotWizardFinished(QString,QString,QString)));
    connect(adWizard,SIGNAL(wizardCancelled()),this,SLOT(slotWizardCancelled()));

    if ( isWindowModified() ) {
        QMessageBox msgBox(QMessageBox::Question,"ActivityDiary",
                "Save Changes for Track \"" + trackname->text() + "\" ?",
                QMessageBox::Yes | QMessageBox::No,this);
        switch (msgBox.exec()) {
            case QMessageBox::Yes: {
                slotSaveTrackInfos();
                break;
            }
            case QMessageBox::No: {
                setWindowModified(false);
                break;
            }
            default:
                break;
        }
    }

    adWizard->show();
}

void SportsDiary::slotWizardFinished(QString name,QString trackSource, QString physical)
{
    qDebug() << "Wizard finished";

    clearTrackInfos();
    trackname->setText(name);
    setWindowModified(true);

    if (trackSource == "gps") {

        slotImportTrack(adWizard->getGPXFileName());

    } else if (trackSource == "manual") {

        mapFrame->setEditMode(true);
        QDateTime start = adWizard->getManualStartDateTime();
        QDateTime end = adWizard->getManualEndDateTime();
    }

    adWizard->deleteLater();
    adWizard = 0;
}


void SportsDiary::slotWizardCancelled()
{
    qDebug() << "Wizard cancelled";
    adWizard->deleteLater();
    adWizard = 0;
}


void SportsDiary::enableRating(int star)
{
    switch (star) {
            case 1:
                ratingLabel_1->setText(QString("<a href=\"disablerating_1\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                break;
            case 2:
                ratingLabel_1->setText(QString("<a href=\"disablerating_1\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                ratingLabel_2->setText(QString("<a href=\"disablerating_2\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                break;
            case 3:
                ratingLabel_1->setText(QString("<a href=\"disablerating_1\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                ratingLabel_2->setText(QString("<a href=\"disablerating_2\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                ratingLabel_3->setText(QString("<a href=\"disablerating_3\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                break;
            case 4:
                ratingLabel_1->setText(QString("<a href=\"disablerating_1\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                ratingLabel_2->setText(QString("<a href=\"disablerating_2\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                ratingLabel_3->setText(QString("<a href=\"disablerating_3\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                ratingLabel_4->setText(QString("<a href=\"disablerating_4\"><img src=\"" + iconDir + "star_small_yellow.png" + "\"></a>"));;
                break;
    }
    if (star > 0 && star < 5) {
        trackRating = star;
        slotSetWindowModified("");
    }
}

void SportsDiary::disableRating(int /*star*/)
{
    ratingLabel_1->setText(QString("<a href=\"enablerating_1\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));;
    ratingLabel_2->setText(QString("<a href=\"enablerating_2\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));;
    ratingLabel_3->setText(QString("<a href=\"enablerating_3\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));;
    ratingLabel_4->setText(QString("<a href=\"enablerating_4\"><img src=\"" + iconDir + "star_small_gray.png" + "\"></a>"));;
}

