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

#include "hrmparser.h"
#include "Physical.h"


#include <QSettings>
#include <QFile>
#include <QStringList>

	
HRMParser::HRMParser()
{
}

HRMParser::~HRMParser()
{
}
			
Physical HRMParser::read(const QString& fileName)
{

    // read header
    QSettings file( fileName, QSettings::IniFormat );
    file.beginGroup( "Params" );
    QDate date = QDate::fromString( file.value( "Date" ).toString() ,  "yyyyMMdd" );
    QTime time = QTime::fromString( file.value("StartTime").toString(), "hh:mm:ss.z");
    int interval  = file.value("Interval").toInt();
    file.endGroup();

    qDebug() << date;
    qDebug() << time;

    QFile ifile(fileName);
    if (!ifile.open(QIODevice::ReadOnly | QIODevice::Text))
	return Physical();

    bool ignoreLine = true;
    
    QDateTime dateTime = QDateTime(date, time);
    Physical p;
    p.setInterval(interval);

    QTextStream in(&ifile);
    while (!in.atEnd())
    {
	QString line = in.readLine();
	if ( line.contains("[HRData]") )
	    ignoreLine=false;

	if ( ! ignoreLine )
	{
	   QStringList linesplit = line.split('\t', QString::SkipEmptyParts );
	    if (linesplit.size() > 1 )
	    {
		int hr= linesplit[0].toInt();
		PhysicalElement* pe = new PhysicalElement ( dateTime );
		pe->setHr(hr);
		p << pe;
		dateTime.addSecs( interval );
	    }
	}
    } 

    return p;

}
				
			
