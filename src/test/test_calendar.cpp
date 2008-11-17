#include <iostream>
#include <QtGui>
#include <qdebug.h>

#include "../calendar.h"



int main(int argc, char** argv)
{
        QApplication a(argc, argv);
        QCoreApplication::setApplicationName("ActivityDiary");
        QCoreApplication::setOrganizationName("ActivityDiary");

        QDate today = QDate::currentDate();
        QDate adx_lower_60min = QDate::fromString("Sun Sep 28 2008");
        QDate adx_higher_60min = QDate::fromString("Tue Aug 26 2008");
        
        Calendar* calendar = new Calendar();
        //testing the slot its self
        calendar->slotUpdateCurrentKW(today);
        //testing with a valid ADX date
        calendar->slotUpdateCurrentKW(adx_lower_60min);
        calendar->slotUpdateCurrentKW(adx_higher_60min);


        QMap<QString,double> map;
        map =  calendar->getWeekSummary(today);
        foreach (QString key, map.keys()) {
            //qDebug() << key << ": " << map[key];
        }
        if (! map.keys().size() > 0)
            return 1;

        map =  calendar->getWeekSummary(adx_lower_60min);
        foreach (QString key, map.keys()) {
          qDebug() << key << ": " << map[key];
            if (! map[key] > 0)
                return 1;
        }
        if (! map.keys().size() > 0)
            return 1;



      qDebug() << "Next Activity Day from today: " << calendar->getNextActivityDay(today);
        if (calendar->getNextActivityDay(today) != "")
            return 1;
      qDebug() << "Previous Activity Day from today: " << calendar->getPrevActivityDay(today);
        if (calendar->getPrevActivityDay(today) == "")
            return 1;

        qDebug() << "Next Activity Day from 28.09.2008: " << calendar->getNextActivityDay(adx_lower_60min);
        qDebug() << "Previous Activity Day from 28.09.2008: " << calendar->getPrevActivityDay(adx_lower_60min);


        //empty Date checks
        calendar->slotUpdateCurrentKW(QDate());
        qDebug() << calendar->getNextActivityDay(QDate());
        qDebug() << calendar->getPrevActivityDay(QDate());
        map =  calendar->getWeekSummary(QDate());



        delete calendar;
        return 0;
}
