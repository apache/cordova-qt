/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

PhoneGap.Qt = {};

/**
 * Execute a call to a plugin function
 * @return bool true on success, false on error (e.g. function doesn't exist)
 */
PhoneGap.Qt.exec = function( successCallback, errorCallback, pluginName, functionName, parameters ) {
    // Check if plugin is enabled
    if( PhoneGap.plugins[pluginName] !== true ) {
        return false;
    }

    // Check if we can find the function
    if( typeof PhoneGap.Qt.objects[pluginName][functionName] != "function" ) {
        return false;
    }

    // Store a reference to the callback functions
    var scId = PhoneGap.callbacks.length;
    var ecId = scId + 1;
    PhoneGap.callbacks[scId] = successCallback;
    PhoneGap.callbacks[ecId] = errorCallback;

    parameters.unshift( ecId );
    parameters.unshift( scId );

    // Call the function
    /*debug.log( "Call: " + pluginName + " / " + functionName );
    debug.log( "P-Obj: " + (typeof PhoneGap.plugins[pluginName]) );
    debug.log( "P-Func: " + (typeof PhoneGap.plugins[pluginName][functionName]) );*/
    //PhoneGap.plugins[pluginName][functionName](scId, ecId, parameters);
    PhoneGap.Qt.objects[pluginName][functionName].apply(this, parameters);

    return true;
}

/**
 * Function which is called from the native bridge in order to register the QtWebKit bridge objects
 */
PhoneGap.Qt.objects = {};
PhoneGap.Qt.registerObject = function( pluginName, pluginObject ) {
    PhoneGap.Qt.objects[pluginName] = pluginObject;
}

PhoneGap.exec = PhoneGap.Qt.exec;
