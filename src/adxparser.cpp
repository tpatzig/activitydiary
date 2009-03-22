#include "adxparser.h"

AdxParser::AdxParser()
{
}

AdxParser::~AdxParser()
{
}

void AdxParser::writeSettings(QString filename, QMap<QString,QString> settings)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("activity");
    doc.appendChild(root);
  

    QFile file(filename);
    QFileInfo info(filename);
    QDir dir;
    if (!info.absoluteDir().exists())
        dir.mkpath(info.path());

    QString newGpxFileName = info.path() + "/" + info.baseName() + ".gpx";
    if (!info.exists())
        QFile::copy(settings["trackfile"],newGpxFileName);

    settings["trackfile"] = newGpxFileName;

    if (settings.contains("heartratefile")) {
        QString newHRMFileName = info.path() + "/" + info.baseName() + ".hrm";
        if (!info.exists())
            QFile::copy(settings["heartratefile"],newHRMFileName);

        settings["heartratefile"] = newHRMFileName;
    }

    foreach(QString key, settings.keys()) {
        QDomElement element = doc.createElement(key);
        root.appendChild(element);
        QDomText t = doc.createTextNode(settings[key]);
        element.appendChild(t);
    }

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        qDebug() << "Could not open " << filename;
    else {
        QTextStream out(&file);
        out << doc.toString();
        file.flush();
        file.close();
    }

}

QMap<QString,QString> AdxParser::readSettings(QString filename)
{

    QDomDocument doc("ActivityDiary XML");
    QMap<QString, QString> settings;
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        qDebug() << "Could not open " << filename;
    else
        doc.setContent(&file);
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();

    while(!n.isNull()) {
         QDomElement e = n.toElement();
         if(!e.isNull()) {
             settings[e.tagName()] = e.text();

         }
         n = n.nextSibling();
     }
     return settings;
}

QString AdxParser::readSetting(QString filename, QString key)
{

    QDomDocument doc("ActivityDiary XML");
    QString setting;
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        qDebug() << "Could not open " << filename;
    else
        doc.setContent(&file);
    file.close();

    QDomElement docElem = doc.documentElement();


    QDomElement find = docElem.firstChildElement(key);
    setting = find.text();


     return setting;
}

bool AdxParser::hasSetting(QString filename, QString key)
{
    QDomDocument doc("ActivityDiary XML");
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        qDebug() << "Could not open " << filename;
    else
        doc.setContent(&file);
    file.close();

    QDomElement docElem = doc.documentElement();


    QDomElement find = docElem.firstChildElement(key);
    
    return (! find.isNull());

}


