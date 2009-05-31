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

#include <QLayout>
#include <QPushButton>

#include "gpxparser.h"
#include "Track.h"
#include "mainwindow.h"

MainWindow::MainWindow(): QMainWindow( 0 )
{
    showMaximized();

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout();

    _mapWidget = new MapView(centralWidget);
    setCentralWidget(centralWidget);
    layout->addWidget(_mapWidget);
    QPushButton* button = new QPushButton(tr("Load Track"), centralWidget);
    layout->addWidget(button);
    centralWidget->setLayout(layout);
}

#if 0

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

#endif
