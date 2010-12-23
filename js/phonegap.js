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


function Device() {

    this.name = GapDeviceInfo.name;
    this.platform = GapDeviceInfo.platform;
    this.uuid = GapDeviceInfo.uuid;
    this.version = GapDeviceInfo.version;
    this.phonegap = '0.9.2';
}


navigator.notification = new Notification();
navigator.accelerometer = new Accelerometer();
navigator.network = new Network();

if (window.device == undefined) window.device = new Device();
