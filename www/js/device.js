/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
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
