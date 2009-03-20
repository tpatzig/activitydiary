#ifndef CALENDER
#define CALENDER

#include <QtGui>

#include "adxparser.h"

class Q_DECL_EXPORT Calendar : public QTextBrowser
{
    Q_OBJECT

public:
    Calendar(QWidget* parent = 0);
    ~Calendar();

    QMap<QString,double> getWeekSummary(const QDate& date);
    QString getNextActivityDay(const QDate&, const QTime&);
    QString getPrevActivityDay(const QDate&, const QTime&);

public slots:
    void slotUpdateCurrentKW(const QDate& date);

private slots:
    void slotTrackSelected(const QUrl&);

signals:
    void calendarUpdated(const QDate&);
    void trackSelected(const QString&);

private:

    QSettings* settings;
    QString roundNumberAsString(double x);


};

#endif
