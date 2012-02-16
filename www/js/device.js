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

function Device() {
    var me = this;
    Cordova.exec( function( p_name, p_cordova, p_platform, p_uuid, p_version ) {
                      me.name = p_name;
                      me.cordova = p_cordova;
                      me.platform = p_platform;
                      me.uuid = p_uuid;
                      me.version = p_version;
                  }, null, "com.cordova.Device", "getInfo", [] );
};

Device.prototype.name = "";
Device.prototype.cordova = "";
Device.prototype.platform = "";
Device.prototype.uuid = "";
Device.prototype.version = "";

Cordova.addConstructor( "com.cordova.Device", function() {
                            window.device = new Device();
                        } );
