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
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <QUrl>
#include <QTextStream>

// Create static instance of ourself
FileAPI* FileAPI::m_fileAPI = new FileAPI();
//use static method create initialized map for hardcoded mimetype
FileAPI::MimeTypeMap FileAPI::mimeMap_(FileAPI::createMimeTypeMap());

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
FileAPI::FileAPI() : CPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.File", this );
}

FileAPI::MimeTypeMap FileAPI::createMimeTypeMap(){
    MimeTypeMap map;
    map.insert(QString("txt"),QString("text/plain"));
    map.insert(QString("css"),QString("text/css"));
    map.insert(QString("js"),QString("text/javascript"));
    map.insert(QString("xml"),QString("text/xml"));
    map.insert(QString("html"),QString("text/html"));
    return map;
}

/**
 * LocalFileSystem.requestFileSystem - http://www.w3.org/TR/file-system-api/#widl-LocalFileSystem-requestFileSystem
 */
void FileAPI::requestFileSystem( int scId, int ecId, unsigned short p_type, unsigned long long p_size) {
    QDir dir;
    //FIXEME,what is quota value
    if (p_size >= 10000){
        this->callback( ecId, "FileException.cast( FileException.QUOTA_EXCEEDED_ERR)");
    }
    QString absPath;
    // Get correct system path
    if( p_type == 0 ) {
        dir = QDir::temp();
        absPath = dir.absolutePath();
    }
    else {
        dir = QDir::current();
        absPath = dir.absolutePath() + "/doc";
    }

    if (p_type == 0){
        this->callback( scId, "FileSystem.cast( 'temporary', '" + dir.dirName() + "', '" + absPath + "')" );
    } else if (p_type == 1){
        this->callback( scId, "FileSystem.cast( 'persistent', '" + dir.dirName() + "', '" + absPath + "')" );
    } else {
        this->callback( ecId, "FileException.cast( FileException.SYNTAX_ERR )" );
    }
}

/**
 * LocalFileSystem.resolveLocalFileSystemURL - http://www.w3.org/TR/file-system-api/#widl-LocalFileSystem-resolveLocalFileSystemURL
 */
void FileAPI::resolveLocalFileSystemURL( int scId, int ecId, QString p_url ) {
    QUrl url = QUrl::fromUserInput( p_url );
    // Check if we have a valid URL
    if( !url.isValid() ) {
        this->callback( ecId, "FileException.cast( FileException.ENCODING_ERR )" );
        return;
    }//FIXEME: invalid pass

    // Check for the correct scheme
    if( url.scheme() != "file" ) {
        this->callback( ecId, "FileException.cast( FileException.TYPE_MISMATCH_ERR )" );
        return;
    }

    // Now get info about the path
    QFileInfo fileInfo( url.path() );
    // Check if entry exists
    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
        return;
    }
    if( fileInfo.isDir() ) {
        this->callback( scId, "DirectoryEntry.cast( '" + fileInfo.fileName() + "', '" + QDir::cleanPath(fileInfo.absoluteFilePath()) + "' )" );
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
    //NOTE: colon is not safe in url, it is not a valid path in Win and Mac, simple disable it here.
    if(p_path.contains(":")){
        this->callback( ecId, "FileException.cast( FileException.ENCODING_ERR )" );
        return;
    }
    // Check if we have a valid URL
    QUrl url = QUrl::fromUserInput( p_path );
    if( !url.isValid() ) {
        this->callback( ecId, "FileException.cast( FileException.ENCODING_ERR )" );
        return;
    }
    // Check for the correct scheme
    if( url.scheme() != "file" ) {
        this->callback( ecId, "FileException.cast( FileException.TYPE_MISMATCH_ERR )" );
        return;
    }
    bool create = p_options.value("create").toBool();
    bool exclusive = p_options.value("exclusive").toBool();

    QFile file( p_path );
    QFileInfo pathInfo( p_path );
    QString fileName(pathInfo.fileName());

// if create is false and the path represents a directory, return error
// if file does exist, and create is true and exclusive is true, return error
// if file does not exist and create is false, return error
// if file does not exist and create is true, create file and return File entry
    QFileInfo fileInfo( url.path() );
    if( (!create) && fileInfo.isDir() ) {
        this->callback( ecId, "FileException.cast( FileException.TYPE_MISMATCH_ERR )" );
        return;
    }

    if( file.exists() ) {
        if( create && exclusive ) {
            this->callback( ecId, "FileException.cast( FileException.PATH_EXISTS_ERR )" );
            return;
        }
    }
    else {
        if( !create ) {
            this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
            return;
        }

        // Create the file
        file.open( QIODevice::WriteOnly );
        file.close();

        // Check if creation was successfull
        if( !file.exists() ) {
            this->callback( ecId, "FileException.cast( FileException.NO_MODIFICATION_ALLOWED_ERR )" );
            return;
        }
    }

    // If we reach here, everything went well
    this->callback( scId, "FileEntry.cast( '" + fileName + "', '" + QFileInfo( file ).absoluteFilePath() + "' )" );
}

/**
 * DirectoryEntry.getDirectory - http://www.w3.org/TR/file-system-api/#widl-DirectoryEntry-getDirectory
 */
void FileAPI::getDirectory( int scId, int ecId, QString p_path, QVariantMap p_options ) {
    //NOTE: colon is not safe in url, it is not a valid path in Win and Mac, simple disable it here.
    if(p_path.contains(":")){
        this->callback( ecId, "FileException.cast( FileException.ENCODING_ERR )" );
    }
    // Check if we have a valid URL
    QUrl url = QUrl::fromUserInput( p_path );
    if( !url.isValid()) {
        this->callback( ecId, "FileException.cast( FileException.ENCODING_ERR )" );
        return;
    }

    // Check for the correct scheme
    if( url.scheme() != "file" ) {
        this->callback( ecId, "FileException.cast( File:Exception.TYPE_MISMATCH_ERR )" );
        return;
    }
    bool create = p_options.value("create").toBool();
    bool exclusive = p_options.value("exclusive").toBool();

    QDir dir( p_path );

    //  if create is false and the path represents a file, return error
    //  if directory does exist and create is true and exclusive is true, return error
    //  if directory does not exist and create is false and directory does not exist, return error
    //  if directory does not exist and create is true, create dir and return directory entry

    QFileInfo fileInfo( url.path() );
    if( (!create) && fileInfo.isFile() ) {
        this->callback( ecId, "FileException.cast( FileException.TYPE_MISMATCH_ERR )" );
        return;
    }


    if( dir.exists() ) {
        if( create && exclusive ) {
            this->callback( ecId, "FileException.cast( FileException.PATH_EXISTS_ERR )" );
            return;
        }
    }
    else {
        if( !create ) {
            this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
            return;
        }

        // Create the folder
        QString folderName = dir.dirName();
        dir.cdUp();
        dir.mkdir( folderName );
        dir.cd( folderName );

        // Check if creation was successfull
        if( !dir.exists() ) {
            this->callback( ecId, "FileException.cast( FileException.NO_MODIFICATION_ALLOWED_ERR )" );
            return;
        }
    }

    // If we reach here, everything went well
    this->callback( scId, "DirectoryEntry.cast( '" + dir.dirName() + "', '" + dir.absolutePath() + "' )" );
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
    this->callback( ecId, "FileException.cast( FileException.NO_MODIFICATION_ALLOWED_ERR )" );
}

/**
 * FileEntry.file - http://www.w3.org/TR/file-system-api/#widl-FileEntry-file
 * NOTE: mime-type is not supported right now
 */
void FileAPI::file( int scId, int ecId, QString p_path ) {
    QFileInfo fileInfo(p_path);

    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
        return;
    }
    else {
        this->callback( scId, "File.cast( '" + fileInfo.fileName() + "', '" + fileInfo.absoluteFilePath() + "', 'unknown/unknown', new Date(" + QString::number(fileInfo.lastModified().toMSecsSinceEpoch()) + "), " + QString::number(fileInfo.size()) + " )" );
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
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR ), 0, 0" );
        return;
    }

    // Get detailed info
    QFileInfo fileInfo( file );

    // Try to open the file for writing
    if( !file.open( QIODevice::ReadWrite ) ) {
        this->callback( ecId, "FileException.cast( FileException.NO_MODIFICATION_ALLOWED_ERR ), 0, " + QString::number(fileInfo.size()) );
        return;
    }

    // Create TextStream for writing
    QTextStream textStream( &file );
    textStream.setCodec( "UTF-8" );

    // Seek to correct position
    if( !textStream.seek( p_position ) ) {
        file.close();
        fileInfo.refresh();

        this->callback( ecId, "FileException.cast( FileException.INVALID_MODIFICATION_ERR ), 0, " + QString::number(fileInfo.size()) );
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
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR ), 0, 0" );
        return;
    }

    // Try to resize (truncate) the file
    if( !file.resize(p_size) ) {
        this->callback( ecId, "FileException.cast( FileException.NO_MODIFICATION_ALLOWED_ERR ), " + QString::number(file.size()) + ", " + QString::number(file.size()) );
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

    //can't cdup more than app's root
    QDir root = QDir::current();
    QString absPath = root.absolutePath() + "/doc";
    // Try to change into upper directory
    if( p_path != absPath){
        if( !dir.cdUp() ) {
            this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
            return;
        }

    }
    // Extract names and send back
    this->callback( scId, "DirectoryEntry.cast( '" + dir.dirName() + "', '" + dir.absolutePath() + "' )" );
    return;
}

/**
 * Entry.remove - http://www.w3.org/TR/file-system-api/#widl-Entry-remove
 */
void FileAPI::remove( int scId, int ecId, QString p_path ) {
    QFileInfo fileInfo(p_path);
    QDir root = QDir::current();
    QString absPath = root.absolutePath() + "/doc";
    // Check if entry exists at all
    if( !fileInfo.exists() || (p_path == absPath)) {
        this->callback( ecId, "FileException.cast( FileException.NO_MODIFICATION_ALLOWED_ERR )" );
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
    this->callback( ecId, "FileException.cast( FileException.INVALID_MODIFICATION_ERR )" );
    return;
}

/**
 * Entry.getMetadata - http://www.w3.org/TR/file-system-api/#widl-Entry-getMetadata
 */
void FileAPI::getMetadata( int scId, int ecId, QString p_path ) {
    QFileInfo fileInfo( p_path );


    // Check if file exists
    if( !fileInfo.exists() ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
        return;
    }

    // Return modification date
    this->callback( scId, "Metadata.cast( new Date(" + QString::number(fileInfo.lastModified().toMSecsSinceEpoch()) + ") )" );
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
    this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
}

/**
 * FileReader.readAsText - http://www.w3.org/TR/FileAPI/#dfn-readAsText
 */
void FileAPI::readAsText( int scId, int ecId, QString p_path ) {
    QFile file( p_path );

    // Check if file exists at all
    if( !file.exists() ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
        return;
    }
    // Try to open file for reading
    if( !file.open( QIODevice::ReadOnly ) ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_READABLE_ERR )" );
        return;
    }
    QByteArray byteArray = file.readAll();
    // Escape string & send back
    this->callback( scId, "'" + byteArray.toBase64() + "'" );
    return;
}

/**
 * FileReader.readAsDataURL - http://www.w3.org/TR/FileAPI/#dfn-readAsText
 */
void FileAPI::readAsDataURL( int scId, int ecId, QString p_path ) {
    QFile file( p_path );
    QFileInfo fileInfo( p_path );

    if(p_path.startsWith("content:")){
        this->callback( ecId, "FileException.cast( FileException.NOT_READABLE_ERR )" );
        return;
    }
    // Check if file exists at all
    if( !file.exists() ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_FOUND_ERR )" );
        return;
    }
    // Try to open file for reading
    if( !file.open( QIODevice::ReadOnly ) ) {
        this->callback( ecId, "FileException.cast( FileException.NOT_READABLE_ERR )" );
        return;
    }
    // Read the file content
    QByteArray byteArray = file.readAll();
    QString contentType( mimeMap_[fileInfo.completeSuffix()] );

    // Escape string & send back
//    this->callback( scId, "'" + byteArray.toBase64() + "'" );
    this->callback( scId, "'data:" + contentType + ";base64," + byteArray.toBase64() + "'" );
    return;
}

/*
 * Helper function for recursively removing a directory
 */
bool FileAPI::rmDir( QDir p_dir ) {
    QDir dir = QDir::current();
    QString absPath = dir.absolutePath() + "/doc";
    if ( p_dir == absPath){//can't remove root dir
        return false;
    }
    bool result = true;
    if( p_dir.exists() ) {
        // Iterate over entries and remove them
        Q_FOREACH( const QFileInfo &fileInfo, p_dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) ) {
            if (fileInfo.isDir()) {
                result = rmDir(fileInfo.absoluteFilePath());
            }
            else {
                result = QFile::remove(fileInfo.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }

        // Finally remove the current dir
        qDebug() << p_dir.absolutePath();
        return p_dir.rmdir( p_dir.absolutePath() );
    }
    return result;
}

void FileAPI::copyFile(int scId, int ecId,const QString& sourceFile, const QString& destinationParentDir, const QString& newName)
{
    qDebug()<< Q_FUNC_INFO;
    QString destinationFile;
    if (newName.isEmpty()){
        QFileInfo fileInfo(sourceFile);
        destinationFile = destinationParentDir + "/" + fileInfo.fileName();
        destinationFile = destinationParentDir + "/" + newName;
    } else {
    }
    if(QFile::copy(sourceFile, destinationFile)){
        qDebug()<< Q_FUNC_INFO << "success";
    };
    this->callback( scId, "" );
}

void FileAPI::moveFile(int scId, int ecId,const QString& sourceFile, const QString& destinationParentDir, const QString& newName){
    qDebug()<< Q_FUNC_INFO;
    copyFile(scId,ecId,sourceFile, destinationParentDir, newName);
    remove(scId,ecId, sourceFile);
    this->callback( scId, "" );
}

void FileAPI::copyDir(int scId, int ecId,const QString& sourceFolder, const QString& destinationParentDir, const QString& newName)
{
    qDebug()<< Q_FUNC_INFO;
    QDir sourceDir(sourceFolder);
    QString destFolder(destinationParentDir + "/" + newName);
    if(newName.isEmpty()){
        destFolder = (destinationParentDir + "/" + sourceDir.dirName());
    }
    qDebug() << "destFolder: "<<destFolder;
    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        qDebug() << "mkdir" << destDir.mkdir(destFolder);;
    }
    copyFolder(sourceFolder, destFolder);
    this->callback( scId, "" );
}

void FileAPI::moveDir(int scId, int ecId,const QString& sourceDir, const QString& destinationParentDir, const QString& newName){
     qDebug()<< Q_FUNC_INFO;
     copyDir(scId, ecId, sourceDir, destinationParentDir,newName);
     removeRecursively(scId, ecId,sourceDir );
     this->callback( scId, "" );
}

//helper function to copy foler to new destination
bool FileAPI::copyFolder(const QString& sourceFolder, const QString& destFolder)
{
    qDebug()<< Q_FUNC_INFO;
    QDir sourceDir(sourceFolder);
    if(!sourceDir.exists())
        return false;
    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        destDir.mkdir(destFolder);
    }
    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        QFile::copy(srcName, destName);
    }
    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        copyFolder(srcName, destName);
    }
    return true;
}

