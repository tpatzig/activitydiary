#include <iostream>
#include <QtGui>
#include <qdebug.h>

#include "../calendar.h"



int main(int argc, char** argv)
{
        QApplication a(argc, argv);
        Calendar* calendar = new Calendar();
        calendar->slotUpdateCurrentKW(QDate::currentDate());

        return 0;
}
