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


