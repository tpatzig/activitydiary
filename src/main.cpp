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

#include <QApplication>
#include <iostream>
#include <QtGui>
#include <qdebug.h>
#include <QTranslator>
#include <QLocale>

#include "sportsdiary.h"

#define APP_PREFIX "/usr/share/activitydiary"

int main( int argc, char **argv )
{
    QApplication app(argc, argv);

    QTranslator translator;

    if (QDir(QString(APP_PREFIX) + "/ts").exists()) {
        translator.load(QString(APP_PREFIX) + "/ts/activitydiary_" + QLocale::system().name() );
    } else {
        translator.load("activitydiary_" + QLocale::system().name() );
    }

    app.installTranslator(&translator);

    QCoreApplication::setOrganizationName("ActivityDiary");
    QCoreApplication::setApplicationName("ActivityDiary");
    QCoreApplication::setOrganizationDomain("activitydiary.org");

    SportsDiary mainWin;
    //    MainWindow mainWin;

    mainWin.show();
    return app.exec();
}

