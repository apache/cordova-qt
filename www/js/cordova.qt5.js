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

Cordova.Qt = {};

/**
 * Execute a call to a plugin function
 * @return bool true on success, false on error (e.g. function doesn't exist)
 */
Cordova.Qt.exec = function( successCallback, errorCallback, pluginName, functionName, parameters ) {
    // Check if plugin is enabled
    if( Cordova.plugins[pluginName] !== true ) {
        return false;
    }

    // Store a reference to the callback functions
    var scId = Cordova.callbacks.length;
    var ecId = scId + 1;
    Cordova.callbacks[scId] = successCallback;
    Cordova.callbacks[ecId] = errorCallback;

    parameters.unshift( ecId );
    parameters.unshift( scId );

    navigator.qt.postMessage(JSON.stringify({messageType: "callPluginFunction", plugin: pluginName, func: functionName, params: parameters}))

    return true;
}

Cordova.exec = Cordova.Qt.exec;

navigator.qt.onmessage = function(ev) {
            var received = eval('('+ev.data+')')
            if (typeof received == 'undefined')
                return false
            if (typeof received.messageType == 'undefined')
                return false
            if (received.messageType == "evalJS") {
                if (typeof received.jsData == 'undefined')
                    return false
                eval(received.jsData)
            }
            return true
        }
