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

bool DiagramCurve::isVisible()
{
    return curve->isVisible();
}




