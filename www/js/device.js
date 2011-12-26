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
    PhoneGap.exec( function( p_name, p_phonegap, p_platform, p_uuid, p_version ) {
                      me.name = p_name;
                      me.phonegap = p_phonegap;
                      me.platform = p_platform;
                      me.uuid = p_uuid;
                      me.version = p_version;
                  }, null, "com.phonegap.Device", "getInfo", [] );
};

Device.prototype.name = "";
Device.prototype.phonegap = "";
Device.prototype.platform = "";
Device.prototype.uuid = "";
Device.prototype.version = "";

PhoneGap.addConstructor( "com.phonegap.Device", function() {
                            window.device = new Device();
                        } );
