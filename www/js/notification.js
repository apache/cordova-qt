/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

/**
 * Notification
 * http://docs.phonegap.com/phonegap_notification_notification.md.html
 */
function Notification() {
}

Notification.prototype.alert = function( message, alertCallback, title, buttonNamne ) {
    alert( message );
    alertCallback();
}

Notification.prototype.confirm = function( message, confirmCallback, title, buttonLabels ) {
    var result = confirm( message );
    if( result ) {
        result = 1;
    }
    else {
        result = 0;
    }
    confirmCallback(result);
}

Notification.prototype.beep = function( times ) {
    PhoneGap.exec( null, null, "com.phonegap.Notification", "beep", [times] );
}

Notification.prototype.vibrate = function( milliseconds ) {
    PhoneGap.exec( null, null, "com.phonegap.Notification", "vibrate", [milliseconds] );
}

/**
 * Add the notification object to the navigator
 */
PhoneGap.addConstructor( "com.phonegap.File", function () {
                            navigator.notification = new Notification();
                        } );
