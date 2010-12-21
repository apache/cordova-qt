/*
 * JavaScript file
 */

var vibration_length = 1000;

function get(id)
{
	return document.getElementById(id);
}

function init() 
{
    navigator.accelerometer.watchAcceleration(function (v) {
        get("accel_val").innerHTML = v.x + '   ' + v.y + '    ' + v.z;
    }, null, {});
}

function ping_google() 
{
    navigator.network.isReachable('http://www.google.com', function (state) {
        if (state == NetworkStatus.NOT_REACHABLE) {
            alert('Google not found');
        } else {
            alert('Google says: Pong!');
        }
    }, {});
}

function test_vibra()
{
	navigator.notification.vibrate(vibration_length);
}
