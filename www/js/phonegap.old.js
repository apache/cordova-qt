
/*
  "OLD" JS CODE
*/
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

/*
 * Workaround for old JS functionality
 */
PhoneGap.addConstructor( "com.phonegap.Console",
                        function() {
                            if (typeof debug == "undefined") debug = new DebugConsole();
                        }
                        );

PhoneGap.addConstructor( "com.phonegap.Accelerometer",
                        function() {
                            if (typeof navigator.accelerometer == "undefined") navigator.accelerometer = new Accelerometer();
                        }
                        );

PhoneGap.addConstructor( "com.phonegap.Camera",
                        function() {
                            if (typeof navigator.camera == "undefined" && typeof window.GapCamera != "undefined" ) navigator.camera = new Camera();
                        }
                        );

PhoneGap.addConstructor( "com.phonegap.Utility",
                        function() {
                            if (typeof navigator.utility == "undefined") navigator.utility = new Utility();
                        }
                        );

if (typeof navigator.network == "undefined") navigator.network = new Network();
if (typeof window.device == "undefined") window.device = new Device();
