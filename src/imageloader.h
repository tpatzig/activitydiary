#ifndef IMAGELOADER_H
#define IMAGELOADER_H


#include <QPixmap>
#include <QFile>
#include <QObject>
#include <QHttp>
#include <QDir>
#include <QFileInfo>
#include <qdebug.h>
#include <unistd.h>

#include "waypoint.h"
#include "loader.h"


#define OSM_HOST_A "a.tile.openstreetmap.org"
#define OSM_HOST_B "b.tile.openstreetmap.org"
#define OSM_HOST_C "c.tile.openstreetmap.org"

#define PATH "tiles"


class ImageLoader : public QObject
{
Q_OBJECT

public:

	ImageLoader(QObject* parent = 0);
	~ImageLoader();

	QPixmap load_tile(int zoom, int x, int y);
	QPixmap load_tile(int zoom, Waypoint* wpt);

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
  

public slots:

	void downloadFinished(Loader* loader,int id,bool error);
	void abort_downloads();

signals:

	void download_finished(QString);
	void download_status(int);
    

};
#endif
