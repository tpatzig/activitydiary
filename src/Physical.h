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

#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <QObject>
#include <QString>
#include <QDate>
#include <qdebug.h>


class PhysicalElement : public QObject
{

    public:  
	PhysicalElement(int power, int hr, QDateTime dateTime);
	PhysicalElement(QDateTime dateTime);
	~PhysicalElement();

	int power() { return _power; }
	int hr() { return _hr; }
	QDateTime dateTime() { return _dateTime; }

	void setPower(int power) { _power = power; }
	void setHr(int hr) { _hr = hr; }
	void setDateTime(QDateTime dateTime) { _dateTime = dateTime; }

        bool ge (PhysicalElement* p) const;
	bool le (PhysicalElement* p) const;
	bool gt (PhysicalElement* p) const;
	bool lt (PhysicalElement* p) const;

    private:
	int _power;
	int _hr;
	QDateTime _dateTime;
	

};

class Physical : public QList<PhysicalElement*>
{
    public:
        Physical();
        ~Physical();

        void setInterval(int interval) { _interval = interval; }
    
	PhysicalElement* atSec(int sec);

    private:

        int _interval;
};

#endif
