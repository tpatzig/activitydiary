#include "diagramcurve.h"

DiagramCurve::DiagramCurve(QwtPlot* parent, QString dia_name) : parent(parent), name(dia_name)
{
    setParent(parent);
    curve = new QwtPlotCurve(name);
    setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
}

DiagramCurve::~DiagramCurve()
{
    delete curve;

}

void DiagramCurve::setValues(QVector<double> xValues, QVector<double> yValues)
{
    QwtArray<double> xVal = xValues;
    QwtArray<double> yVal = yValues;

    curve->setData(xVal,yVal);


}

void DiagramCurve::setColor(QColor col)
{
    curve->setPen(QPen(col));

}

void DiagramCurve::setAxis(int xAxis, int yAxis)
{
    curve->setAxis(xAxis,yAxis);
}

void DiagramCurve::detach()
{
    curve->detach();
}

void DiagramCurve::attachToDiagram(QwtPlot* diagram)
{
    curve->attach(diagram);
}

int DiagramCurve::yAxis()
{
    return curve->yAxis();
}





