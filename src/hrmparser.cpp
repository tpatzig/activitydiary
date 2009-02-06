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
				
			
