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
    }, null, {frequency:100});

}

function getCurrentConnectionType() {
    var networkState = navigator.network.connection.type;

    var states = {};
    states[Connection.UNKNOWN]  = 'Unknown connection';
    states[Connection.ETHERNET] = 'Ethernet connection';
    states[Connection.WIFI]     = 'WiFi connection';
    states[Connection.CELL_2G]  = 'Cell 2G connection';
    states[Connection.CELL_3G]  = 'Cell 3G connection';
    states[Connection.CELL_4G]  = 'Cell 4G connection';
    states[Connection.NONE]     = 'No network connection';
    get( "debug_output" ).innerHTML = states[networkState]
    console.log("network state = " + states[networkState])
}

function test_vibra()
{
    navigator.notification.vibrate(vibration_length);
    navigator.notification.beep(5);
}

function test_alert_confirm()
{
    //native javascript alert and confirm only support showing message,
    //title and button name are ignored.
    navigator.notification.alert("This is an alert.",
                                 function alertDismissed() {// do nothing here
                                 }, "title", "buttonName");
    navigator.notification.confirm("This is a confirm.",
                                   function onConfirm(button) {
                                       if ( button === 0) {
                                           alert('User input: No');
                                       } else {alert('User input: Yes');}
                                   }, "title", "buttonName");
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

function getCurrentHeading() {
    navigator.compass.getCurrentHeading( function( heading ) {
                                            get( "heading_val" ).innerHTML = heading.dir;
                                             },
                                             function( error ) {
                                                 get( "heading_val" ).innerHTML = error.code + ": "  + error.message;
                                             },
                                             {timestamp: (new Date()).getTime()} );
}

function getCurrentAcceleration() {
    navigator.accelerometer.getCurrentAcceleration( function( acceleration ) {
                                                 get("accel_val").innerHTML = acceleration.x + ' ' + acceleration.y + ' ' + acceleration.z;
                                             },
                                             function( error ) {
                                             });

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
                                              console.log("basicjs.deviceReady")
                                              get( "debug_output" ).innerHTML = "Device Ready!<br/>";
                                          }, false );

document.addEventListener( "resume", function() {
                                         console.log("basicjs.resume")
                                     }, false );

document.addEventListener( "pause", function() {
                                        console.log("basicjs.pause")
                                    }, false );

document.addEventListener( "offline", function() {
                                          console.log("basicjs.offline")
                                          get( "debug_output" ).innerHTML += "We are offline :(<br/>";
                                      }, false );

document.addEventListener( "online", function() {
                                         console.log("basicjs.online")
                                         get( "debug_output" ).innerHTML += "We are online :)<br/>";
                                     }, false );


document.addEventListener("batterycritical", function (info) {
                                                 console.log("basicjs.batteryCritical")
                                                 get( "debug_output" ).innerHTML = "Battery Level Critical " + info.level + "%<br/>";
                                             }, false)


document.addEventListener("batterylow", function (info) {
                                            console.log("basicjs.batteryLow")
                                            get( "debug_output" ).innerHTML = "Battery Level Low " + info.level + "%<br/>";
                                        }, false)

document.addEventListener("batterystatus", function (info) {
                                               console.log("basicjs.batteryStatus")
                                               get( "debug_output" ).innerHTML = "Battery Level Changed " + info.level + "%<br/>";
                                           }, false)

document.addEventListener("volumedownbutton", function () {
                                                  console.log("basicjs.volumeDownKeyPressed")
                                                  get( "debug_output" ).innerHTML = "Volume Down Button<br/>";
                                              }, false)

document.addEventListener("volumeupbutton", function () {
                                                console.log("basicjs.volumeUpKeyPressed")
                                                get( "debug_output" ).innerHTML = "Volume Up Button<br/>";
                                            }, false)
