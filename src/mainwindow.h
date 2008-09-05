#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <qobject.h>
#include <QMainWindow>
#include "mapview.h"

// Class to represent our MainWindow 
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
		MainWindow();

private:
	MapView* _mapWidget;


private slots:
	void slotLoad();

};


#endif

