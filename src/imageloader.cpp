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

#include "imageloader.h"



ImageLoader::ImageLoader(QObject* parent) : QObject(parent) {

    loader_1 = new Loader(this,OSM_HOST_A);
    loader_1->setObjectName("loader_1");
    loader_2 = new Loader(this,OSM_HOST_B);
    loader_2->setObjectName("loader_2");
    loader_3 = new Loader(this,OSM_HOST_C);
    loader_3->setObjectName("loader_3");
    loader_4 = new Loader(this,OSM_HOST_B);
    loader_4->setObjectName("loader_4");
    loader_5 = new Loader(this,OSM_HOST_A);
    loader_5->setObjectName("loader_5");

    loader_list.append(loader_1 );
    loader_list.append(loader_2 );
    loader_list.append(loader_3 );
    loader_list.append(loader_4 );
    loader_list.append(loader_5 );


    qDebug() << "OSM Host: " << OSM_HOST_A;

    active_download = 0;

    QSettings settings;
    tilesPath = settings.value("TilesDir",QString(QDir::homePath() + "/ActivityDiary/tiles")).toString();
}


ImageLoader::~ImageLoader() {
    qDeleteAll(loader_list.begin(),loader_list.end());
    loader_list.clear();

}

// public functions // 
QPixmap ImageLoader::load_tile(int zoom, int x, int y) {

    if ( access( get_pic_path( zoom,x,y,QString(tilesPath) ).toLatin1(),F_OK ) == 0 ) {
        //qDebug() << "file exists";
        QPixmap local_pixmap(get_pic_path( zoom,x,y,QString(tilesPath) ));
        return local_pixmap;
    } else {
        QString download_path = get_pic_path(zoom,x,y);
        qDebug() << "trying to download " << download_path;
        download_image(download_path);
        return QPixmap("loading.png");
    }
}

QPixmap ImageLoader::load_tile(int zoom, Waypoint* wpt) {
    return load_tile(zoom,wpt->get_floor_x(zoom),wpt->get_floor_y(zoom));
}


// slots //

void ImageLoader::downloadFinished(Loader* loader, int id, bool err) {

    disconnect(loader,SIGNAL(downloadReady(Loader*,int,bool)),this,SLOT(downloadFinished(Loader*,int,bool)));

    foreach( Loader* load,loader_list ) {
        if ( (load->objectName() == loader->objectName()) && (id == loader->active_download_id) ) {

            if (err) {
                qDebug() << "an error occured " << load->http->errorString();
                // remove failed or aborted downloads //
                if (load->current_png_file->exists()) {
                    load->current_png_file->remove();
                    load->close_file();
                }

            } else {

                qDebug() << loader->current_png_file->fileName() << " downloaded";

                QFileInfo info(*loader->current_png_file);

                QString osm_path = info.absoluteFilePath().split(QString(tilesPath))[1];

                qDebug() << "OSM-PATH : " << osm_path;

                emit download_finished(loader->current_png_file->fileName());
                emit download_status(download_queue.count());

                loader->close_file();

                qDebug() << download_queue.count() << " items in queue.";
                if (download_queue.contains(osm_path)) {
                    download_queue.removeAt(download_queue.indexOf(osm_path));
                }
                foreach(QString path,download_queue) {
                    download_image(path);
                }		
            }
            break;
        }
    }
}


// some helper functions //

QString ImageLoader::get_pic_path(int zoom, int x, int y, QString pre) {
    // the same local path as on the osm server:  $zoom/$lat/$lon.png   //
    return QString ( pre + "/" + QString::number(zoom) + "/" + QString::number(x) + "/" + QString::number(y) + ".png" );
}

void ImageLoader::download_image(QString path) {

    foreach(Loader* loader,loader_list) {

        if ( loader->http->currentId() == 0) {
            qDebug() << "Activating Download";

            QFileInfo info( QString(tilesPath) + path );
            QDir dir;
            qDebug() << "Checking if path exists: " << info.absoluteDir();
            if (!info.absoluteDir().exists())
                dir.mkpath(info.path());

            loader->current_png_file = new QFile(info.filePath());
            if (!loader->current_png_file->open(QIODevice::WriteOnly)) {
                qDebug() << "cant open " << loader->current_png_file->fileName();
                return;
            }

            loader->active_download_id = loader->http->get(QString(path),loader->current_png_file);
            //			active_download = 1;
            connect(loader,SIGNAL(downloadReady(Loader*,int,bool)),this,SLOT(downloadFinished(Loader*,int,bool)));
            break;
        }

    } 

    if (!download_queue.contains(path)) {
        qDebug() << "Download queued";
        download_queue.append(path);
    }


}

void ImageLoader::abort_downloads() {
    qDebug() << "Loading Tiles aborted";
    foreach(Loader* loader,loader_list) {
        loader->abort_download();
    }
    download_queue.clear();
    emit download_status(download_queue.count());
}

