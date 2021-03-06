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

#include "sportsdiary.h"
#include "Physical.h"

#include <QComboBox>

SportsDiary::SportsDiary(QObject* parent)
  :parser( 0 )
{
    Q_UNUSED(parent);

    setupUi(this);

    settings = new QSettings();
    readSettings();
    qDebug() << iconDir;
    activitytype->addItem(QIcon(iconDir + iconMap["default"].toString()), tr("Select your Activity..."));
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
    connect(actionNewActivity,SIGNAL(triggered()),this,SLOT(slotStartWizard()));
    connect(zoomSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSetZoom(int)));
    connect(leftDiagramCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotLeftDiagramChanged(const QString&)));
    connect(rightDiagramCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotRightDiagramChanged(const QString&)));
    connect(mCalendarButton,SIGNAL(clicked(bool)),this,SLOT(slotShowCalendarWidget(bool)));
    connect(calToolButton,SIGNAL(clicked(bool)),this,SLOT(slotShowCalendar(bool)));
    connect(diagramDockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(slotSetDiagramWidgetVisibility(bool)));
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
    kwLabel->setText( QString("%1. ").arg(calendarWidget->selectedDate().weekNumber()) + tr("CW") + QString(" %1").arg(calendarWidget->selectedDate().year()) );
    calendar->slotUpdateCurrentKW(calendarWidget->selectedDate());
    //calendarGroupBox->setHidden(true);

    mNextDayButton->setEnabled(false);
    mPrevDayButton->setEnabled(false);

    mapFrame->setEditMode(false);

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

                 speedValues << tmpWp->get_speed_to_wp(it) * 3.6;
		 if ( _physical.size() > 0 )
		 {
		    PhysicalElement* pe = _physical.atSec(start->get_time().secsTo(it->get_time()));
		    if (pe) {
			hrValues << pe->hr();
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
        altitudeDiagram = new DiagramCurve(diagram,tr("Altitude"));
        altitudeDiagram->setColor(QColor(0,0,255));
        if (altitudeValues.size() > 0) 
            leftDiagramCombo->setCurrentIndex(leftDiagramCombo->findText(tr("Altitude")));
    }

    if (!speedDiagram) {
        speedDiagram = new DiagramCurve(diagram,tr("Speed"));
        speedDiagram->setColor(QColor(0,255,0));
        //if (speedValues.size() > 0)
            //rightDiagramCombo->setCurrentIndex(rightDiagramCombo->findText("Speed"));
    }

    if (!hrDiagram) {
        hrDiagram = new DiagramCurve(diagram,tr("Heart Rate"));
        hrDiagram->setColor(QColor(255,0,0));
    } 

    diagram->setAxisTitle(QwtPlot::xBottom,tr("Time in min"));

    altitudeDiagram->setValues(timeValues,altitudeValues);
    speedDiagram->setValues(timeValues,speedValues);
    hrDiagram->setValues(timeValues,hrValues);

    diagram->replot();
    
    if (timeValues.size() > 0 && ( altitudeValues.size() > 0 || speedValues.size() > 0 || hrValues.size() > 0) ) {
        leftDiagramCombo->setEnabled(true);
        rightDiagramCombo->setEnabled(true);
    } else {
        leftDiagramCombo->setEnabled(false);
        rightDiagramCombo->setEnabled(false);
    }

}

void SportsDiary::enableDisableDiagram(bool check, DiagramCurve* curve, QString axText, int axisId)
{
    if (curve) {
        if (check) {
            curve->detach();
            qDebug() << "enable " << axText << " Diagram";

            curve->setAxis(QwtPlot::xBottom, axisId);
            diagram->setAxisTitle(axisId,axText);
            curve->attachToDiagram(diagram);

        } else {
            qDebug() << "disable " << axText << " Diagram";

            curve->detach();
            diagram->setAxisTitle(axisId,"");
        }
        diagram->replot();
    }
}

void SportsDiary::setActualDiagramContent(DiagramCurve* curveLeft, DiagramCurve* curveRight,QString textLeft, QString textRight)
{
    //diagram->clear();
    if (speedDiagram)
         speedDiagram->detach();
    if (altitudeDiagram)
         altitudeDiagram->detach();
    if (hrDiagram)
         hrDiagram->detach();

    if (curveLeft)
        enableDisableDiagram(true,curveLeft,textLeft,QwtPlot::yLeft);

    if (curveRight) 
        enableDisableDiagram(true,curveRight,textRight,QwtPlot::yRight);

}

DiagramCurve* SportsDiary::getDiagramFromText(QString text)
{
    if (text == "Speed")
        return speedDiagram;
    else if (text == "Altitude")
        return altitudeDiagram;
    else if (text == "Heartrate")
        return hrDiagram;
    else
        return 0;
}

QString SportsDiary::getDiagramTextFromName(QString text)
{
    if (text == "Speed")
        return "Speed in km/h";
    else if (text == "Altitude")
        return "Altitude in m";
    else if (text == "Heartrate")
        return "Heartrate in 1/min";
    else
        return "";
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
         activities[tr("Cycling - Racing Bike")] = "cycling.png";
         activities[tr("Cycling - Mountain Bike")] = "cycling.png";
         activities[tr("Running")] = "running.png";
         activities[tr("Hiking")] = "hiking.png";
         activities[tr("Skiing")] = "skiing.png";
         activities[tr("default")] = "kompassberg_small.png";

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
                        tr("Save Changes for Track \"") + trackname->text() + "\" ?",
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
                tr("Save Changes for Track \"") + trackname->text() + "\" ?",
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
    qDebug() << "HR Count " << _physical.size();

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

    startLabel->setText(mCurrentTrack->get_start_time().toString());
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

    nextAvailAdx = calendar->getNextActivityDay(mCurrentTrack->get_start_date(), mCurrentTrack->get_start_time());
    previousAvailAdx = calendar->getPrevActivityDay(mCurrentTrack->get_start_date(), mCurrentTrack->get_start_time());

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
    startLabel->setText(mCurrentTrack->get_start_time().toString());
}

void SportsDiary::slotLeftDiagramChanged(const QString& text)
{
    disconnect(rightDiagramCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotRightDiagramChanged(const QString&)));
    QString currentRightComboText = rightDiagramCombo->currentText();
    rightDiagramCombo->clear();
    QStringList items;

    qDebug() << "slotLeftDiagramChanged  Text: " << text << " currentRightComboText: " << currentRightComboText;

    if (text == "Speed") {
        diagram->enableAxis(QwtPlot::yLeft,true);
        items << "-" << "Altitude" << "Heartrate";
    } else if (text == "Altitude") {
        diagram->enableAxis(QwtPlot::yLeft,true);
        items << "-" << "Speed" << "Heartrate";
    } else if (text == "Heartrate") {
        diagram->enableAxis(QwtPlot::yLeft,true);
        items << "-" << "Speed" << "Altitude";
    } else {
        diagram->enableAxis(QwtPlot::yLeft,false);
        items << "-" << "Speed" << "Altitude" << "Heartrate";

    }
    rightDiagramCombo->addItems(items);
    rightDiagramCombo->setCurrentIndex(rightDiagramCombo->findText(currentRightComboText));
    
    connect(rightDiagramCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotRightDiagramChanged(const QString&)));
    setActualDiagramContent(getDiagramFromText(text),getDiagramFromText(currentRightComboText),getDiagramTextFromName(text),getDiagramTextFromName(currentRightComboText));
}

void SportsDiary::slotRightDiagramChanged(const QString& text)
{
    disconnect(leftDiagramCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotLeftDiagramChanged(const QString&)));
    QString currentLeftComboText = leftDiagramCombo->currentText();
    leftDiagramCombo->clear();
    QStringList items;

    qDebug() << "slotRightDiagramChanged  Text: " << text << " currentLeftComboText: " << currentLeftComboText;
    if (text == "Speed") {
        diagram->enableAxis(QwtPlot::yRight,true);
        items << "-" << "Altitude" << "Heartrate";
    } else if (text == "Altitude") {
        diagram->enableAxis(QwtPlot::yRight,true);
        items << "-" << "Speed" << "Heartrate";
    } else if (text == "Heartrate") {
        diagram->enableAxis(QwtPlot::yRight,true);
        items << "-" << "Speed" << "Altitude";
    } else {
        diagram->enableAxis(QwtPlot::yRight,false);
        items << "-" << "Speed" << "Altitude" << "Heartrate";
    }
    leftDiagramCombo->addItems(items);
    leftDiagramCombo->setCurrentIndex(leftDiagramCombo->findText(currentLeftComboText));

    connect(leftDiagramCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotLeftDiagramChanged(const QString&)));
    setActualDiagramContent(getDiagramFromText(currentLeftComboText), getDiagramFromText(text),getDiagramTextFromName(currentLeftComboText), getDiagramTextFromName(text));

}

void SportsDiary::slotShowCalendarWidget(bool /*check*/ )
{
       rightGroupBox->setVisible(!rightGroupBox->isVisible());
       actionShow_Calendar->setChecked(rightGroupBox->isVisible());
       if ( rightGroupBox->isVisible() )
           mCalendarButton->setToolTip(tr("Hide Calendar Panel"));
       else
           mCalendarButton->setToolTip(tr("Show Calendar Panel"));
}

void SportsDiary::slotShowTrackProperties(bool /*check*/ )
{
       leftGroupBox->setVisible(!leftGroupBox->isVisible());
       actionShow_Track_Settings->setChecked(leftGroupBox->isVisible());
       if (leftGroupBox->isVisible())
           propsButton->setToolTip(tr("Hide Track Information Panel"));
        else
           propsButton->setToolTip(tr("Show Track Information Panel"));

}

void SportsDiary::slotShowCalendar(bool /*check*/ )
{
    calendarGroupBox->setVisible(!calendarGroupBox->isVisible());
    if (calendarGroupBox->isVisible())
        calToolButton->setToolTip(tr("Hide Day Selection"));
    else
        calToolButton->setToolTip(tr("Show Day Selection"));
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
            startdate = manualStart.date();
            starttime = manualStart.time();

        } else {
            startdate = mCurrentTrack->at(0)->get_date();
            starttime = mCurrentTrack->at(0)->get_time();
        }

        QMap<QString,QString> trackSettings;
        filename = QString("%6/%1/%2/%1_%3_%4-%5.adx").arg(startdate.year())
                                                            .arg(startdate.weekNumber())
                                                            .arg(startdate.toString("MM"))
                                                            .arg(startdate.toString("dd"))
                                                            .arg(starttime.toString("HHmmss"))
                                                            .arg(settings->value("TracksDir").toString());
        QFileInfo info(filename);                                                            

        if (! trackname->text().isEmpty() )
            trackSettings["trackname"] = trackname->text();
        if (! activitytype->currentIndex() == 0) 
            trackSettings["activitytype"] = activitytype->currentText();
        trackSettings["totaltime"] = roundNumberAsString(mCurrentTrack->get_overall_time());
        trackSettings["distance"] = roundNumberAsString(mCurrentTrack->get_overall_distance());
        trackSettings["startdate"] = mCurrentTrack->get_start_date().toString();
        trackSettings["starttime"] = mCurrentTrack->get_start_time().toString();
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

            if (! mTrackCombo->currentText().isEmpty() )
                trackSettings["selectedTrack"] = mTrackCombo->currentText().split("No. ")[1];
        }

        if (!HRM_File.isEmpty())
            trackSettings["heartratefile"] = HRM_File;


        if (mCurrentTrack->is_custom_track() ) {

            QString filenameGPX = filename;
            filenameGPX.chop(3);
            filenameGPX += "gpx";

            trackSettings["startdate"] = manualStart.date().toString();
            trackSettings["starttime"] = manualStart.time().toString();
            trackSettings["trackfile"] = filename;
            trackSettings["totaltime"] = roundNumberAsString(manualStart.secsTo(manualEnd) / 60);
            GPXParser::writeGPX(filenameGPX,mCurrentTrack,manualStart,manualEnd);
        } else {

            trackSettings["trackfile"] = parser->getFileName();
            if (!info.exists()) {
                if (parser->getTracks().size() > 1 && mTrackCombo->count() == 1)
                    trackSettings["allInOneTrack"] = "true";
                else if (parser->getTracks().size() > 1 && mTrackCombo->count() > 1 )
                    trackSettings["allInOneTrack"] = "false";
            }
        }

        AdxParser::writeSettings(filename, trackSettings);

        currentAdx = filename;

        if (calendarWidget->selectedDate().weekNumber() == QDate::fromString(mDateLabel->text()).weekNumber())
            calendar->slotUpdateCurrentKW(QDate::fromString(mDateLabel->text()));

    }
        setWindowModified(false);

        mapFrame->setEditMode(false);
        manualStart = QDateTime();
        manualEnd = QDateTime();
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
                    tr("Really remove Track \"") + trackname->text() + tr("\" from ActivityDiary ?"),
                    QMessageBox::Yes | QMessageBox::No,this);
            switch (msgBox.exec()) {
                case QMessageBox::Yes: {
                    setWindowModified(false);
                    QFile adxfile(currentAdx);
                    QFile gpxfile(AdxParser::readSetting(currentAdx,"trackfile"));
                    QFile hrm(AdxParser::readSetting(currentAdx,"heartratefile"));

                    QFileInfo info(adxfile);
                    QStringList dirnames = info.absolutePath().split('/');
                    QString dirname = dirnames[dirnames.size() -1];
                    QDir adxdir(info.absolutePath());

                    adxfile.remove();
                    gpxfile.remove();
                    hrm.remove();

                    if (! adxdir.entryList(QStringList("*"),QDir::NoDotAndDotDot).size() > 0) {
                        adxdir.cdUp();
                        adxdir.rmdir(dirname);
                    }

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
                tr("Save Changes for Track \"") + trackname->text() + "\" ?",
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

    diagram->replot();

    heartrateLabel->setText("");
    speedLabel->setText("");
    altitudeLabel->setText("");
    timeLabel->setText("");
    distanceLabel->setText("");
    startLabel->setText("");

    leftDiagramCombo->setEnabled(false);
    rightDiagramCombo->setEnabled(false);

    actionSave->setEnabled(false);
    actionRemoveTrack->setEnabled(false);

    mCurrentTrack = 0;
    if (parser) {
        delete parser;
        parser = 0;
    }

    currentAdx = "";
    HRM_File = "";

    nextAvailAdx = calendar->getNextActivityDay(QDate::currentDate(), QTime::currentTime());
    previousAvailAdx = calendar->getPrevActivityDay(QDate::currentDate(),QTime::currentTime());
    mPrevDayButton->setEnabled(!previousAvailAdx.isEmpty());
    mNextDayButton->setEnabled(!nextAvailAdx.isEmpty());

}

void SportsDiary::slotCalendarUpdated(const QDate& date)
{

    kwLabel->setText( QString("%1. ").arg(date.weekNumber()) + tr("CW") + QString(" %1").arg(date.year()) );
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
                    tr("Save Changes for Track \"") + trackname->text() + "\" ?",
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
        QString filenameHRM = AdxParser::readSetting(filenameAdx,"heartratefile");
        qDebug() << "Loading HRM" << filenameHRM;

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

        if (!filenameHRM.isEmpty()) {
            slotImportPhysical(filenameHRM);
        }

        HRM_File = filenameHRM;

        slotImportTrack(filenameGpx);
        setWindowModified(false);
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
                tr("Save Changes for Track \"") + trackname->text() + "\" ?",
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
    qDebug() << "Physical " << physical;

    slotClearAll();

    trackname->setText(name);
    setWindowModified(true);

    if (!physical.isEmpty() && QFile::exists(physical)) {
        slotImportPhysical(physical);
        HRM_File = physical;
    }

    if (trackSource == "gps") {

        slotImportTrack(adWizard->getGPXFileName());

        slotSaveTrackInfos();
    } else if (trackSource == "manual") {

        mapFrame->setEditMode(true);

        manualStart = adWizard->getManualStartDateTime();
        manualEnd = adWizard->getManualEndDateTime();

        mDateLabel->setText(manualStart.date().toString() );

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

