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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H


#include <QPixmap>
#include <QFile>
#include <QObject>
#include <QHttp>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <qdebug.h>
#include <unistd.h>

#include "waypoint.h"
#include "loader.h"


#define OSM_HOST_A "a.tile.openstreetmap.org"
#define OSM_HOST_B "b.tile.openstreetmap.org"
#define OSM_HOST_C "c.tile.openstreetmap.org"


class ImageLoader : public QObject
{
    Q_OBJECT

    public:

        ImageLoader(QObject* parent = 0);
        ~ImageLoader();

        QPixmap load_tile(int zoom, int x, int y);
        QPixmap load_tile(int zoom, Waypoint* wpt);

        int getQueueCount() {return download_queue.count(); };

    private:

        Loader* loader_1;
        Loader* loader_2;
        Loader* loader_3;
        Loader* loader_4;
        Loader* loader_5;

        QList<Loader*> loader_list;

        void download_image(QString path);
        QFile* local_file(int zoom,int x,int y);
        //	QString get_local_path(int zoom, int x, int y);
        //	QString get_local_path(QString);
        //	QString get_osm_path(int zoom, int x, int y);
        QString get_pic_path(int zoom, int x, int y, QString pre = 0);

        QStringList download_queue;
        bool active_download;
        int active_download_id;

        QString tilesPath;


    public slots:

        void downloadFinished(Loader* loader,int id,bool error);
        void abort_downloads();

    signals:

        void download_finished(QString);
        void download_status(int);


};
#endif
