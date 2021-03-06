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

#ifndef ADXPARSER
#define ADXPARSER

#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class Q_DECL_EXPORT AdxParser
{

public:
    AdxParser();
    ~AdxParser();

    static void writeSettings(QString filename, QMap<QString,QString> values);
    static QMap<QString,QString> readSettings(QString filename);
    static QString readSetting(QString filename, QString key);
    static bool hasSetting(QString filename, QString key);


};

#endif
