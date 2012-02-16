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

Cordova = {
    plugins: {},
    constructors: {},
    callbacks: [],
};

/*
 * Execute a callback function & remove it from the Cordova object
 */
Cordova.callback = function() {
    var scId = arguments[0];
    var callbackRef = null;

    var parameters = [];
    for( var i = 1; i < arguments.length; i++ ) {
        //debug.log( "Adding parameter " + arguments[i] );
        parameters[i-1] = arguments[i];
    }
    // Keep reference to callback
    callbackRef = Cordova.callbacks[scId];

    // Even IDs are success-, odd are error-callbacks - make sure we remove both
    if( (scId % 2) !== 0 ) {
        scId = scId - 1;
    }
    // Remove both the success as well as the error callback from the stack
    Cordova.callbacks.splice( scId, 2 );

    // Finally run the callback
    if( typeof callbackRef == "function" ) callbackRef.apply( this, parameters );
};

/*
 * Enable a plugin for use within Cordova
 */
Cordova.enablePlugin = function( pluginName ) {
    // Enable the plugin
    Cordova.plugins[pluginName] = true;

    // Run constructor for plugin if available
    if( typeof Cordova.constructors[pluginName] == "function" ) Cordova.constructors[pluginName]();
}

/*
 * Add a plugin-specific constructor function which is called once the plugin is loaded
 */
Cordova.addConstructor = function( pluginName, constructor ) {
    Cordova.constructors[pluginName] = constructor;
}

/**
 * Event interface - http://www.w3.org/TR/DOM-Level-2-Events/events.html#Events-Event
 */
Cordova.Event = function() {
};

Cordova.Event.CAPTURING_PHASE = 1;
Cordova.Event.AT_TARGET = 2;
Cordova.Event.BUBBLING_PHASE = 3;

Cordova.Event.prototype.type = "unknown";
Cordova.Event.prototype.target = Cordova;
Cordova.Event.prototype.currentTarget = Cordova;
Cordova.Event.prototype.eventPhase = Cordova.Event.AT_TARGET;
Cordova.Event.prototype.bubbles = false;
Cordova.Event.prototype.cancelable = false;
Cordova.Event.prototype.timeStamp = 0;

Cordova.Event.prototype.stopPropagation = function() {};
Cordova.Event.prototype.preventDefault = function() {};
Cordova.Event.prototype.initEvent = function( eventTypeArg, canBubbleArg, cancelableArg ) {
    this.type = eventTypeArg;
    this.timeStamp = (new Date()).getMilliseconds();
};

/*
 * EventHandler interface - handles one type of event
 * Not W3C defined, but required in order to handle our custom events
 */
Cordova.EventHandler = function( p_type ) {
    this.type = p_type;
}

Cordova.EventHandler.prototype.type = "unknown";
Cordova.EventHandler.prototype.listeners = [];
Cordova.EventHandler.prototype.addEventListener = function( p_listener, p_capture ) {
    if( p_capture ) {
        this.listeners.unshift( p_listener );
    }
    else {
        this.listeners.push( p_listener );
    }
};

Cordova.EventHandler.prototype.removeEventListener = function( p_listener, p_capture ) {
    // Try to find the event listener in our list
    for( var i = 0; i < this.listeners.length; i++ ) {
        if( this.listeners[i] == p_listener ) {
            // Remove the listener from our queue
            this.listeners.splice( i, 1 );
            return;
        }
    }
};

Cordova.EventHandler.prototype.dispatchEvent = function() {
    var event = new Cordova.Event();
    event.initEvent( this.type, false, false );

    // Translate arguments into an array including the custom event as first element
    var parameters = [ event ];
    for( var i = 0; i < arguments.length; i++ ) {
        parameters[i+1] = arguments[i];
    }

    // Notify all listeners about this event
    for( var i = 0; i < this.listeners.length; i++ ) {
        this.listeners[i].apply(Cordova, parameters);
    }
};

/*
 * Create the custom Cordova events
 */
Cordova.events = {
    deviceready: new Cordova.EventHandler( "deviceready" ),
    resume: new Cordova.EventHandler( "resume" ),
    pause: new Cordova.EventHandler( "pause" )
};

/*
 * EventTarget interface - http://www.w3.org/TR/DOM-Level-2-Events/events.html#Events-EventTarget
 */
//Keep references to the original EventTarget implementations
Cordova.doc_addEventListener = document.addEventListener;
Cordova.doc_removeEventListener = document.removeEventListener;
Cordova.doc_dispatchEvent = document.dispatchEvent;

document.addEventListener = function( type, listener, useCapture ) {
    if( typeof Cordova.events[type] != "undefined" ) {
        Cordova.events[type].addEventListener( listener, useCapture );
    }
    else {
        Cordova.doc_addEventListener.call(document, type, listener, useCapture);
    }
};

document.removeEventListener = function( type, listener, useCapture ) {
    if( typeof Cordova.events[type] != "undefined" ) {
        Cordova.events[type].removeEventListener( listener, useCapture );
    }
    else {
        Cordova.doc_removeEventListener.call(document, type, listener, useCapture);
    }
};

document.dispatchEvent = function( evt ) {
    if( typeof Cordova.events[evt.type] != "undefined" ) {
        Cordova.events[evt.type].dispatchEvent();
    }
    else {
        Cordova.doc_dispatchEvent.call(document, evt);
    }
};

/*
 * Trigger the global deviceready event - fired from native code
 */
Cordova.deviceready = function() {
    Cordova.events.deviceready.dispatchEvent();
}
