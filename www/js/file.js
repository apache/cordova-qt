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

/**
 * FileError interface
 * http://www.w3.org/TR/file-system-api/#the-fileerror-interface
 */
function FileError() {
}

FileError.cast = function( p_code ) {
        var fe = new FileError();
        fe.code = p_code;

        return fe;
}
FileError.prototype.code = 0;
FileError.NOT_FOUND_ERR = 1;
FileError.SECURITY_ERR = 2;
FileError.ABORT_ERR = 3;
FileError.NOT_READABLE_ERR = 4;
FileError.ENCODING_ERR = 5;
FileError.NO_MODIFICATION_ALLOWED_ERR = 6;
FileError.INVALID_STATE_ERR = 7;
FileError.SYNTAX_ERR = 8;
FileError.INVALID_MODIFICATION_ERR = 9;
FileError.QUOTA_EXCEEDED_ERR = 10;
FileError.TYPE_MISMATCH_ERR = 11;
FileError.PATH_EXISTS_ERR = 12;

/**
 * FileException interface
 * http://www.w3.org/TR/FileAPI/#dfn-FileException
 */
function FileException() {
}

FileException.cast = function( p_code ) {
        var fe = new FileException();
        fe.code = p_code;

        return fe;
}
FileException.prototype.code = 0;
FileException.NOT_FOUND_ERR = 1;
FileException.SECURITY_ERR = 2;
FileException.ABORT_ERR = 3;
FileException.NOT_READABLE_ERR = 4;
FileException.ENCODING_ERR = 5;
FileException.NO_MODIFICATION_ALLOWED_ERR = 6;
FileException.INVALID_STATE_ERR = 7;
FileException.SYNTAX_ERR = 8;

/**
 * FileMetadata (Metadata) interface
 * http://www.w3.org/TR/file-system-api/#idl-def-Metadata
 */
function FileMetadata() {
}

FileMetadata.cast = function( p_modificationDate ) {
        var md = new FileMetadata();
        md.modificationTime = p_modificationDate;

        return md;
}
FileMetadata.prototype.modificationTime = null;

/**
 * Entry interface
 * http://www.w3.org/TR/file-system-api/#idl-def-Entry
 */
function Entry() {
}
Entry.prototype.isFile = false;
Entry.prototype.isDirectory = false;
Entry.prototype.name = "";
Entry.prototype.fullPath = "";
Entry.prototype.filesystem = null;
Entry.prototype.getMetadata = function( successCallback, errorCallback ) {
        // Get metadata for this entry
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "getMetadata", [this.fullPath]);
}
Entry.prototype.moveTo = function( parent, newName, successCallback, errorCallback ) {
}
Entry.prototype.copyTo = function( parent, newName, successCallback, errorCallback ) {
}
Entry.prototype.toURL = function( mimeType ) {
        return "file://" + this.fullPath;
}
Entry.prototype.remove = function( successCallback, errorCallback ) {
        // Remove this entry
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "remove", [this.fullPath]);
}
Entry.prototype.getParent = function( successCallback, errorCallback ) {
        // Ask the system for our parent
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "getParent", [this.fullPath]);
}

/**
 * FileInfo (File) interface
 * (had to call the 'File' object FileInfo since there were name conflicts)
 * http://dev.w3.org/2006/webapi/FileAPI/#dfn-file
 */
function FileInfo() {
}

FileInfo.cast = function( p_name, p_fullPath, p_type, p_lastModifiedDate, p_size ) {
        var f = new FileInfo();
        f.name = p_name;
        f.fullPath = p_fullPath;
        f.type = p_type;
        f.lastModifiedDate = p_lastModifiedDate;
        f.size = p_size;

        return f;
}
FileInfo.prototype.name = "";
FileInfo.prototype.fullPath = "";
FileInfo.prototype.type = "unknown/unknown";
FileInfo.prototype.lastModifiedDate = null;
FileInfo.prototype.size = 0;

/**
 * FileSaver interface
 * http://www.w3.org/TR/file-writer-api/#idl-def-FileSaver
 */
function FileSaver() {
}

FileSaver.createEvent = function( p_type, p_target ) {
        var evt = {
                "type": p_type,
                "target": p_target
        };

        return evt;
}

FileSaver.prototype.abort = function() {
        if( this.readyState == FileSaver.INIT || this.readyState == FileSaver.DONE ) throw FileException.cast(FileException.INVALID_STATE_ERR);

        this.error = FileError.cast(FileError.ABORT_ERR);
        this.readyState = FileSaver.DONE;

        if( typeof this.onerror === "function" ) this.onerror(FileSaver.createEvent("error", this));
        if( typeof this.onabort === "function" ) this.onabort(FileSaver.createEvent("abort", this));
        if( typeof this.onwriteend === "function" ) this.onwriteend(FileSaver.createEvent("writeend", this));
}

FileSaver.INIT = 0;
FileSaver.WRITING = 1;
FileSaver.DONE = 2;

FileSaver.prototype.readyState = FileSaver.INIT;
FileSaver.prototype.error = new FileError();
FileSaver.prototype.onwritestart = null;
FileSaver.prototype.onprogress = null;
FileSaver.prototype.onwrite = null;
FileSaver.prototype.onabort = null;
FileSaver.prototype.onerror = null;
FileSaver.prototype.onwriteend = null;

FileSaver.prototype.fullPath = "";		// Not W3C conform, but we need the path for handling!

/**
 * FileWriter interface
 * (derives from FileSaver)
 * http://www.w3.org/TR/file-writer-api/#idl-def-FileWriter
 */
function FileWriter() {
}

FileWriter.cast = function( p_fullPath, p_length ) {
        var fw = new FileWriter();
        fw.fullPath = p_fullPath;
        fw.length = p_length;

        return fw;
}

FileWriter.prototype = new FileSaver();
FileWriter.prototype.position = 0;
FileWriter.prototype.length = 0;
FileWriter.prototype.write = function( data ) {
        //console.log( 'Calling write: ' + this.position + " / " + this.length );

        // Check if we are able to write
        if( this.readyState == FileSaver.WRITING ) throw FileException.cast(FileException.INVALID_STATE_ERR);
        // We are writing now
        this.readyState = FileSaver.WRITING;
        // Emit the start event
        if( typeof this.onwritestart === "function" ) this.onwritestart( FileSaver.createEvent("writestart", this) );

        // Finally do the writing
        var me = this;
        Cordova.exec(function(p_position, p_length) {
                // Update position & length for file
                me.position = p_position;
                me.length = p_length;

                // Update state
                me.readyState = FileSaver.DONE;

                // Dispatch missing events
                if( typeof me.onwrite === "function" ) me.onwrite( FileSaver.createEvent("write", me) );
                if( typeof me.onwriteend === "function" ) me.onwriteend( FileSaver.createEvent("writeend", me) );
        }, function( p_fileError, p_position, p_length ) {
                // Update position & length for file
                me.position = p_position;
                me.length = p_length;

                // Set error object & update state
                me.error = p_fileError;
                me.readyState = FileSaver.DONE;

                // Dispatch missing events
                if( typeof me.onerror === "function" ) me.onerror( FileWriter.createEvent("error", me) );
                if( typeof me.onwriteend === "function" ) me.onwriteend( FileWriter.createEvent("writeend", me) );
        }, "com.cordova.File", "write", [this.fullPath, this.position, data]);
}

FileWriter.prototype.seek = function( offset ) {
        //console.log( 'Calling seek: ' + offset + " / " + this.position + " / " + this.length + " / " + this.readyState );

        if( this.readyState == FileSaver.WRITING ) throw FileException.cast(FileException.INVALID_STATE_ERR);

        if( offset < 0 ) {
                this.position = offset + this.length;
        }
        else if( offset > this.length ) {
                this.position = this.length;
        }
        else {
                this.position = offset;
        }
}

FileWriter.prototype.truncate = function( size ) {
        // Check if we are able to write
        if( this.readyState == FileSaver.WRITING ) throw FileException.cast(FileException.INVALID_STATE_ERR);
        // We are writing now
        this.readyState = FileSaver.WRITING;
        // Emit the start event
        if( typeof this.onwritestart === "function" ) this.onwritestart( FileSaver.createEvent("writestart", this) );

        // Finally do the writing
        var me = this;
        Cordova.exec(function(p_position, p_length) {
                // Update position & length for file
                me.position = p_position;
                me.length = p_length;

                // Update state
                me.readyState = FileSaver.DONE;

                // Dispatch missing events
                if( typeof me.onwrite === "function" ) me.onwrite( FileSaver.createEvent("write", me) );
                if( typeof me.onwriteend === "function" ) me.onwriteend( FileSaver.createEvent("writeend", me) );
        }, function( p_fileError, p_position, p_length ) {
                // Update position & length for file
                me.position = p_position;
                me.length = p_length;

                // Set error object & update state
                me.error = p_fileError;
                me.readyState = FileSaver.DONE;

                // Dispatch missing events
                if( typeof me.onerror === "function" ) me.onerror( FileSaver.createEvent("error", me) );
                if( typeof me.onwriteend === "function" ) me.onwriteend( FileSaver.createEvent("writeend", me) );
        }, "com.cordova.File", "truncate", [this.fullPath, size]);
}

/**
 * FileReader interface
 * http://www.w3.org/TR/FileAPI/#dfn-filereader
 */
function FileReader() {
}

FileReader.EMPTY = 0;
FileReader.LOADING = 1;
FileReader.DONE = 2;

FileReader.prototype.readyState = FileReader.EMPTY;
FileReader.prototype.result = "";
FileReader.prototype.error = new FileError();
FileReader.prototype.onloadstart = null;
FileReader.prototype.onprogress = null;
FileReader.prototype.onload = null;
FileReader.prototype.onabort = null;
FileReader.prototype.onerror = null;
FileReader.prototype.onloadend = null;

FileReader.prototype.readAsArrayBuffer = function( file ) {
}
FileReader.prototype.readAsBinaryString = function( file ) {
}
FileReader.prototype.readAsText = function( file ) {
        this.readyState = FileReader.EMPTY;
        this.result = null;

        this.readyState = FileReader.LOADING;

        if( typeof this.onloadstart === "function" ) this.onloadstart( FileSaver.createEvent( "loadstart", this) );

        var me = this;

        // Lets read the file...
        Cordova.exec(function( p_data ) {
                me.readyState = FileReader.DONE;
                me.result = atob( p_data );

                if( typeof me.onload === "function" ) me.onload( FileSaver.createEvent( "load", me) );
                if( typeof me.onloadend === "function" ) me.onloadend( FileSaver.createEvent( "loadend", me) );
        }, function( p_fileError ) {
                me.readyState = FileReader.DONE;
                me.result = null;
                me.error = p_fileError;

                if( typeof me.onloadend === "function" ) me.onloadend( FileSaver.createEvent( "loadend", me) );
                if( typeof me.onerror === "function" ) me.onerror( FileSaver.createEvent( "error", me) );
        }, "com.cordova.File", "readAsDataURL", [file.fullPath]);
}
FileReader.prototype.readAsDataURL = function( file ) {
        this.readyState = FileReader.EMPTY;
        this.result = null;

        this.readyState = FileReader.LOADING;

        if( typeof this.onloadstart === "function" ) this.onloadstart( FileSaver.createEvent( "loadstart", this) );

        var me = this;

        // Lets read the file...
        Cordova.exec(function( p_data ) {
                me.readyState = FileReader.DONE;
                me.result = p_data;

                if( typeof me.onloadend === "function" ) me.onloadend( FileSaver.createEvent( "loadend", me) );
        }, function( p_fileError ) {
                me.readyState = FileReader.DONE;
                me.result = null;
                me.error = p_fileError;

                if( typeof me.onloadend === "function" ) me.onloadend( FileSaver.createEvent( "loadend", me) );
                if( typeof me.onerror === "function" ) me.onerror( FileSaver.createEvent( "error", me) );
        }, "com.cordova.File", "readAsDataURL", [file.fullPath]);
}
FileReader.prototype.abort = function() {
        this.readyState = FileReader.DONE;
        this.result = null;
        this.error = FileError.cast( FileError.ABORT_ERR );

        if( typeof this.onerror === "function" ) this.onerror( FileSaver.createEvent( "error", me) ) ;
        if( typeof this.onabort === "function" ) this.onabort( FileSaver.createEvent( "abort", me) ) ;
        if( typeof this.onloadend === "function" ) this.onloadend( FileSaver.createEvent( "loadend", me) ) ;
}


/**
 * FileEntry interface
 * (derives from Entry)
 * http://www.w3.org/TR/file-system-api/#the-fileentry-interface
 */
function FileEntry() {
        this.isFile = true;
        this.isDirectory = false;
}

FileEntry.cast = function( filename, path ) {
        var fe = new FileEntry();
        fe.name = filename;
        fe.fullPath = path;

        return fe;
}

FileEntry.prototype = new Entry();
FileEntry.prototype.createWriter = function( successCallback, errorCallback ) {
        this.file( function(p_file) {
                successCallback(FileWriter.cast(p_file.fullPath, p_file.size));
        }, errorCallback);
}
FileEntry.prototype.file = function( successCallback, errorCallback ) {
        // Lets get the fileinfo
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "file", [this.fullPath]);
}

/**
 * DirectoryReader interface
 * http://www.w3.org/TR/file-system-api/#the-directoryreader-interface
 */
function DirectoryReader() {
}

DirectoryReader.cast = function( p_fullPath ) {
        var dr = new DirectoryReader();
        dr.fullPath = p_fullPath;

        return dr;
}

DirectoryReader.prototype.fullPath = "";	// Not W3C conform, but required
DirectoryReader.prototype.readEntries = function( successCallback, errorCallback ) {
        // Get all entries for the directory
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "readEntries", [this.fullPath]);
}

/**
 * DirectoryEntry interface
 * (derives from Entry)
 * http://www.w3.org/TR/file-system-api/#the-directoryentry-interface
 */
function DirectoryEntry() {
        this.isFile = false;
        this.isDirectory = true;
}

DirectoryEntry.cast = function( dirname, path ) {
        var de = new DirectoryEntry();
        de.name = dirname;
        de.fullPath = path;

        return de;
}

DirectoryEntry.prototype = new Entry();
DirectoryEntry.prototype.createReader = function() {
        return DirectoryReader.cast(this.fullPath);
}
DirectoryEntry.prototype.getFile = function( path, options, successCallback, errorCallback ) {
        var requestPath = path;

        // Check for a relative path
        if( requestPath.charAt(0) != '/' ) requestPath = this.fullPath + requestPath;

        // Lets get the file
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "getFile", [requestPath, options]);
}
DirectoryEntry.prototype.getDirectory = function( path, options, successCallback, errorCallback ) {
        var requestPath = path;

        // Check for a relative path
        if( requestPath.charAt(0) != '/' ) requestPath = this.fullPath + requestPath;
        // Make sure we have a trailing slash
        if( requestPath.charAt(requestPath.length - 1) != '/' ) requestPath = requestPath + "/";

        // Lets get the directory
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "getDirectory", [requestPath, options]);
}
DirectoryEntry.prototype.removeRecursively = function( successCallback, errorCallback ) {
        // Remove the directory
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "removeRecursively", [this.fullPath]);
}

/**
 * FileSystem interface
 * http://www.w3.org/TR/file-system-api/#the-filesystem-interface
 */
function FileSystem() {
}

FileSystem.cast = function( fsname, dirname, path ) {
        var fs = new FileSystem();
        fs.name = fsname;
        fs.root = DirectoryEntry.cast(dirname, path);

        return fs;
}

FileSystem.prototype.name = "";
FileSystem.prototype.root = null;	// Should be a DirectoryEntry

/**
 * LocalFileSystem interface
 * http://www.w3.org/TR/file-system-api/#using-localfilesystem
 */
function LocalFileSystem() {
}

LocalFileSystem.TEMPORARY = 0;
LocalFileSystem.PERSISTENT = 1;

LocalFileSystem.prototype.requestFileSystem = function( type, size, successCallback, errorCallback ) {
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "requestFileSystem", [type]);
}
LocalFileSystem.prototype.resolveLocalFileSystemURL = function( url, successCallback, errorCallback ) {
        Cordova.exec(successCallback, errorCallback, "com.cordova.File", "resolveLocalFileSystemURL", [url]);
}

/**
 * Let window implement the localfilesystem
 */
Cordova.addConstructor( "com.cordova.File", function () {
    var localFileSystem = new LocalFileSystem();
    window.requestFileSystem = localFileSystem.requestFileSystem;
    window.resolveLocalFileSystemURI = localFileSystem.resolveLocalFileSystemURL;
} );
