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

PhoneGap = {
    plugins: {},
    constructors: {},
    callbacks: [],
};

/*
 * Execute a callback function & remove it from the PhoneGap object
 */
PhoneGap.callback = function() {
    var scId = arguments[0];
    var callbackRef = null;

    var parameters = [];
    for( var i = 1; i < arguments.length; i++ ) {
        //debug.log( "Adding parameter " + arguments[i] );
        parameters[i-1] = arguments[i];
    }
    // Keep reference to callback
    callbackRef = PhoneGap.callbacks[scId];

    // Even IDs are success-, odd are error-callbacks - make sure we remove both
    if( (scId % 2) !== 0 ) {
        scId = scId - 1;
    }
    // Remove both the success as well as the error callback from the stack
    PhoneGap.callbacks.splice( scId, 2 );

    // Finally run the callback
    if( typeof callbackRef == "function" ) callbackRef.apply( this, parameters );
};

/*
 * Enable a plugin for use within PhoneGap
 */
PhoneGap.enablePlugin = function( pluginName ) {
    // Enable the plugin
    PhoneGap.plugins[pluginName] = true;

    // Run constructor for plugin if available
    if( typeof PhoneGap.constructors[pluginName] == "function" ) PhoneGap.constructors[pluginName]();
}

/*
 * Add a plugin-specific constructor function which is called once the plugin is loaded
 */
PhoneGap.addConstructor = function( pluginName, constructor ) {
    PhoneGap.constructors[pluginName] = constructor;
}

/**
 * Event interface - http://www.w3.org/TR/DOM-Level-2-Events/events.html#Events-Event
 */
PhoneGap.Event = function() {
};

PhoneGap.Event.CAPTURING_PHASE = 1;
PhoneGap.Event.AT_TARGET = 2;
PhoneGap.Event.BUBBLING_PHASE = 3;

PhoneGap.Event.prototype.type = "unknown";
PhoneGap.Event.prototype.target = PhoneGap;
PhoneGap.Event.prototype.currentTarget = PhoneGap;
PhoneGap.Event.prototype.eventPhase = PhoneGap.Event.AT_TARGET;
PhoneGap.Event.prototype.bubbles = false;
PhoneGap.Event.prototype.cancelable = false;
PhoneGap.Event.prototype.timeStamp = 0;

PhoneGap.Event.prototype.stopPropagation = function() {};
PhoneGap.Event.prototype.preventDefault = function() {};
PhoneGap.Event.prototype.initEvent = function( eventTypeArg, canBubbleArg, cancelableArg ) {
    this.type = eventTypeArg;
    this.timeStamp = (new Date()).getMilliseconds();
};

/*
 * EventHandler interface - handles one type of event
 * Not W3C defined, but required in order to handle our custom events
 */
PhoneGap.EventHandler = function( p_type ) {
    this.type = p_type;
}

PhoneGap.EventHandler.prototype.type = "unknown";
PhoneGap.EventHandler.prototype.listeners = [];
PhoneGap.EventHandler.prototype.addEventListener = function( p_listener, p_capture ) {
    if( p_capture ) {
        this.listeners.unshift( p_listener );
    }
    else {
        this.listeners.push( p_listener );
    }
};

PhoneGap.EventHandler.prototype.removeEventListener = function( p_listener, p_capture ) {
    // Try to find the event listener in our list
    for( var i = 0; i < this.listeners.length; i++ ) {
        if( this.listeners[i] == p_listener ) {
            // Remove the listener from our queue
            this.listeners.splice( i, 1 );
            return;
        }
    }
};

PhoneGap.EventHandler.prototype.dispatchEvent = function() {
    var event = new PhoneGap.Event();
    event.initEvent( this.type, false, false );

    // Translate arguments into an array including the custom event as first element
    var parameters = [ event ];
    for( var i = 0; i < arguments.length; i++ ) {
        parameters[i+1] = arguments[i];
    }

    // Notify all listeners about this event
    for( var i = 0; i < this.listeners.length; i++ ) {
        this.listeners[i].apply(PhoneGap, parameters);
    }
};

/*
 * Create the custom phonegap events
 */
PhoneGap.events = {
    deviceready: new PhoneGap.EventHandler( "deviceready" ),
    resume: new PhoneGap.EventHandler( "resume" ),
    pause: new PhoneGap.EventHandler( "pause" )
};

/*
 * EventTarget interface - http://www.w3.org/TR/DOM-Level-2-Events/events.html#Events-EventTarget
 */
//Keep references to the original EventTarget implementations
PhoneGap.doc_addEventListener = document.addEventListener;
PhoneGap.doc_removeEventListener = document.removeEventListener;
PhoneGap.doc_dispatchEvent = document.dispatchEvent;

document.addEventListener = function( type, listener, useCapture ) {
    if( typeof PhoneGap.events[type] != "undefined" ) {
        PhoneGap.events[type].addEventListener( listener, useCapture );
    }
    else {
        PhoneGap.doc_addEventListener.call(document, type, listener, useCapture);
    }
};

document.removeEventListener = function( type, listener, useCapture ) {
    if( typeof PhoneGap.events[type] != "undefined" ) {
        PhoneGap.events[type].removeEventListener( listener, useCapture );
    }
    else {
        PhoneGap.doc_removeEventListener.call(document, type, listener, useCapture);
    }
};

document.dispatchEvent = function( evt ) {
    if( typeof PhoneGap.events[evt.type] != "undefined" ) {
        PhoneGap.events[evt.type].dispatchEvent();
    }
    else {
        PhoneGap.doc_dispatchEvent.call(document, evt);
    }
};

/*
 * Trigger the global deviceready event - fired from native code
 */
PhoneGap.deviceready = function() {
    PhoneGap.events.deviceready.dispatchEvent();
}
