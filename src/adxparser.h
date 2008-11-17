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
