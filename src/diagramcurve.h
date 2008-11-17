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




    private:
        QwtPlot* parent;
        QwtPlotCurve* curve;
        QString name;
};

#endif // DIAGRAMCURVE_H
