/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 */

/**
 * The order of events during page load and PhoneGap startup is as follows:
 *
 * onDOMContentLoaded         Internal event that is received when the web page is loaded and parsed.
 * window.onload              Body onload event.
 * onNativeReady              Internal event that indicates the PhoneGap native side is ready.
 * onPhoneGapInit             Internal event that kicks off creation of all PhoneGap JavaScript objects (runs constructors).
 * onPhoneGapReady            Internal event fired when all PhoneGap JavaScript objects have been created
 * onPhoneGapInfoReady        Internal event fired when device properties are available
 * onDeviceReady              User event fired to indicate that PhoneGap is ready
 * onResume                   User event fired to indicate a start/resume lifecycle event
 * onPause                    User event fired to indicate a pause lifecycle event
 * onDestroy                  Internal event fired when app is being destroyed (User should use window.onunload event, not this one).
 *
 * The only PhoneGap events that user code should register for are:
 *      onDeviceReady
 *      onResume
 *
 * Listeners can be registered as:
 *      document.addEventListener("deviceready", myDeviceReadyListener, false);
 *      document.addEventListener("resume", myResumeListener, false);
 *      document.addEventListener("pause", myPauseListener, false);
 */

if (typeof(DeviceInfo) !== 'object') {
    var DeviceInfo = {};
}

/**
 * This represents the PhoneGap API itself, and provides a global namespace for accessing
 * information about the state of PhoneGap.
 * @class
 */
var PG = {
    queue: {
        ready: true,
        commands: [],
        timer: null
    }
};

// alias PG to PhoneGap for compatibilty with common JS
var PhoneGap = PG;

/**
 * Custom pub-sub channel that can have functions subscribed to it
 * @constructor
 */
PG.Channel = function (type)
{
    this.type = type;
    this.handlers = {};
    this.guid = 0;
    this.fired = false;
    this.enabled = true;
};

/**
 * Subscribes the given function to the channel. Any time that
 * Channel.fire is called so too will the function.
 * Optionally specify an execution context for the function
 * and a guid that can be used to stop subscribing to the channel.
 * Returns the guid.
 */
PG.Channel.prototype = {
    subscribe: function(f, c, g) {
        // need a function to call
        if (f === null) return;

        var func = f;
        if (typeof c === "object" && typeof f === "function") { func = PG.close(c, f); }

        g = g || func.observer_guid || f.observer_guid || this.guid++;
        func.observer_guid = g;
        f.observer_guid = g;
        this.handlers[g] = func;
        return g;
    },

    /**
     * Like subscribe but the function is only called once and then it
     * auto-unsubscribes itself.
     */
    subscribeOnce: function(f, c) {
        var g = null;
        var _this = this;
        var m = function() {
            f.apply(c || null, arguments);
            _this.unsubscribe(g);
        };
        if (this.fired) {
            if (typeof c === "object" && typeof f === "function") f = PG.close(c, f);
            f.apply(this, this.fireArgs);
        } else {
            g = this.subscribe(m);
        }
        return g;
    },

    /**
     * Unsubscribes the function with the given guid from the channel.
     */
    unsubscribe: function(g) {
        if (typeof g === "function") { g = g.observer_guid; }
        this.handlers[g] = null;
        delete this.handlers[g];
    },

    /**
     * Calls all functions subscribed to this channel.
     */
    fire: function(e) {
        if (this.enabled) {
            var fail = false;
            var item, handler, rv;
            for (item in this.handlers) {
                if (this.handlers.hasOwnProperty(item)) {
                    handler = this.handlers[item];
                    if (typeof handler === "function") {
                        rv = (handler.apply(this, arguments) === false);
                        fail = fail || rv;
                    }
                }
            }
            this.fired = true;
            this.fireArgs = arguments;
            return !fail;
        }
        return true;
    }
}

/**
 * Calls the provided function only after all of the channels specified
 * have been fired.
 */
PG.Channel.join = function(h, c) {
    var i = c.length, f = function() {
        if (!(--i)) h();
    }, len = i, j;
    for (j=0; j<len; j++) {
        if (!c[j].fired) c[j].subscribeOnce(f);
        else i--;
    }
    if (!i) h();
};

/**
 * Boolean flag indicating if the PhoneGap API is available and initialized.
 */ // TODO: Remove this, it is unused here ... -jm
PG.available = DeviceInfo.uuid !== undefined;

/**
 * onDOMContentLoaded channel is fired when the DOM content
 * of the page has been parsed.
 */
PG.onDOMContentLoaded = new PG.Channel('onDOMContentLoaded');

/**
 * onNativeReady channel is fired when the PhoneGap native code
 * has been initialized.
 */
PG.onNativeReady = new PG.Channel('onNativeReady');

/**
 * onPhoneGapInit channel is fired when the web page is fully loaded and
 * PhoneGap native code has been initialized.
 */
PG.onPhoneGapInit = new PG.Channel('onPhoneGapInit');

/**
 * onPhoneGapReady channel is fired when the JS PhoneGap objects have been created.
 */
PG.onPhoneGapReady = new PG.Channel('onPhoneGapReady');

/**
 * onPhoneGapInfoReady channel is fired when the PhoneGap device properties
 * has been set.
 */
PG.onPhoneGapInfoReady = new PG.Channel('onPhoneGapInfoReady');

/**
 * onPhoneGapConnectionReady channel is fired when the PhoneGap connection properties
 * has been set.
 */
PG.onPhoneGapConnectionReady = new PG.Channel('onPhoneGapConnectionReady');

/**
 * onResume channel is fired when the PG native code
 * resumes.
 */
PG.onResume = new PG.Channel('onResume');

/**
 * onPause channel is fired when the PhoneGap native code
 * pauses.
 */
PG.onPause = new PG.Channel('onPause');

/**
 * onDestroy channel is fired when the PhoneGap native code
 * is destroyed.  It is used internally.
 * Window.onunload should be used by the user.
 */
PG.onDestroy = new PG.Channel('onDestroy');
PG.onDestroy.subscribeOnce(function() {
    PG.shuttingDown = true;
});
PG.shuttingDown = false;

// _nativeReady is global variable that the native side can set
// to signify that the native code is ready. It is a global since
// it may be called before any PhoneGap JS is ready.
if (typeof _nativeReady !== 'undefined') { PG.onNativeReady.fire(); }

/**
 * onDeviceReady is fired only after all PhoneGap objects are created and
 * the device properties are set.
 */
PG.onDeviceReady = new PG.Channel('onDeviceReady');


// Array of channels that must fire before "deviceready" is fired
PG.deviceReadyChannelsArray = [ PG.onPhoneGapReady, PG.onPhoneGapInfoReady, PG.onPhoneGapConnectionReady];

// Hashtable of user defined channels that must also fire before "deviceready" is fired
PG.deviceReadyChannelsMap = {};

/**
 * Indicate that a feature needs to be initialized before it is ready to be used.
 * This holds up PhoneGap's "deviceready" event until the feature has been initialized
 * and PhoneGap.initComplete(feature) is called.
 *
 * @param feature {String}     The unique feature name
 */
PG.waitForInitialization = function(feature) {
    if (feature) {
        var channel = new PG.Channel(feature);
        PG.deviceReadyChannelsMap[feature] = channel;
        PG.deviceReadyChannelsArray.push(channel);
    }
};

/**
 * Indicate that initialization code has completed and the feature is ready to be used.
 *
 * @param feature {String}     The unique feature name
 */
PG.initializationComplete = function(feature) {
    var channel = PG.deviceReadyChannelsMap[feature];
    if (channel) channel.fire();
};

/**
 * Create all PhoneGap objects once page has fully loaded and native side is ready.
 */
PG.Channel.join(function() {

    // Start listening for XHR callbacks
    setTimeout(function() {
            if (PG.usePolling) {
                PG.pollCallbackServer();
            } else {
                var polling = prompt("usePolling", "gap_callbackServer:");
                PG.usePolling = polling;
                if (polling == "true") {
                    PG.usePolling = true;
                    PG.pollCallbackServer();
                } else {
                    PG.usePolling = false;
                    PG.requestCallbackServer();
                }
            }
        }, 1);

    // Run PhoneGap constructors
    PG.onPhoneGapInit.fire();

    // Fire event to notify that all objects are created
    PG.onPhoneGapReady.fire();

    // Fire onDeviceReady event once all constructors have run and PhoneGap info has been
    // received from native side, and any user defined initialization channels.
    PG.Channel.join(function() {

        // Turn off app loading dialog
        //PG.notification.activityStop();

        PG.onDeviceReady.fire();

        // Fire the onresume event, since first one happens before JavaScript is loaded
        PG.onResume.fire();
    }, PG.deviceReadyChannelsArray);

}, [ PG.onDOMContentLoaded, PG.onNativeReady ]);

// Listen for DOMContentLoaded and notify our channel subscribers
document.addEventListener('DOMContentLoaded', function() {
    PG.onDOMContentLoaded.fire();
}, false);

// Intercept calls to document.addEventListener and watch for deviceready
PG.m_document_addEventListener = document.addEventListener;

document.addEventListener = function(evt, handler, capture) {
    var e = evt.toLowerCase();
    if (e === 'deviceready') PG.onDeviceReady.subscribeOnce(handler);
    else if (e === 'resume') {
        PG.onResume.subscribe(handler);
        if (PG.onDeviceReady.fired) PG.onResume.fire();
    } else if (e === 'pause') PG.onPause.subscribe(handler);
    else {
        // If subscribing to Android backbutton
        if (e === 'backbutton') PG.exec(null, null, "App", "overrideBackbutton", [true]);
        PG.m_document_addEventListener.call(document, evt, handler, capture);
    }
};

// Intercept calls to document.removeEventListener and watch for events that
// are generated by PhoneGap native code
PG.m_document_removeEventListener = document.removeEventListener;

document.removeEventListener = function(evt, handler, capture) {
    var e = evt.toLowerCase();

    // If unsubscribing to Android backbutton
    if (e === 'backbutton') PG.exec(null, null, "App", "overrideBackbutton", [false]);
    PG.m_document_removeEventListener.call(document, evt, handler, capture);
};

/**
 * Method to fire event from native code
 */
PG.fireEvent = function(type) {
    var e = document.createEvent('Events');
    e.initEvent(type);
    document.dispatchEvent(e);
};

/**
 * If JSON not included, use our own stringify. (Android 1.6)
 * The restriction on ours is that it must be an array of simple types.
 *
 * @param args
 * @return {String}
 */
PG.stringify = function(args) {
    if (typeof JSON === "undefined") {
        var s = "[";
        var i, type, start, name, nameType, a;
        for (i = 0; i < args.length; i++) {
            if (args[i] !== null) {
                if (i > 0) s = s + ",";
                type = typeof args[i];
                if ((type === "number") || (type === "boolean")) s = s + args[i];
                else if (args[i] instanceof Array) s = s + "[" + args[i] + "]";
                else if (args[i] instanceof Object) {
                    start = true;
                    s = s + '{';
                    for (name in args[i]) {
                        if (args[i][name] !== null) {
                            if (!start) s = s + ',';
                            s = s + '"' + name + '":';
                            nameType = typeof args[i][name];
                            if ((nameType === "number") || (nameType === "boolean")) s = s + args[i][name];
                            else if ((typeof args[i][name]) === 'function') s = s + '""';
                            else if (args[i][name] instanceof Object) s = s + PG.stringify(args[i][name]);
                            else s = s + '"' + args[i][name] + '"';
                            start = false;
                        }
                    }
                    s = s + '}';
                } else {
                    a = args[i].replace(/\\/g, '\\\\');
                    a = a.replace(/"/g, '\\"');
                    s = s + '"' + a + '"';
                }
            }
        }
        s = s + "]";
        return s;
    } else {
        return JSON.stringify(args);
    }
};

/**
 * Does a deep clone of the object.
 *
 * @param obj
 * @return {Object}
 */
PG.clone = function(obj) {
    var i, retVal;
    if(!obj) return obj;
    
    if(obj instanceof Array){
        retVal = [];
        for(i = 0; i < obj.length; ++i) retVal.push(PG.clone(obj[i]));
        return retVal;
    }
    
    if (typeof obj === "function") return obj;
    
    if(!(obj instanceof Object)) return obj;
    
    if (obj instanceof Date) return obj;
    
    retVal = {};
    for(i in obj){
        if(!(i in retVal) || retVal[i] !== obj[i]) retVal[i] = PG.clone(obj[i]);
    }
    return retVal;
};

PG.callbackId = 0;
PG.callbacks = {};
PG.callbackStatus = {
    NO_RESULT: 0,
    OK: 1,
    CLASS_NOT_FOUND_EXCEPTION: 2,
    ILLEGAL_ACCESS_EXCEPTION: 3,
    INSTANTIATION_EXCEPTION: 4,
    MALFORMED_URL_EXCEPTION: 5,
    IO_EXCEPTION: 6,
    INVALID_ACTION: 7,
    JSON_EXCEPTION: 8,
    ERROR: 9
};


/**
 * Execute a PhoneGap command.  It is up to the native side whether this action is synch or async.
 * The native side can return:
 *      Synchronous: PluginResult object as a JSON string
 *      Asynchrounous: Empty string ""
 * If async, the native side will PhoneGap.callbackSuccess or PhoneGap.callbackError,
 * depending upon the result of the action.
 *
 * @param {Function} success    The success callback
 * @param {Function} fail       The fail callback
 * @param {String} service      The name of the service to use
 * @param {String} action       Action to be run in PhoneGap
 * @param {Array.<String>} [args]     Zero or more arguments to pass to the method
 */
PG.exec = function(success, fail, service, action, args) {
    try {
        var callbackId = service + PG.callbackId++;
        if (success || fail) PG.callbacks[callbackId] = {success:success, fail:fail};

        var r = prompt(PG.stringify(args), "gap:"+PG.stringify([service, action, callbackId, true]));

        // If a result was returned
        if (r.length > 0) {
            eval("var v="+r+";");

            // If status is OK, then return value back to caller
            if (v.status === PG.callbackStatus.OK) {

                // If there is a success callback, then call it now with
                // returned value
                if (success) {
                    try {
                        success(v.message);
                    } catch (e) {
                        console.log("Error in success callback: " + callbackId  + " = " + e);
                    }

                    // Clear callback if not expecting any more results
                    if (!v.keepCallback) {
                        delete PG.callbacks[callbackId];
                    }
                }
                return v.message;
            } else if (v.status === PG.callbackStatus.NO_RESULT) { // If no result

                // Clear callback if not expecting any more results
                if (!v.keepCallback) delete PG.callbacks[callbackId];
            } else { // If error, then display error
                console.log("Error: Status="+v.status+" Message="+v.message);

                // If there is a fail callback, then call it now with returned value
                if (fail) {
                    try {
                        fail(v.message);
                    } catch (e1) {
                        console.log("Error in error callback: "+callbackId+" = "+e1);
                    }

                    // Clear callback if not expecting any more results
                    if (!v.keepCallback) delete PG.callbacks[callbackId];
                }
                return null;
            }
        }
    } catch (e2) {
        console.log("Error: "+e2);
    }
};

/**
 * Called by native code when returning successful result from an action.
 *
 * @param callbackId
 * @param args
 */
PG.callbackSuccess = function(callbackId, args) {
    if (PG.callbacks[callbackId]) {

        // If result is to be sent to callback
        if (args.status === PG.callbackStatus.OK) {
            try {
                if (PG.callbacks[callbackId].success) PG.callbacks[callbackId].success(args.message);
            } catch (e) {
                console.log("Error in success callback: "+callbackId+" = "+e);
            }
        }

        // Clear callback if not expecting any more results
        if (!args.keepCallback) delete PG.callbacks[callbackId];
    }
};

/**
 * Called by native code when returning error result from an action.
 *
 * @param callbackId
 * @param args
 */
PG.callbackError = function(callbackId, args) {
    if (PG.callbacks[callbackId]) {
        try {
            if (PG.callbacks[callbackId].fail) PG.callbacks[callbackId].fail(args.message);
        } catch (e) {
            console.log("Error in error callback: "+callbackId+" = "+e);
        }

        // Clear callback if not expecting any more results
        if (!args.keepCallback) delete PG.callbacks[callbackId];
    }
};

PG.callbackServerPort = null;
PG.callbackServerToken = null;

/**
 * This is only for Android.
 *
 * Internal function that uses XHR to call into PhoneGap Java code and retrieve
 * any JavaScript code that needs to be run.  This is used for callbacks from
 * Java to JavaScript.
 */
PG.requestCallbackServer = function() {

    // Exit if shutting down app
    if (PG.shuttingDown) return;

    // If polling flag was changed, start using polling from now on
    if (PG.usePolling) {
        PG.pollCallbackServer();
        return;
    }

    var xmlhttp = new XMLHttpRequest();

    // Callback function when XMLHttpRequest is ready
    xmlhttp.onreadystatechange=function(){
        if(xmlhttp.readyState === 4){

            // Exit if shutting down app
            if (PG.shuttingDown) return;

            // If callback has JavaScript statement to execute
            if (xmlhttp.status === 200) {

                // Need to url decode the response
                var msg = decodeURIComponent(xmlhttp.responseText);
                setTimeout(function() {
                    try {
                        var t = eval(msg);
                    } catch (e) {
                        // If we're getting an error here, seeing the message will help in debugging
                        console.log("requestCallbackServer: Message from Server: " + msg);
                        console.log("requestCallbackServer Error: "+e);
                    }
                }, 1);
                setTimeout(PG.requestCallbackServer, 1);
            } else if (xmlhttp.status === 404) {    // If callback ping (used to keep XHR request from timing out)
                setTimeout(PG.requestCallbackServer, 10);
            } else if (xmlhttp.status === 403) {    // If security error
                console.log("requestCallbackServer Error: Invalid token.  Stopping callbacks.");
            } else if (xmlhttp.status === 503) {    // If server is stopping
                console.log("requestCallbackServer Error: Service unavailable.  Stopping callbacks.");
            } else if (xmlhttp.status === 400) {    // If request wasn't GET
                console.log("requestCallbackServer Error: Bad request.  Stopping callbacks.");
            } else {                                // If error, revert to polling
                console.log("requestCallbackServer Error: Request failed.");
                PG.usePolling = true;
                PG.pollCallbackServer();
            }
        }
    };

    if (PG.callbackServerPort === null) PG.callbackServerPort = prompt("getPort", "gap_callbackServer:");
    if (PG.callbackServerToken === null) PG.callbackServerToken = prompt("getToken", "gap_callbackServer:");

    xmlhttp.open("GET", "http://127.0.0.1:"+PG.callbackServerPort+"/"+PG.callbackServerToken , true);
    xmlhttp.send();
};

/**
 * The polling period to use with pollCallbackServer.
 * This can be changed by the application.  The default is 50ms.
 */
PG.pollCallbackServerPeriod = 50;

/**
 * Flag that can be set by the user to force polling to be used or force XHR to be used.
 */
PG.usePolling = false;    // T=use polling, F=use XHR

/**
 * This is only for Android.
 *
 * Internal function that uses polling to call into PhoneGap Java code and retrieve
 * any JavaScript code that needs to be run.  This is used for callbacks from
 * Java to JavaScript.
 */
PG.pollCallbackServer = function() {

    // Exit if shutting down app
    if (PG.shuttingDown) return;

    // If polling flag was changed, stop using polling from now on
    if (!PG.usePolling) {
        PG.requestCallbackServer();
        return;
    }

    var msg = prompt("", "gap_poll:");
    if (msg) {
        setTimeout(function() {
            try {
                var t = eval(""+msg);
            } catch (e) {
                console.log("pollCallbackServer: Message from Server: " + msg);
                console.log("pollCallbackServer Error: "+e);
            }
        }, 1);
        setTimeout(PG.pollCallbackServer, 1);
    } else {
        setTimeout(PG.pollCallbackServer, PG.pollCallbackServerPeriod);
    }
};

/**
 * Create a UUID
 *
 * @return {String}
 */
PG.createUUID = function() {
    return PG.UUIDcreatePart(4) + '-' +
        PG.UUIDcreatePart(2) + '-' +
        PG.UUIDcreatePart(2) + '-' +
        PG.UUIDcreatePart(2) + '-' +
        PG.UUIDcreatePart(6);
};

PG.UUIDcreatePart = function(length) {
    var uuidpart = "";
    var i, uuidchar;
    for (i=0; i<length; i++) {
        uuidchar = parseInt((Math.random() * 256),0).toString(16);
        if (uuidchar.length === 1) uuidchar = "0" + uuidchar;
        uuidpart += uuidchar;
    }
    return uuidpart;
};

PG.close = PG.bind = function(context, func, params) {
    if (typeof params === 'undefined') return function() {
            return func.apply(context, arguments);
        };
    else return function() {
            return func.apply(context, params);
        };
};