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
PhoneGap.addConstructor( "com.phonegap.Notification", function () {
                            navigator.notification = new Notification();
                        } );
