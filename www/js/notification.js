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
 * http://docs.cordova.io/en/2.0.0/cordova_notification_notification.md.html
 */
function Notification() {
}

Notification.Callback = function() {};

Notification.prototype.alert = function( message, alertCallback, title, buttonNamne ) {
            alert( message );
            alertCallback();
        }

Notification.prototype.confirm = function( message, confirmCallback, title, buttonLabels ) {
            Notification.Callback = confirmCallback;
            window.qmlWrapper.callConfirm(message);
        }

Notification.prototype.beep = function( times ) {
            Cordova.exec( null, null, "com.cordova.Notification", "beep", [times] );
        }

Notification.prototype.vibrate = function( milliseconds ) {
            Cordova.exec( null, null, "com.cordova.Notification", "vibrate", [milliseconds] );
        }

/**
 * Add the notification object to the navigator
 */
Cordova.addConstructor( "com.cordova.Notification", function () {
                           navigator.notification = new Notification();
                       } );
