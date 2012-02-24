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

function Connection() {
}

Connection.prototype.type = 1;
Connection.UNKNOWN = 1;
Connection.ETHERNET = 2;
Connection.WIFI = 3;
Connection.CELL_2G = 4;
Connection.CELL_3G = 5;
Connection.CELL_4G = 6;
Connection.NONE = 7;

/**
 * Internal function for listening to change events
 */
Connection.prototype.setChangeCallback = function() {
    var me = this;

    console.log( "connection - setChangeCallback" );

    Cordova.exec( function( p_type ) {
                      // Save new type
                      me.type = p_type;

                      console.log( "Connection - new type: " + p_type );

                      // Register new callback instance in order to receive future updates
                      setTimeout( navigator.network.connection.setChangeCallback, 100 );
                  }, null, "com.cordova.Connection", "setChangeCallback", [] );
}

Cordova.addConstructor( "com.cordova.Connection", function() {
                            if( typeof navigator.network === "undefined" ) navigator.network = {};

                            navigator.network.connection = new Connection();
                            navigator.network.connection.setChangeCallback();
                        } );
