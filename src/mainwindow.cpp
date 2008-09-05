#include <QLayout>
#include <QPushButton>

#include "gpxparser.h"
#include "Track.h"
#include "mainwindow.h"

MainWindow::MainWindow()
	: QMainWindow( 0 )
{
	showMaximized();

	QWidget* centralWidget = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout();

	_mapWidget = new MapView(centralWidget);
	setCentralWidget(centralWidget);
	layout->addWidget(_mapWidget);
	QPushButton* button = new QPushButton("Load Track", centralWidget);
	layout->addWidget(button);
	centralWidget->setLayout(layout);

	connect (button, SIGNAL(clicked()), this, SLOT(slotLoad()));
}



void MainWindow::slotLoad()
{
	// that's what our user interface will to do in the future
	GPXParser* parser = new GPXParser ("2008-08-26-20-15-01.gpx");
	Track* track = new Track();
	track->set_waypoint_list(parser->get_waypoint_list());

	track->set_max_north(parser->max_north());
	track->set_max_south(parser->max_south());
	track->set_max_west(parser->max_west());
	track->set_max_east(parser->max_east());

	_mapWidget->setTrack( track );

}
