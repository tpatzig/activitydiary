#ifndef HRMPARSER_H
#define HRMPARSER_H

#include <QObject>

#include <qdebug.h>
#include "Physical.h"

class HRMParser : public QObject
{

    public:

        HRMParser();
        ~HRMParser();

	static Physical read(const QString& filename);

};

#endif

