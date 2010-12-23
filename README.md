PhoneGap/Qt
================
PhoneGap/Qt is a Qt application library that allows for PhoneGap based projects
to be built for the Qt Platform. PhoneGap based applications are, at the core,
an application written with web technology: HTML, CSS and JavaScript.

What works
================

Only the Accelerometer, Camera*, Geolocation, Notification* and Network modules
are working, but we are looking forward to support the other modules as well.

In addition to the regular PhoneGap API there are the Hash and MemoryWatcher*
modules. These allow for calculating HMAC-SHA1 hashes (which is too slow to
perform in JavaScript on Nokia hardware) and monitoring the memory usage of the
application, respectively.

(*) These modules are supported on Symbian only.

Know Issues
================

When calling navigator.network.isReachable, if the device if not connected, it
will connect to the internet using the default access point *without warning the
user* or letting the user choose what access point to use.
