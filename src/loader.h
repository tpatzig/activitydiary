#ifndef LOADER_H
#define LOADER_H


#include <QObject>
#include <QHttp>
#include <QFile>

class Loader : public QObject
{
Q_OBJECT

public:
	Loader(QObject* parent = 0,QString host = "");
	~Loader();
	
	QHttp* http;
	QFile* current_png_file;
	int active_download_id;

	void close_file();

public slots:
	void downloaded(int,bool);
	void abort_download();

signals:

	void downloadReady(Loader*,int id,bool err);
		
	
};

#endif
