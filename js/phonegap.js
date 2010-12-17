function Accelerometer() {
    this.getCurrentAcceleration = function (accelerometerSuccess, accelerometerError) {
        try {
            var accel_res = extentions.getCurrentAcceleration();
            var accel = {
                x: accel_res[0],
                y: accel_res[1],
                z: accel_res[2],
            };
            accelerometerSuccess(accel);
        } catch (err) {
            accelerometerError();
        }
    };
    
    this.watchAcceleration = function (accelerometerSuccess, accelerometerError, options) {
        var freq = 500;
        if (options.frequency) freq = options.frequency;
        return setInterval(function () {
            navigator.accelerometer.getCurrentAcceleration(accelerometerSuccess, accelerometerError);
        }, freq);
    };
    
    this.clearWatch = function (watchID) {
        clearInterval(watchID);
    };
};


NetworkStatus = {
    NOT_REACHABLE: 0,
    REACHABLE_VIA_WIFI_NETWORK: 1,
    REACHABLE_VIA_CARRIER_DATA_NETWORK: 2
};

function Network() 
{
    this.isReachable = function(hostName, successCb, options)
    {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", hostName, true);
        xhr.onreadystatechange = function(req) {
            if (xhr.readyState != 4) return;
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

function Notification ()
{
    this.vibrate = function (milis) {
        extentions.vibrate(milis);
    };
    
    this.alert = function (message, callback, title, button) {
        alert(message);
        callback();
    };
    
    this.confirm = function (message, callback, title, buttons) {
        var result = confirm(message);
        callback();
        return result;
    };
    
    this.beep = function (times) {
        alert("not implemented");
    };
}

function Device () 
{
    this.name = extentions.device_info.name;
    this.platform = extentions.device_info.platform;
    this.uuid = extentions.device_info.uuid;
    this.version = extentions.device_info.version;
    this.phonegap = '0.9.2';
}

navigator = {
    notification: new Notification(),
    accelerometer: new Accelerometer(),
    network: new Network()
};

if (window.device == undefined) window.device = new Device();
