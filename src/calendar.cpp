#include "calendar.h"

Calendar::Calendar(QWidget* parent) : QTextBrowser(parent)
{
    settings = new QSettings(); 


    setOpenLinks(false);
    setFrameShape(QFrame::NoFrame);

    connect(this,SIGNAL(anchorClicked(const QUrl&)),this,SLOT(slotTrackSelected(const QUrl&)));

}

Calendar::~Calendar()
{
    delete settings;

}

void Calendar::slotTrackSelected(const QUrl& url)
{
    emit trackSelected(url.toString());
}

void Calendar::slotUpdateCurrentKW(const QDate& date)
{

    QString path = settings->value("TracksDir").toString() + "/" + QString::number(date.year()) + "/" + QString::number(date.weekNumber());
    QDir kwDir(path);
    QMap<int,QString> weekDaysHtml;
    QString html;



    for(int i= 1;i <= 7 ; i++) {
        QDate myDate = date.addDays( -date.dayOfWeek() + i );

        html += ("<td bgcolor=#D0D0D0 align=center valign=middle width=\"10%\">" + myDate.longDayName(myDate.dayOfWeek()) + "<br>");
        html += (myDate.toString("dd.MM.") + "</td>");

        weekDaysHtml[i] = html;
        html = "";
    }

    if (kwDir.entryList(QStringList("*.adx")).size() > 0 ) {
        QMap<QString,QVariant> iconMap = settings->value("ActivityImgMap").toMap();

        foreach(QString filename,kwDir.entryList(QStringList("*.adx"))) {

            html = "";
            QString activityIcon = "";
            QDate adxDate = QDate::fromString(AdxParser::readSetting(path + "/" + filename,"startdate"));
            QString activityType = AdxParser::readSetting(path + "/" + filename,"activitytype");

            if (iconMap.contains(activityType))
                activityIcon = iconMap[activityType].toString();
            else
                activityIcon = iconMap["default"].toString();

            QString activityTime = AdxParser::readSetting(path + "/" + filename,"totaltime");
            QString activityDistance = AdxParser::readSetting(path + "/" + filename,"distance");
       
            html += "<td bgcolor=#FFFFFF align=center valign=middle ><a href= \"" + path + "/" + filename + "\"><img src=\"icons/" + activityIcon + "\"><br>";
            html += activityType + "<br>";
            html += activityDistance + " km<br>";

            if (activityTime.toDouble() > 60)
                html += roundNumberAsString(activityTime.toDouble() / 60) + " h</a></td>";
            else
                html += activityTime + " min</a></td>";

            weekDaysHtml[ adxDate.dayOfWeek() ] += html;

        }
        html = "";
    }
    else
    {
        // add dummy entry to ensure proper table format even if no items are shown
        weekDaysHtml[1] += "<td width=\"100%\">&nbsp;</td>";    
    }

    QString css = 
        "a:link { text-decoration:none; color:black; } "
        "a:visited { text-decoration:none; color:black; } "
        "a:hover { text-decoration:none; color:black }"
        "a:active { text-decoration:none; color:black } "
        "a:focus { text-decoration:none; color:black; } "
        ;

    document()->addResource( QTextDocument::StyleSheetResource, QUrl( "format.css" ), css );

        
    html += "<html><head>";
    html += "<link rel='stylesheet' type='text/css' href='format.css'>";
    html += "</head><body>";

    html += "<table align=\"center\" width=\"100%\">"; 
    for(int day = 1; day <= 7; day++) {
        html += "<tr>";
        html += weekDaysHtml[day];
        html += "</tr>";
    }

    html += "</table>";
    html += "</body></html>";
        
    setHtml(html);

    emit calendarUpdated(date);

}

QMap<QString,double> Calendar::getWeekSummary(const QDate& date)
{
    double distance = 0;
    double totaltime = 0;
    QMap<QString,double> summary;

    QString path = settings->value("TracksDir").toString() + "/" + QString::number(date.year()) + "/" + QString::number(date.weekNumber());
    QDir kwDir(path);

    if (kwDir.entryList(QStringList("*.adx")).size() > 0 ) {

        foreach(QString filename,kwDir.entryList(QStringList("*.adx"))) {
            QString activityDistance = AdxParser::readSetting(path + "/" + filename,"distance");
            QString activityTime = AdxParser::readSetting(path + "/" + filename,"totaltime");
            distance += activityDistance.toDouble();
            totaltime += activityTime.toDouble();
        }
    }
    summary["distance"] = distance;
    summary["totaltime"] = totaltime;

    return summary;
}

QString Calendar::getNextActivityDay(const QDate& date)
{
    QString path = settings->value("TracksDir").toString() + "/" + QString::number(date.year()) + "/" + QString::number(date.weekNumber());
    QString yearPath = settings->value("TracksDir").toString() + "/" + QString::number(date.year());
    QString basePath = settings->value("TracksDir").toString();
    QDir kwDir(path);
    QDir yearDir(yearPath);
    QDir baseDir(basePath);

    //next track is in the same calendar week number
    if (kwDir.entryList(QStringList("*.adx")).size() > 0 ) {
        foreach(QString filename,kwDir.entryList(QStringList("*.adx"))) {
            QDate adxDate = QDate::fromString(AdxParser::readSetting(path + "/" + filename,"startdate"));
            if (adxDate > date)
                return path + "/" + filename;
        }
    }
    //next track is in a higher calendar week number
    if (yearDir.entryList(QStringList("*"),QDir::Dirs).size() > 0 ) {
        foreach(QString dirname,yearDir.entryList(QStringList("*"),QDir::Dirs,QDir::Name)) {
            if (dirname.toInt() > date.weekNumber()) {
                QDir tmpDir(yearDir.path() + "/" + dirname);
                if (tmpDir.entryList(QStringList("*.adx")).size() > 0 ) 
                    return tmpDir.path() + "/" + tmpDir.entryList(QStringList("*.adx"),QDir::Files,QDir::Name)[0];
            }
        }
    }
    //next track is in a higher calendar year
    if (baseDir.entryList(QStringList("*"),QDir::Dirs).size() > 0 ) {
        foreach(QString dirname,baseDir.entryList(QStringList("*"),QDir::Dirs,QDir::Name)) {
            if (dirname.toInt() > date.year()) {
                QDir tmpDir(baseDir.path() + "/" + dirname);
                if (tmpDir.entryList(QStringList("*"),QDir::Dirs).size() > 0 ) {
                    QDir tmpKwDir(tmpDir.path() + "/" + tmpDir.entryList(QStringList("*"),QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)[0]);
                    if (tmpKwDir.entryList(QStringList("*.adx")).size() > 0 )
                        return tmpKwDir.path() + "/" + tmpKwDir.entryList(QStringList("*.adx"),QDir::Files,QDir::Name)[0];
                }
            }
        }
    }
    // no next Track found.
    return "";

}

QString Calendar::getPrevActivityDay(const QDate& date)
{
    QString path = settings->value("TracksDir").toString() + "/" + QString::number(date.year()) + "/" + QString::number(date.weekNumber());
    QString yearPath = settings->value("TracksDir").toString() + "/" + QString::number(date.year());
    QString basePath = settings->value("TracksDir").toString();
    QDir kwDir(path);
    QDir yearDir(yearPath);
    QDir baseDir(basePath);

    //prevoius track is in the same calendar week number
    if (kwDir.entryList(QStringList("*.adx")).size() > 0 ) {
        foreach(QString filename,kwDir.entryList(QStringList("*.adx"))) {
            QDate adxDate = QDate::fromString(AdxParser::readSetting(path + "/" + filename,"startdate"));
            if (adxDate < date)
                return path + "/" + filename;
        }
    }
    //prevoius track is in a lower calendar week number
    if (yearDir.entryList(QStringList("*"),QDir::Dirs).size() > 0 ) {
        foreach(QString dirname,yearDir.entryList(QStringList("*"),QDir::Dirs,QDir::Name | QDir::Reversed)) {
            if (dirname.toInt() < date.weekNumber()) {
                QDir tmpDir(yearDir.path() + "/" + dirname);
                if (tmpDir.entryList(QStringList("*.adx")).size() > 0 )
                    return tmpDir.path() + "/" + tmpDir.entryList(QStringList("*.adx"),QDir::Files,QDir::Name | QDir::Reversed)[0];
                
            }
        }
    }
    // prevoius track is in a lower calendar year
    if (baseDir.entryList(QStringList("*"),QDir::Dirs).size() > 0 ) {
        foreach(QString dirname,baseDir.entryList(QStringList("*"),QDir::Dirs,QDir::Name | QDir::Reversed)) {
            if (dirname.toInt() < date.year()) {
                QDir tmpDir(baseDir.path() + "/" + dirname);
                if (tmpDir.entryList(QStringList("*"),QDir::Dirs).size() > 0 ) {
                    QDir tmpKwDir(tmpDir.path() + "/" + tmpDir.entryList(QStringList("*"),QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::Reversed)[0]);
                    if (tmpKwDir.entryList(QStringList("*.adx")).size() > 0 )
                        return tmpKwDir.path() + "/" + tmpKwDir.entryList(QStringList("*.adx"),QDir::Files,QDir::Name | QDir::Reversed)[0];
                }
            }
        }
    }
    // no previous Track found.
    return "";
}

QString Calendar::roundNumberAsString(double x)
{
    QString val;
    val.setNum(x,'g',3);
    return val;
}

