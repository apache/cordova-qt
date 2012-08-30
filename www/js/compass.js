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

//compass interface http://docs.cordova.io/en/2.0.0/cordova_compass_compass.md.html

function CompassHeading(p_heading, p_trueHeading, p_accuracy, timestamp){
                   this.magneticHeading = p_heading || 0;
                   this.trueHeading = p_trueHeading || 0;
                   this.headingAccuracy = p_accuracy || 0;
                   this.timestamp = timestamp || 0;
return this;
};

CompassHeading.prototype.magneticHeading = 0;
CompassHeading.prototype.trueHeading = 0;
CompassHeading.prototype.headingAccuracy = 0;
CompassHeading.prototype.timestamp = 0;

CompassHeading.cast = function(heading, trueHeading, accuracy, timestamp){
                   var that = new CompassHeading(heading, trueHeading, accuracy, timestamp);
return that;
};


//CompassError

function CompassError() {
};

CompassError.cast = function( p_code, p_message ) {
            var CompassError = new CompassError();
            CompassError.code = p_code;
            CompassError.message = p_message;

            return CompassError;
        };

CompassError.COMPASS_INTERNAL_ERR = 0;
CompassError.COMPASS_NOT_SUPPORTED = 20;

CompassError.prototype.code = 0;
CompassError.prototype.message = "";

/**
 * HeadingOptions interface
 */
function HeadingOptions() {
};

/**
 * Compass interface
 */
function Compass() {
};

Compass.prototype.watchIds = [];
Compass.prototype.cachedHeading = null;

Compass.prototype.getCurrentHeading = function( successCallback, errorCallback, options ) {
            // Check the callbacks
            if( typeof successCallback !== "function" ) return;
            if( typeof errorCallback !== "function" ) errorCallback = function() {};
            var headingOptions = new HeadingOptions();

            //    // Check the timestamp
            //    if( this.cachedHeading !== null &&
            //            (this.cachedHeading.timestamp <= (new Date()).getTime) ) {
            //        successCallback( this.cachedHeading );
            //        return;
            //    }

            // Call the native function and query for a new heading
            var me = this;
            Cordova.exec( function( p_heading ) {
                             received = true;
                             me.cachedHeading = p_heading;
                             successCallback( p_heading );
                         }, errorCallback, "com.cordova.Compass", "getCurrentHeading", [ headingOptions ] );
            return me.cachedHeading;
        };

Compass.prototype.watchHeading = function( successCallback, errorCallback, options ) {
            // Check the callbacks
            if( typeof successCallback !== "function" ) return;
            if( typeof errorCallback !== "function" ) errorCallback = function() {};

            var watchId = this.watchIds.length + 1; // +1 in order to avoid 0 as watchId
            this.watchIds[watchId] = true;
            var me = this;

            function doWatch() {
                me.getCurrentHeading( function( p_heading ) {
                                         if( !me.watchIds[watchId] ) return;
                                         successCallback( p_heading );
                                         // Wait some time before starting again
                                         setTimeout( doWatch, 100 );
                                     }, function( p_headingError ) {
                                         if( !me.watchIds[watchId] ) return;

                                         errorCallback( p_headingError );
                                         // Wait some time before starting again
                                         setTimeout( doWatch, 100 );
                                     }, options );
            }

            // Start watching for heading changes (slight delay, in order to simulate asynchronous behaviour)
            setTimeout( doWatch, 100 );

            return watchId;
        };

Compass.prototype.clearWatch = function( watchId ) {
            this.watchIds[watchId] = false;
        };

/**
 * Add the compass object to the navigator
 */
Cordova.addConstructor( "com.cordova.Compass", function () {
                           navigator.compass = new Compass();} );
