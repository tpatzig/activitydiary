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

#include "Physical.h"

#include <QDebug>



PhysicalElement::PhysicalElement(int power, int hr, QDateTime dateTime)
    : _power ( power )
    , _hr ( hr )
    , _dateTime ( dateTime )
{
}

PhysicalElement::PhysicalElement(QDateTime dateTime)
    : _power ( 0 )
    , _hr ( 0 )
    , _dateTime ( dateTime )
{
}

PhysicalElement::~PhysicalElement()
{
}



bool PhysicalElement::ge (PhysicalElement* p) const
{
    if (!p)
        return false;
    return _dateTime >= p->dateTime();
}

bool PhysicalElement::le (PhysicalElement* p) const
{
    if (!p)
        return false;
    return _dateTime <= p->dateTime();
}

bool PhysicalElement::gt (PhysicalElement* p) const
{
    if (!p)
        return false;
    return _dateTime > p->dateTime();
}

bool PhysicalElement::lt (PhysicalElement* p) const
{
    if (!p)
        return false;
    return _dateTime < p->dateTime();
}


Physical::Physical() : QList<PhysicalElement*>()
{
}

Physical::~Physical()
{

}

PhysicalElement* Physical::atSec(int sec)
{
    int index = sec / _interval;

    if ( index <= size() )
	return value (index); 
    else
	return NULL;

}

