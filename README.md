PhoneGap/Qt
================
PhoneGap/Qt is a Qt application library that allows for PhoneGap based projects to be built for the Qt Platform. PhoneGap based applications are, at the core, an application written with web technology: HTML, CSS and JavaScript.

What works
================

Only the Accelerometer, Notification and Network modules are working, but we are looking forward to support the other modules as well.

Know Issues
================

When calling navigator.network.isReachable, if the device if not connected, it will connect to the internet using the default access point *without warning the user* or letting the user choose what access point to use.

