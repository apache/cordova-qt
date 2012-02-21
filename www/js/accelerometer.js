/*
 *  Copyright 2011  Integrated Computer Solutions - http://www.ics.com
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

//accelerometer interface http://docs.phonegap.com/en/1.0.0/phonegap_accelerometer_accelerometer.md.html


function Acceleration() {
};

Acceleration.cast = function( p_acceleration) {
    var acceleration = new Acceleration();
    acceleration.x = p_acceleration.x;
    acceleration.y = p_acceleration.y;
    acceleration.z = p_acceleration.z;
    acceleration.timestamp = p_acceleration.timestamp;
    return acceleration;
};

Acceleration.prototype.x = null;
Acceleration.prototype.y = null;
Acceleration.prototype.z = null;
Acceleration.prototype.timestamp = null;

Accelerometer.prototype.watchIds = [];

/**
 * Accelerometer interface
 */
function Accelerometer() {
};


Accelerometer.prototype.getCurrentAcceleration = function( successCallback, errorCallback) {
    // Check the callbacks
    if( typeof successCallback !== "function" ) return;
    if( typeof errorCallback !== "function" ) errorCallback = function() {};

    // Call the native function and query for the new x,y,z accel values
    var me = this;
    Cordova.exec( function( p_acceleration ) {
                      successCallback( p_acceleration );
                  }, errorCallback, "com.cordova.Accelerometer", "getCurrentAcceleration", [ {} ] );
};

Accelerometer.prototype.watchAcceleration = function( successCallback, errorCallback, options ) {
            // Check the callbacks
            if( typeof successCallback !== "function" ) return;
            if( typeof errorCallback !== "function" ) errorCallback = function() {};

            var watchId = this.watchIds.length + 1; // +1 in order to avoid 0 as watchId
            this.watchIds[watchId] = true;
            var me = this;

            var frequency=10000;

            if(options.frequency != null){frequency=options.frequency;}

            function doWatch() {
                me.getCurrentAcceleration( function( p_acceleration ) {
                                            if( !me.watchIds[watchId] ) return;

                                            successCallback( p_acceleration );

                                            // Wait some time before starting again
                                            setTimeout( doWatch, frequency );
                                        }, function( p_accelerationError ) {
                                            if( !me.watchIds[watchId] ) return;

                                            errorCallback( p_accelerationError );
                                            // Wait some time before starting again
                                            setTimeout( doWatch, frequency );
                                        });
            }

            setTimeout( doWatch, frequency );

            return watchId;
        };

Accelerometer.prototype.clearWatch = function( watchId ) {
            this.watchIds[watchId] = false;
};

/**
 * Add the accelerometer object to the navigator
 */
Cordova.addConstructor( "com.cordova.Accelerometer", function () {
                            navigator.accelerometer = new Accelerometer();
                        } );
