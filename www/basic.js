/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

/*
 * JavaScript file
 */


//TODO: remove later this method. Now it is needed for debugging
function dump(arr,level) {
    var dumped_text = "";
    if(!level) level = 0;

    //The padding given at the beginning of the line.
    var level_padding = "";
    for(var j=0;j<level+1;j++) level_padding += "    ";

    if(typeof(arr) == 'object') { //Array/Hashes/Objects
        for(var item in arr) {
            var value = arr[item];

            if(typeof(value) == 'object') { //If it is an array,
                dumped_text += level_padding + "'" + item + "' ... :\n";
                dumped_text += dump(value,level+1);
            } else {
                dumped_text += level_padding + "'" + item + "' => \"" + value + "\";\n";
            }
        }
    } else { //Stings/Chars/Numbers etc.
        dumped_text = "===>"+arr+"<===("+typeof(arr)+")";
    }
    return dumped_text;
}

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
                                       if ( button === 1) {
                                           alert('User input: YES');
                                       } else if( button === 2) {alert('User input: No');}
                                   }, "title", "buttonName");
}


function getCurrentPosition() {
    console.log("clicked position");
    navigator.geolocation.getCurrentPosition( function( position ) {
//                                                 get( "position_val" ).innerHTML = position.coords.latitude + " / " + position.coords.longitude;
                                                   get( "position_val" ).innerHTML = " success ";
                                             },
                                             function( error ) {
//                                                 get( "position_val" ).innerHTML = error.code + ": "  + error.message;
                                                 get( "position_val" ).innerHTML = "error";
                                             },
                                             { maximumAge: 3000, timeout: 5000, enableHighAccuracy: true } );
}

function getCurrentHeading() {
    navigator.compass.getCurrentHeading( function( heading ) {
                                            get( "heading_val" ).innerHTML = heading.magneticHeading;
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

function getPicture(){
    navigator.camera.getPicture(function(picture_file){
                                    console.log("getPicture succeed callback: "+picture_file)
                                    get("picture_val").innerHTML = '<img width="200" src="' + picture_file +'" />';
                                },
                                function(){
                                    console.log("getPicture error callback")
                                    get("picture_val").innerHTML = 'Capture cancelled or error occured'
                                },
                                { quality : 75,
                                  destinationType : Camera.DestinationType.DATA_URL,
                                  sourceType : Camera.PictureSourceType.CAMERA,
                                  allowEdit : true,
                                  encodingType: Camera.EncodingType.JPEG,
                                  targetWidth: 100,
                                  targetHeight: 100});
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


function createTestContact() {
    var created = navigator.contacts.create({"name": {familyName: "Family", givenName: "Given"}, phoneNumbers: [{"value": "+123456789", pref: false, type: "work"}], emails: [{"value": "given.family@gmail.com", pref: false, type: "email"}, {"value": "given@family.com", pref: false, type: "email"}], birthday: new Date(1985, 4, 3, 0, 0, 0)})
    created.save(function() {
                     get("create_contact_result").innerHTML = "Contact created"
                 },
                 function(error) {
                     get("create_contact_result").innerHTML = "Error occured: " + error
                 })
}

function searchForTestContact() {
    navigator.contacts.find(["name", "phoneNumbers", "nickname", "displayName", "emails", "ims", "addresses", "organizations", "birthday", "photos"],
                            function(contacts) {
                                var result = ""
                                for (var contact in contacts) {
                                    result += contacts[contact].name.formatted + ": " + contacts[contact].phoneNumbers[0].value + ", " + contacts[contact].emails[0].value + "<br />"
                                }
                                get("search_contact_result").innerHTML = result
                            },
                            function(error) {
                                get("search_contact_result").innerHTML = "Error occured: " + error
                            },
                            {filter:"Given", multiple: true})
}


function removeTestContact() {
    get("remove_contact_result").innerHTML = ""
    navigator.contacts.find(["name"],
                            function(contacts){
                                for (var contact in contacts) {
                                    contacts[contact].remove(function() {
                                                                 get("remove_contact_result").innerHTML += "Contact removed; "
                                                             },
                                                             function(error) {
                                                                 get("remove_contact_result").innerHTML += "Error occured: " + error + "; "
                                                             })
                                }
                            },
                            0, {filter:"Given", multiple: true})
}


function mediaOpen(){
    console.log("opened media file...");
    navigator.media.openMedia("testFile.mp3");
    console.log("finished media file");
    var position=0;
    var duration=0;

    navigator.media.getDuration( function( dur ) {
                                                 duration=dur;
                                             },
                                             function( error ) {
                                             });

    // Update media position every second
        var mediaTimer = setInterval(function() {
            // get media position
            navigator.media.getCurrentPosition(
                // success callback
                function(position) {
                    if (position > -1) {
                        navigator.media.getCurrentPosition( function( pos) {
                                                                     position=pos;
                                                                 },
                                                                 function( error ) {
                                                                 });
                        get("media_position_duration_val").innerHTML = position + " / " + duration;
                    }
                },
                // error callback
                function(e) {
                    console.log("Error getting pos=" + e);
                }
            );
        }, 1000);
}

function mediaPlay(){
    navigator.media.play();
}
function mediaPause(){
    navigator.media.pause();
}
function mediaStop(){
    navigator.media.stop();
}
function mediaStartRecording(){
    navigator.media.startRecord();
}
function mediaStopRecording(){
    navigator.media.stopRecord();
}
function mediaFF5sec(){
    var jumpPosition=navigator.media.getCurrentPosition() + 5000;
    if( jumpPosition < mediaTest.getDuration())
        navigator.media.seekTo(jumpPosition);
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
