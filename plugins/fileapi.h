/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#ifndef FILEAPI_H
#define FILEAPI_H

#include "../pgplugin.h"

#include <QDir>

class FileAPI : public PGPlugin
{
    Q_OBJECT
public:
    explicit FileAPI(QWebFrame *p_webFrame);

signals:

public slots:
    void requestFileSystem( int scId, int ecId, unsigned short p_type );
    void resolveLocalFileSystemURL( int scId, int ecId, QString p_url );
    void getFile( int scId, int ecId, QString p_path, QVariantMap p_options );
    void getDirectory( int scId, int ecId, QString p_path, QVariantMap p_options );
    void removeRecursively( int scId, int ecId, QString p_path );
    void file( int scId, int ecId, QString p_path );
    void write( int scId, int ecId, QString p_path, unsigned long long p_position, QString p_data );
    void truncate( int scId, int ecId, QString p_path, unsigned long long p_size );
    void getParent( int scId, int ecId, QString p_path );
    void remove( int scId, int ecId, QString p_path );
    void getMetadata( int scId, int ecId, QString p_path );
    void readEntries( int scId, int ecId, QString p_path );
    void readAsDataURL( int scId, int ecId, QString p_path );

protected:
    static bool rmDir( QDir p_dir );
};

#endif // FILEAPI_H
