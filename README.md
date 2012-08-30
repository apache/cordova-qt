Cordova/Qt
==========

Cordova/Qt is the Qt port of the Apache Cordova project. It should compile on any platform which is compatible with Qt and Qt Mobility.

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
- File (Except for File Transfer API which has no direct support in Qt Mobility and is not implemented for now)
- Geolocation (Uses the W3C Geolocation API specifications)
- Notification (implemented for MeeGo Harmattan only)
- Storage (Uses the W3C WebSQL Database and Web Storage API specifications)
- Camera (Basic support on Symbian and MeeGo Harmattan but no gallery or image settings)

Unsupported APIs
-----------------

- Capture - in progress
- Media (Cordova will be moving to follow the W3C specification which is supported by WebKit and these APIS are likely to be deprecated)

License
-------

Licensed under the APACHE-2.0 license. See LICENSE file for details.
