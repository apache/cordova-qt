/*
 * JavaScript file
 */

var vibration_length = 1000;

function get(id)
{
	return document.getElementById(id);
}

function init() 
{
    navigator.accelerometer.watchAcceleration(function (v) {
        get("accel_val").innerHTML = v.x + '   ' + v.y + '    ' + v.z;
    }, null, {});
}

function ping_google() 
{
    navigator.network.isReachable('http://www.google.com', function (state) {
        if (state == NetworkStatus.NOT_REACHABLE) {
            alert('Google not found');
        } else {
            alert('Google says: Pong!');
        }
    }, {});
}

function test_vibra()
{
	navigator.notification.vibrate(vibration_length);
    navigator.notification.beep(5);
}

function getCurrentPosition() {
    navigator.geolocation.getCurrentPosition( function( position ) {
                                                 get( "position_val" ).innerHTML = position.coords.latitude + " / " + position.coords.longitude;
                                             },
                                             function( error ) {
                                                 get( "position_val" ).innerHTML = error.code + ": "  + error.message;
                                             },
                                             { maximumAge: 3000, timeout: 5000, enableHighAccuracy: true } );
}

function test_requestFileSystem() {
    window.requestFileSystem( LocalFileSystem.PERSISTENT, 0, function( p_fileSystem ) {
                                 get( "debug_output" ).innerHTML = p_fileSystem.name + " / " + p_fileSystem.root.fullPath;


                                 p_fileSystem.root.getFile( "test.txt", { create: true, exclusive: false }, function( p_fileEntry) {
                                                               p_fileEntry.file( function( p_file ) {
                                                                                    var fileReader = new FileReader();
                                                                                    fileReader.readAsText( p_file );
                                                                                }, fileError );
                                                           }, fileError );

                                 /*p_fileSystem.root.getFile( "test.txt", { create: true, exclusive: false }, function( p_fileEntry) {
                                                               p_fileEntry.getMetadata( function( p_metadata ) {
                                                                                           get( "debug_output" ).innerHTML = p_metadata.modificationTime.toLocaleString();
                                                                                       }, fileError );
                                                           }, fileError );*/


                                 /*p_fileSystem.root.getDirectory( "test", { create: true, exclusive: false }, function( p_directoryEntry ) {
                                                                    p_directoryEntry.remove();
                                                                }, fileError );

                                 p_fileSystem.root.getFile( "test.txt", { create: true, exclusive: false }, function( p_fileEntry) {
                                                               p_fileEntry.remove();
                                                           }, fileError );*/

                                 /*p_fileSystem.root.getParent( function( p_directoryEntry ) {
                                                                 get( "debug_output" ).innerHTML = p_directoryEntry.fullPath;
                                                             }, fileError );*/

                                 /*p_fileSystem.root.getFile( "test.txt", { create: true, exclusive: false }, function( p_fileEntry ) {
                                                               p_fileEntry.createWriter( function( p_fileWriter ) {
                                                                                            p_fileWriter.onwriteend = function() {
                                                                                                p_fileWriter.onwriteend = function() {
                                                                                                    p_fileWriter.onwriteend = null;
                                                                                                    p_fileWriter.truncate( 5 );
                                                                                                };
                                                                                                p_fileWriter.write( " - test2" );
                                                                                            };
                                                                                            p_fileWriter.write( "test" );
                                                                                        }, fileError );
                                                           }, fileError );*/

                                 /*window.resolveLocalFileSystemURI( p_fileSystem.root.toURL(), function( p_entry ) {
                                                                      get("debug_output").innerHTML = p_entry.fullPath;
                                                                  }, function() {} );*/

                                 /*var dirReader = p_fileSystem.root.createReader();
                                 dirReader.readEntries( function( p_dirEntries ) {
                                                           get( "debug_output" ).innerHTML = "";
                                                           for( var i = 0; i < p_dirEntries.length; i++ ) {
                                                               var currEntry = p_dirEntries[i];
                                                               get( "debug_output" ).innerHTML += " " + currEntry.name;
                                                           }
                                                       }, function() {} );*/
                                 /*p_fileSystem.root.getDirectory( "test", { create: true, exclusive: false }, function( p_directoryEntry ) {
                                                                    p_directoryEntry.removeRecursively( function() {}, function() {} );
                                                                }, function() {} );*/
                             }, function() {} );
}

function fileError( p_fileError ) {
    get("debug_output").innerHTML = p_filerError.code;
}

/*
 * Register for the device ready event
 */
document.addEventListener( "deviceready", function() {
                              get( "debug_output" ).innerHTML = "Device Ready!";
                          }, false );

