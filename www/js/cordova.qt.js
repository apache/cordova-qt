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

    window.qmlWrapper.callPluginFunction(pluginName, functionName, JSON.stringify(parameters))
    return true;
}

/**
 * Function which is called from the native bridge in order to register the QtWebKit bridge objects
 */
Cordova.Qt.objects = {};
Cordova.Qt.registerObject = function( pluginName, pluginObject ) {
    Cordova.Qt.objects[pluginName] = pluginObject;
}

Cordova.exec = Cordova.Qt.exec;
