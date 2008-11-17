#include <iostream>
#include <QtGui>
#include <qwt_plot.h>
#include <qdebug.h>

#include "../diagramcurve.h"



int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ActivityDiary");
    QCoreApplication::setOrganizationName("ActivityDiary");

    QwtPlot* plot = new QwtPlot();
    DiagramCurve* curve = new DiagramCurve(plot,"testcurve");

    curve->setValues(QVector<double>(),QVector<double>());
    curve->setColor(QColor(255,0,0));
    curve->setAxis(0,0);
    curve->attachToDiagram(plot);

    int yAxis = curve->yAxis();

    if (yAxis != 0)
        return 1;

    curve->detach();

    delete curve;
    delete plot;
    return 0;
};
