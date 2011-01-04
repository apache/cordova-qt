function Accelerometer() {

    this.getCurrentAcceleration = function(accelerometerSuccess, accelerometerError) {
        try {
            var accel = GapAccelerometer.getCurrentAcceleration();
            accelerometerSuccess(accel);
        } catch(err) {
            accelerometerError();
        }
    };
    
    this.watchAcceleration = function(accelerometerSuccess, accelerometerError, options) {
        var freq = options.frequency || 500;
        var self = this;
        return setInterval(function () {
            self.getCurrentAcceleration(accelerometerSuccess, accelerometerError);
        }, freq);
    };
    
    this.clearWatch = function(watchID) {
        clearInterval(watchID);
    };
}


function DebugConsole() {
}

DebugConsole.prototype.log = function (output, showTime) {
        if (showTime) {
                var now = "" + new Date().getTime();
                output = now.substring(now.length - 5) + ": " + output;
        }
        GapDebugConsole.log(output);
}


/**
 * This class provides access to the device camera.
 * @constructor
 */
Camera = function() {
        this.successCallback = null;
        this.errorCallback = null;
        var self = this;

        window.GapCamera.pictureDataCaptured.connect(function(image) {
                if (typeof(self.successCallback) == 'function') {
                        console.log("pictureDataCaptured");
                        self.successCallback(image);
                }
        });

        window.GapCamera.pictureFileCaptured.connect(function(fileName) {
                if (typeof(self.successCallback) == 'function') {
                        console.log("pictureFileCaptured");
                        self.successCallback(fileName);
                }
        });

        window.GapCamera.error.connect(function(errorCode, message) {
                if (typeof(self.errorCallback) == 'function') {
                        /// @todo translate error message
                        self.errorCallback(message);
                }
        });
}

/**
 * Format of image returned from getPicture
 */
Camera.DestinationType = {
        DATA_URL: 0,
        FILE_URI: 1
};
Camera.prototype.DestinationType = Camera.DestinationType;

/**
 * We use the Platform Services 2.0 API here. So we must include a portion of the
 * PS 2.0 source code (camera API).
 * @param {Function} successCallback
 * @param {Function} errorCallback
 * @param {Object} options
 */
Camera.prototype.getPicture = function(successCallback, errorCallback, options){

        this.successCallback = successCallback;
        this.errorCallback = errorCallback;

        GapCamera.quality = (typeof(options) == 'object' && options.quality) ? options.quality : 75;
        GapCamera.destinationType = (typeof(options) == 'object' && options.destinationType) ?
                                                                 options.destinationType :
                                                                 this.DestinationType.DATA_URL; // default to BASE64 encoded image

        GapCamera.takePicture();
}


NetworkStatus = {
    NOT_REACHABLE: 0,
    REACHABLE_VIA_WIFI_NETWORK: 1,
    REACHABLE_VIA_CARRIER_DATA_NETWORK: 2
};

function Network() {

    this.isReachable = function(hostName, successCb, options) {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", hostName, true);
        xhr.onreadystatechange = function(req) {
            if (xhr.readyState != 4) {
                return;
            }

            alert(xhr.status);
            if (xhr.status != 200 && xhr.status != 304) {
                successCb(NetworkStatus.NOT_REACHABLE);
            } else {
                successCb(NetworkStatus.REACHABLE_VIA_WIFI_NETWORK);
            }
        };
        xhr.send();
    }
}


/**
 * This class provides access to device GPS data.
 * @constructor
 */
function Geolocation() {
    /**
     * The last known GPS position.
     */
    this.lastPosition = null;
    this.lastError = null;

    var self = this;
    GapGeolocation.positionUpdated.connect(function(position) {
        self.lastPosition = position;
    });
    GapGeolocation.error.connect(function(error) {
        self.lastError = error;
    });
}

/**
 * Asynchronously aquires the current position.
 * @param {Function} successCallback The function to call when the position
 * data is available
 * @param {Function} errorCallback The function to call when there is an error
 * getting the position data.
 * @param {PositionOptions} options The options for getting the position data
 * such as timeout.
 */
Geolocation.prototype.getCurrentPosition = function(successCallback, errorCallback, options) {
    var referenceTime = 0;
    if (this.lastPosition) {
        referenceTime = this.lastPosition.timeout;
    } else {
        this.start(options);
    }

    var timeout = 20000;
    var interval = 500;
    if (typeof(options) == 'object' && options.interval) {
        interval = options.interval;
    }

    if (typeof(successCallback) != 'function') {
        successCallback = function() {};
    }
    if (typeof(errorCallback) != 'function') {
        errorCallback = function() {};
    }

    var self = this;
    var delay = 0;
    var timer = setInterval(function() {
        delay += interval;

        if (typeof(self.lastPosition) == 'object' && self.lastPosition.timestamp > referenceTime) {
            successCallback(self.lastPosition);
            clearInterval(timer);
        } else if (delay >= timeout) {
            errorCallback({
                code: PositionError.TIMEOUT,
                message: "A timeout occurred."
            });
            clearInterval(timer);
        } else {
            // the interval gets called again
        }
    }, interval);
};

/**
 * Asynchronously aquires the position repeatedly at a given interval.
 * @param {Function} successCallback The function to call each time the position
 * data is available
 * @param {Function} errorCallback The function to call when there is an error
 * getting the position data.
 * @param {PositionOptions} options The options for getting the position data
 * such as timeout and the frequency of the watch.
 */
Geolocation.prototype.watchPosition = function(successCallback, errorCallback, options) {
    // Invoke the appropriate callback with a new Position object every time the implementation
    // determines that the position of the hosting device has changed.

    this.getCurrentPosition(successCallback, errorCallback, options);
    var maximumAge = 10000;
    if (typeof(options) == 'object' && options.maximumAge) {
        maximumAge = options.maximumAge;
    }

    var self = this;
    return setInterval(function() {
        self.getCurrentPosition(successCallback, errorCallback, options);
    }, maximumAge);
};

/**
 * Clears the specified position watch.
 * @param {String} watchId The ID of the watch returned from #watchPosition.
 */
Geolocation.prototype.clearWatch = function(watchId) {
    clearInterval(watchId);
};

Geolocation.prototype.start = function(options) {
    GapGeolocation.start(options.interval);
};

Geolocation.prototype.stop = function() {
    GapGeolocation.stop();
};


function Notification() {

    this.vibrate = function(milis) {
        GapNotification.vibrate(milis);
    };
    
    this.alert = function(message, callback, title, button) {
        alert(message);
        callback();
    };
    
    this.confirm = function(message, callback, title, buttons) {
        var result = confirm(message);
        callback();
        return result;
    };
    
    this.beep = function (times) {
        alert("not implemented");
    };
}


function Utility() {

    this.exit = function() {
        GapUtility.exit();
    };
}


function Device() {

    this.name = GapDeviceInfo.name;
    this.platform = GapDeviceInfo.platform;
    this.uuid = GapDeviceInfo.uuid;
    this.version = GapDeviceInfo.version;
    this.phonegap = '0.9.2';
}


if (typeof debug == "undefined") debug = new DebugConsole();

if (typeof navigator.accelerometer == "undefined") navigator.accelerometer = new Accelerometer();
if (typeof navigator.camera == "undefined") navigator.camera = new Camera();
if (typeof navigator.geolocation == "undefined") navigator.geolocation = new Geolocation();
if (typeof navigator.network == "undefined") navigator.network = new Network();
if (typeof navigator.notification == "undefined") navigator.notification = new Notification();
if (typeof navigator.utility == "undefined") navigator.utility = new Utility();

if (typeof window.device == "undefined") window.device = new Device();
