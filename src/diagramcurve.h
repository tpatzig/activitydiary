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

#ifndef DIAGRAMCURVE_H
#define DIAGRAMCURVE_H

#include <QtGui>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_curve_fitter.h>

class Q_DECL_EXPORT DiagramCurve : public QWidget
{
    Q_OBJECT

    public:
        DiagramCurve(QwtPlot* parent = 0,QString name = "");
        ~DiagramCurve();

        void setValues(QVector<double>,QVector<double>);
        void setColor(QColor);
        void setAxis(int,int);
        void detach();
        void attachToDiagram(QwtPlot*);
        int yAxis();
        bool isVisible();




    private:
        QwtPlot* parent;
        QwtPlotCurve* curve;
        QString name;
};

#endif // DIAGRAMCURVE_H
