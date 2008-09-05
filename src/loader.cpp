#include "loader.h"

Loader::Loader(QObject* parent, QString host ) : QObject(parent) {

	http = new QHttp(host);
	connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(downloaded(int,bool)));
	current_png_file = 0;
}

Loader::~Loader() {

	if (http)
		delete http;
	if (current_png_file)
		delete current_png_file;
}

void Loader::downloaded(int id, bool err) {

	emit downloadReady(this,id,err);
}

void Loader::close_file() {
	if (current_png_file) {
		current_png_file->close();
		delete current_png_file;
		current_png_file = 0;
	}

}

void Loader::abort_download() {

	http->abort();
	
}


