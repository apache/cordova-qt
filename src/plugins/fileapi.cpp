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

#include "fileapi.h"

#include "../pluginregistry.h"

#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QUrl>
#include <QTextStream>

// Create static instance of ourself
FileAPI* FileAPI::m_fileAPI = new FileAPI();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
FileAPI::FileAPI() : CPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.File", this );
}

/**
 * LocalFileSystem.requestFileSystem - http://www.w3.org/TR/file-system-api/#widl-LocalFileSystem-requestFileSystem
 */
void FileAPI::requestFileSystem( int scId, int ecId, unsigned short p_type ) {
    Q_UNUSED(ecId)

    QDir dir;

    // Get correct system path
    if( p_type == 0 ) {
        dir = QDir::temp();
    }
    else {
        dir = QDir::home();
    }

    this->callback( scId, "FileSystem.cast( 'Home', '" + dir.dirName() + "', '" + dir.absolutePath() + "/' )" );
}

/**
 * LocalFileSystem.resolveLocalFileSystemURL - http://www.w3.org/TR/file-system-api/#widl-LocalFileSystem-resolveLocalFileSystemURL
 */
void FileAPI::resolveLocalFileSystemURL( int scId, int ecId, QString p_url ) {
    QUrl url = QUrl::fromUserInput( p_url );

    // Check if we have a valid URL
    if( !url.isValid() ) {
        this->callback( ecId, "FileError.cast( FileError.SYNTAX_ERR )" );
        return;
    }

    // Check for the correct scheme
    if( url.scheme() != "file" ) {
        this->callback( ecId, "FileError.cast( FileError.TYPE_MISMATCH_ERR )" );
        return;
    }

    // Now get info about the path
    QFileInfo fileInfo( url.path() );
    // Check if entry exists
    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
        return;
    }

    // Now check if this is a dir or a file
    if( fileInfo.isDir() ) {
        this->callback( scId, "DirectoryEntry.cast( '" + fileInfo.fileName() + "', '" + QDir::cleanPath(fileInfo.absoluteFilePath()) + "/' )" );
        return;
    }
    else {
        this->callback( scId, "FileEntry.cast( '" + fileInfo.fileName() + "', '" + fileInfo.absoluteFilePath() + "' )" );
        return;
    }
}

/**
 * DirectoryEntry.getFile - http://www.w3.org/TR/file-system-api/#widl-DirectoryEntry-getFile
 */
void FileAPI::getFile( int scId, int ecId, QString p_path, QVariantMap p_options ) {
    qDebug() << "Path: " << p_path;

    bool create = p_options.value("create").toBool();
    bool exclusive = p_options.value("exclusive").toBool();

    QFile file( p_path );

    if( file.exists() ) {
        if( create && exclusive ) {
            this->callback( ecId, "FileError.cast( FileError.PATH_EXISTS_ERR )" );
            return;
        }
    }
    else {
        if( !create ) {
            this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
            return;
        }

        // Create the file
        file.open( QIODevice::WriteOnly );
        file.close();

        // Check if creation was successfull
        if( !file.exists() ) {
            this->callback( ecId, "FileError.cast( FileError.NO_MODIFICATION_ALLOWED_ERR )" );
            return;
        }
    }

    // If we reach here, everything went well
    this->callback( scId, "FileEntry.cast( '" + file.fileName() + "', '" + QFileInfo( file ).absoluteFilePath() + "' )" );
}

/**
 * DirectoryEntry.getDirectory - http://www.w3.org/TR/file-system-api/#widl-DirectoryEntry-getDirectory
 */
void FileAPI::getDirectory( int scId, int ecId, QString p_path, QVariantMap p_options ) {
    qDebug() << "Path: " << p_path;

    bool create = p_options.value("create").toBool();
    bool exclusive = p_options.value("exclusive").toBool();

    QDir dir( p_path );

    if( dir.exists() ) {
        if( create && exclusive ) {
            this->callback( ecId, "FileError.cast( FileError.PATH_EXISTS_ERR )" );
            return;
        }
    }
    else {
        if( !create ) {
            this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
            return;
        }

        // Create the folder
        QString folderName = dir.dirName();
        dir.cdUp();
        dir.mkdir( folderName );
        dir.cd( folderName );

        // Check if creation was successfull
        if( !dir.exists() ) {
            this->callback( ecId, "FileError.cast( FileError.NO_MODIFICATION_ALLOWED_ERR )" );
            return;
        }
    }

    // If we reach here, everything went well
    this->callback( scId, "DirectoryEntry.cast( '" + dir.dirName() + "', '" + dir.absolutePath() + "/' )" );
}

/**
 * DirectoryEntry.removeRecursively - http://www.w3.org/TR/file-system-api/#widl-DirectoryEntry-removeRecursively
 */
void FileAPI::removeRecursively( int scId, int ecId, QString p_path ) {
    QDir dir( p_path );

    if( FileAPI::rmDir(dir) ) {
        this->callback( scId, "" );
        return;
    }

    // Something went wrong if we reach here
    this->callback( ecId, "FileError.cast( FileError.INVALID_MODIFICATION_ERR )" );
}

/**
 * FileEntry.file - http://www.w3.org/TR/file-system-api/#widl-FileEntry-file
 * NOTE: mime-type is not supported right now
 */
void FileAPI::file( int scId, int ecId, QString p_path ) {
    QFileInfo fileInfo(p_path);

    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
        return;
    }
    else {
        this->callback( scId, "FileInfo.cast( '" + fileInfo.fileName() + "', '" + fileInfo.absoluteFilePath() + "', 'unknown/unknown', new Date(" + QString::number(fileInfo.lastModified().toMSecsSinceEpoch()) + "), " + QString::number(fileInfo.size()) + " )" );
        return;
    }
}

/**
 * FileWriter.write - http://www.w3.org/TR/file-writer-api/#widl-FileWriter-write
 */
void FileAPI::write( int scId, int ecId, QString p_path, unsigned long long p_position, QString p_data ) {
    QFile file( p_path );

    // Check if file exists
    if( !file.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR ), 0, 0" );
        return;
    }

    // Get detailed info
    QFileInfo fileInfo( file );

    // Try to open the file for writing
    if( !file.open( QIODevice::ReadWrite ) ) {
        this->callback( ecId, "FileError.cast( FileError.NO_MODIFICATION_ALLOWED_ERR ), 0, " + QString::number(fileInfo.size()) );
        return;
    }

    // Create TextStream for writing
    QTextStream textStream( &file );
    textStream.setCodec( "UTF-8" );

    // Seek to correct position
    if( !textStream.seek( p_position ) ) {
        file.close();
        fileInfo.refresh();

        this->callback( ecId, "FileError.cast( FileError.INVALID_MODIFICATION_ERR ), 0, " + QString::number(fileInfo.size()) );
        return;
    }

    // Write the data
    textStream << p_data;
    textStream.flush();
    file.flush();
    file.close();
    fileInfo.refresh();

    // Hooray - we are done!
    this->callback( scId, QString::number(fileInfo.size()) + ", " + QString::number(fileInfo.size()) );
    return;
}

/**
 * FileWriter.truncate - http://www.w3.org/TR/file-writer-api/#widl-FileWriter-truncate
 */
void FileAPI::truncate( int scId, int ecId, QString p_path, unsigned long long p_size ) {
    QFile file(p_path);

    // Check if file exists at all
    if( !file.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR ), 0, 0" );
        return;
    }

    // Try to resize (truncate) the file
    if( !file.resize(p_size) ) {
        this->callback( ecId, "FileError.cast( FileError.NO_MODIFICATION_ALLOWED_ERR ), " + QString::number(file.size()) + ", " + QString::number(file.size()) );
        return;
    }

    // Done
    this->callback( scId, QString::number(p_size) + ", " + QString::number(p_size) );
    return;
}

/**
 * Entry.getParent - http://www.w3.org/TR/file-system-api/#widl-Entry-getParent
 */
void FileAPI::getParent( int scId, int ecId, QString p_path ) {
    QDir dir( p_path );

    // Try to change into upper directory
    if( !dir.cdUp() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
        return;
    }

    // Extract names and send back
    this->callback( scId, "DirectoryEntry.cast( '" + dir.dirName() + "', '" + dir.absolutePath() + "/' )" );
    return;
}

/**
 * Entry.remove - http://www.w3.org/TR/file-system-api/#widl-Entry-remove
 */
void FileAPI::remove( int scId, int ecId, QString p_path ) {
    QFileInfo fileInfo(p_path);

    // Check if entry exists at all
    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
        return;
    }

    // Check if we have a dir
    if( fileInfo.isDir() ) {
        // Try to remove the directory
        QDir dir(p_path);
        if( dir.rmdir( dir.absolutePath() ) ) {
            this->callback( scId, "" );
            return;
        }
    }
    else {
        // Try to remove the file
        QFile file(p_path);
        if( file.remove() ) {
            this->callback( scId, "" );
            return;
        }
    }

    // Something went wrong if we reached here
    this->callback( ecId, "FileError.cast( FileError.NO_MODIFICATION_ALLOWED_ERR )" );
    return;
}

/**
 * Entry.getMetadata - http://www.w3.org/TR/file-system-api/#widl-Entry-getMetadata
 */
void FileAPI::getMetadata( int scId, int ecId, QString p_path ) {
    QFileInfo fileInfo( p_path );

    // Check if file exists
    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
        return;
    }

    // Return modification date
    this->callback( scId, "FileMetadata.cast( new Date(" + QString::number(fileInfo.lastModified().toMSecsSinceEpoch()) + ") )" );
    return;
}

/**
 * DirectoryReader.readEntries - http://www.w3.org/TR/file-system-api/#widl-DirectoryReader-readEntries
 */
void FileAPI::readEntries( int scId, int ecId, QString p_path ) {
    QDir dir( p_path );

    QString entriesList = "";

    if( dir.exists() ) {
        // Iterate over entries and add them to response
        Q_FOREACH( const QFileInfo &fileInfo, dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) ) {
            if( fileInfo.isDir() ) {
                entriesList += "DirectoryEntry.cast('" + fileInfo.fileName() + "','" + fileInfo.absoluteFilePath() + "/'),";
            }
            else {
                entriesList += "FileEntry.cast('" + fileInfo.fileName() + "','" + fileInfo.absoluteFilePath() + "'),";
            }
        }

        // Remove trailing comma
        if(entriesList.size() > 0 ) entriesList.remove(entriesList.size()-1, 1);
        entriesList = "new Array(" + entriesList + ")";

        // All done, send back to client
        this->callback( scId, entriesList );
        return;
    }

    // If we reach here, something went wrong
    this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
}

/**
 * FileReader.readAsText - http://www.w3.org/TR/FileAPI/#dfn-readAsText
 */
void FileAPI::readAsDataURL( int scId, int ecId, QString p_path ) {
    QFile file( p_path );

    // Check if file exists at all
    if( !file.exists() ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_FOUND_ERR )" );
        return;
    }

    // Try to open file for reading
    if( !file.open( QIODevice::ReadOnly ) ) {
        this->callback( ecId, "FileError.cast( FileError.NOT_READABLE_ERR )" );
        return;
    }

    // Read the file content
    QByteArray byteArray = file.readAll();

    // Escape string & send back
    this->callback( scId, "'" + byteArray.toBase64() + "'" );
    return;
}

/*
 * Helper function for recursively removing a directory
 */
bool FileAPI::rmDir( QDir p_dir ) {
    if( p_dir.exists() ) {
        // Iterate over entries and remove them
        Q_FOREACH( const QFileInfo &fileInfo, p_dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) ) {
            if( fileInfo.isDir() ) {
                if( !FileAPI::rmDir( fileInfo.dir() ) ) {
                    return false;
                }
            }
            else {
                if( !QFile::remove( fileInfo.absoluteFilePath() ) ) {
                    return false;
                }
            }
        }

        // Finally remove the current dir
        qDebug() << p_dir.absolutePath();
        return p_dir.rmdir( p_dir.absolutePath() );
    }

    return false;
}
