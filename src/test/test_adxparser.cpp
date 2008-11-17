#include <iostream>
#include <QtGui>
#include <qdebug.h>

#include "../adxparser.h"



int main(int argc, char** argv)
{
        QApplication a(argc, argv);
        QCoreApplication::setApplicationName("ActivityDiary");
        QCoreApplication::setOrganizationName("ActivityDiary");

        
        QString testfile = "/tmp/testfile.adx";
        QFile file(testfile);
        QFileInfo info(file);
        QString newGpxFileName = info.path() + "/" + info.baseName() + ".gpx";
        QMap<QString,QString> map;
        map["activitytype"] = "Running";
        map["description"] = "foobar";
        map["startdate"] = "Mon 29 Sep 2008";
        
        AdxParser::writeSettings(testfile,map);

        qDebug() << "has Setting? " << AdxParser::hasSetting(testfile,"activitytype");
        if (! AdxParser::hasSetting(testfile,"activitytype") )
            return 1;

        qDebug() << "Setting: " << AdxParser::readSetting(testfile,"trackfile");
        if ( AdxParser::readSetting(testfile,"trackfile") != "/tmp/testfile.gpx")
            return 1;

        qDebug() << "Setting: " << AdxParser::readSetting(testfile,"activitytype");
        if (AdxParser::readSetting(testfile,"activitytype") != "Running")
            return 1;
        qDebug() << "SettingSize: " << AdxParser::readSettings(testfile).keys().size();
        if (AdxParser::readSettings(testfile).keys().size() != 4 ) 
            return 1;

        file.remove();
        
        return 0;
}
