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

#ifndef CALENDER
#define CALENDER

#include <QtGui>

#include "adxparser.h"

class Q_DECL_EXPORT Calendar : public QTextBrowser
{
    Q_OBJECT

public:
    Calendar(QWidget* parent = 0);
    ~Calendar();

    QMap<QString,double> getWeekSummary(const QDate& date);
    QString getNextActivityDay(const QDate&, const QTime&);
    QString getPrevActivityDay(const QDate&, const QTime&);

public slots:
    void slotUpdateCurrentKW(const QDate& date);

private slots:
    void slotTrackSelected(const QUrl&);

signals:
    void calendarUpdated(const QDate&);
    void trackSelected(const QString&);

private:

    QSettings* settings;
    QString roundNumberAsString(double x);


};

#endif
