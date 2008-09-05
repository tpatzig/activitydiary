#include <iostream>
#include <QtGui>
#include <qdebug.h>

#include "../gpxparser.h"
#include "../imageloader.h"



int main(int argc, char** argv)
{
        GPXParser* parser = new GPXParser(argv[1]);
        ImageLoader* loader = new ImageLoader();
        qDebug() << "Count " << parser->count_waypoints();
        
        printf("Latitude 0: %6.10f \n",parser->waypoint_list[0]->get_latitude());
        printf("Longitude 0: %6.10f \n",parser->waypoint_list[0]->get_longitude());
        printf("X: %6.10f \n",parser->waypoint_list[0]->get_rel_x(16));
        printf("Y: %6.10f \n",parser->waypoint_list[0]->get_rel_y(16));

        loader->load_tile(16,parser->waypoint_list[0]);
        

        return 0;
}
