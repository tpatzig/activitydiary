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
