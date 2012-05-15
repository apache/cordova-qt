/*
 *  Copyright 2011 Wolfgang Koller - http://www.gofg.at/
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef FILEAPI_H
#define FILEAPI_H

#include "../cplugin.h"

#include <QDir>
#include <QVariantMap>
#include <QMap>

class FileAPI : public CPlugin
{
    Q_OBJECT
public:
    explicit FileAPI();

signals:

public slots:
    void requestFileSystem( int scId, int ecId, unsigned short p_type, unsigned long long p_size);
    void resolveLocalFileSystemURL( int scId, int ecId, QString p_url );
    void getDirectory( int scId, int ecId, QString p_path, QVariantMap p_options );
    void getFile( int scId, int ecId, QString p_path, QVariantMap p_options );
    void getParent( int scId, int ecId, QString p_path );
    void getMetadata( int scId, int ecId, QString p_path );
    void remove( int scId, int ecId, QString p_path );
    void removeRecursively( int scId, int ecId, QString p_path );
    void file( int scId, int ecId, QString p_path );
    void write( int scId, int ecId, QString p_path, unsigned long long p_position, QString p_data );
    void truncate( int scId, int ecId, QString p_path, unsigned long long p_size );
    void readEntries( int scId, int ecId, QString p_path );
    void readAsDataURL( int scId, int ecId, QString p_path );
//    void copyTo();
//    void moveTo();
//    void canCopyMoveSrc();
//    void doCopyMove();
//    void getFileMetadata();
//    void readEntries();
//    void readFile();
//    void readAsText();
//    void readAsDataUrl();
protected:
    static bool rmDir( QDir p_dir );

private:
    static FileAPI* m_fileAPI;
    typedef QMap<QString, QString> MimeTypeMap;
    static MimeTypeMap createMimeTypeMap();
    static MimeTypeMap mimeMap_;
};

#endif // FILEAPI_H
