Cordova/Qt
==========

Cordova/Qt is the Qt port of the Cordova (formerly PhoneGap) framework. It should compile on any platform which is compatible with Qt and Qt Mobility.

Requirements
------------

- Qt SDK

To Create a Project
-------------------

Download the source code and open the project file with Qt Creator. Add your HTML and JavaScript files to the www folder. Edit xml/plugins.xml to enable or disable any required plugins. Compiling, deployment, and packaging works as described in the Qt Creator documentation.

Supported APIs
--------------

- Accelerometer
- Compass
- Connection
- Contacts
- Device
- Events
- File (File Transfer API has no direct support in Qt Mobility and is not be implemented for now)
- Geolocation (Uses the W3C Geolocation API specification which is supported by WebKit 2)
- Notification (implemented for MeeGo Harmattan only)
- Storage (Uses the W3C WebSQL Database and Web Storage API specifications which are supported by WebKit 2)
- Camera (Basic support on MeeGo Harmattan but no gallery and no image settings)

Unsupported APIs
-----------------

- Capture - in progress
- Media (PhoneGap will be moving to follow the W3C specification which is supported by WebKit and these APIS are likley to be deprecated)

License
-------

Licensed under the APACHE-2.0 license. See LICENSE file for details.
