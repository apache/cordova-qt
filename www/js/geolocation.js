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
 * Coordinates interface - http://www.w3.org/TR/geolocation-API/#coordinates
 */
function Coordinates() {
};

Coordinates.cast = function( p_latitude, p_longitude, p_altitude, p_accuracy, p_altitudeAccuracy, p_heading, p_speed ) {
    var coordinates = new Coordinates();

    coordinates.latitude = p_latitude;
    coordinates.longitude = p_longitude;
    coordinates.altitude = p_altitude;
    coordinates.accuracy = p_accuracy;
    coordinates.altitudeAccuracy = p_altitudeAccuracy;
    coordinates.heading = p_heading;
    coordinates.speed = p_speed;

    return coordinates;
};

Coordinates.prototype.latitude = 0;
Coordinates.prototype.longitude = 0;
Coordinates.prototype.altitude = 0;
Coordinates.prototype.accuracy = 0;
Coordinates.prototype.altitudeAccuracy = 0;
Coordinates.prototype.heading = 0;
Coordinates.prototype.speed = 0;

/**
 * Position interface - http://www.w3.org/TR/geolocation-API/#position
 */
function Position() {
};

Position.cast = function( p_coords, p_timestamp ) {
    // The timestamp is optional and can be auto-generated on creation
    if( typeof p_timestamp == "undefined" ) p_timestamp = (new Date()).getMilliseconds();

    var position = new Position();

    position.coords = p_coords;
    position.timestamp = p_timestamp;

    return position;
};

Position.prototype.coords = null;
Position.prototype.timestamp = 0;

/**
 * PositionError interface - http://www.w3.org/TR/geolocation-API/#position-error
 */
function PositionError() {
};

PositionError.cast = function( p_code, p_message ) {
    var positionError = new PositionError();
    positionError.code = p_code;
    positionError.message = p_message;

    return positionError;
};

PositionError.PERMISSION_DENIED = 1;
PositionError.POSITION_UNAVAILABLE = 2;
PositionError.TIMEOUT = 3;

PositionError.prototype.code = 0;
PositionError.prototype.message = "";

/**
 * PositionOptions interface - http://www.w3.org/TR/geolocation-API/#position_options_interface
 */
function PositionOptions() {
};

PositionOptions.prototype.enableHighAccuracy = false;
PositionOptions.prototype.timeout = -1;  // Timeout by default negative, which means no timeout
PositionOptions.prototype.maximumAge = 0;

/**
 * Geolocation interface - http://www.w3.org/TR/geolocation-API/#geolocation_interface
 */
function Geolocation() {
};

Geolocation.prototype.watchIds = [];
Geolocation.prototype.cachedPosition = null;

Geolocation.prototype.getCurrentPosition = function( successCallback, errorCallback, options ) {
    // Check the callbacks
    if( typeof successCallback !== "function" ) return;
    if( typeof errorCallback !== "function" ) errorCallback = function() {};

    // This is a workaround as we allow passing any object in as options (for convenience)
    var positionOptions = new PositionOptions();
    if( typeof options.maximumAge != "undefined" && options.maximumAge > 0 ) positionOptions.maximumAge = options.maximumAge;
    if( typeof options.timeout != "undefined" ) {
        if( options.timeout > 0 ) {
            positionOptions.timeout = options.timeout;
        }
        else {
            positionOptions.timeout = 0;
        }
    }
    if( typeof options.enableHighAccuracy != "undefined" ) positionOptions.enableHighAccuracy = options.enableHighAccuracy;

    // Check if the cached object is sufficient
    if( this.cachedPosition !== null && this.cachedPosition.timestamp > ((new Date()).getTime() - positionOptions.maximumAge) ) {
        successCallback( this.cachedPosition );
        return;
    }

    // Check if the timeout is 0, if yes invoke the ErrorCallback immediately
    if( positionOptions.timeout == 0 ) {
        errorCallback( PositionError.cast( PositionError.TIMEOUT, "Timeout" ) );
        return;
    }

    var timedOut = false;   // Flag for indicating a timeout
    var timeoutId = 0;   // Flag for indicating a successful location receive

    // Handle the timeout in javascript - no need for native handling
    if( options.timeout > 0 ) {
        timeoutId = window.setTimeout( function() {
                                          // Request timed out, set status and execute errorCallback
                                          timedOut = true;
                                          timeoutId = 0;
                                          errorCallback( PositionError.cast( PositionError.TIMEOUT, "Timeout" ) );
                          }, options.timeout );
    }

    // Call the native function and query for a new position
    var me = this;
    Cordova.exec( function( p_position ) {
                      received = true;

                      // Cancel timeout
                      if( timeoutId > 0 ) {
                          window.clearTimeout( timeoutId );
                      }

                      // Cache the new position
                      me.cachedPosition = p_position;

                      // Execute the successCallback if not timed out
                      if( !timedOut ) successCallback( p_position );
                  }, errorCallback, "com.cordova.Geolocation", "getCurrentPosition", [ positionOptions ] );
};

Geolocation.prototype.watchPosition = function( successCallback, errorCallback, options ) {
    // Check the callbacks
    if( typeof successCallback !== "function" ) return;
    if( typeof errorCallback !== "function" ) errorCallback = function() {};

    var watchId = this.watchIds.length + 1; // +1 in order to avoid 0 as watchId
    this.watchIds[watchId] = true;
    var me = this;

    function doWatch() {
        me.getCurrentPosition( function( p_position ) {
                                    if( !me.watchIds[watchId] ) return;

                                    successCallback( p_position );

                                    // Wait some time before starting again
                                    setTimeout( doWatch, 100 );
                                }, function( p_positionError ) {
                                    if( !me.watchIds[watchId] ) return;

                                    errorCallback( p_positionError );
                                    // Wait some time before starting again
                                    setTimeout( doWatch, 100 );
                                }, options );
    }

    // Start watching for position changes (slight delay, in order to simulate asynchronous behaviour)
    setTimeout( doWatch, 100 );

    // Return watchId
    return watchId;
};

Geolocation.prototype.clearWatch = function( watchId ) {
    this.watchIds[watchId] = false;
};

/**
 * Add the geolocation object to the navigator
 */
Cordova.addConstructor( "com.cordova.Geolocation", function () {
                            navigator.geolocation = new Geolocation();
} );
