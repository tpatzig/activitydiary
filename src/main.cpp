#include <QApplication>
#include <iostream>
#include <QtGui>
#include <qdebug.h>

#include "mainwindow.h"
#include "sportsdiary.h"

int main( int argc, char **argv )
{
    QApplication app(argc, argv);

    SportsDiary mainWin;
//    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}

