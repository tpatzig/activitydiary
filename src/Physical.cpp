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

