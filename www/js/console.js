/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

function Console() {
}

Console.prototype.log = function( p_message ) {
    PhoneGap.exec( null, null, "com.phonegap.Console", "log", [p_message] );
}

PhoneGap.addConstructor( "com.phonegap.Console", function() {
                            window.console = new Console();
                        } );
