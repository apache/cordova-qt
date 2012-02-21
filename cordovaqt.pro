# Add files and directories to ship with the application
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
wwwDir.source = www
xmlDir.source = xml
qmlDir.source = qml

DEPLOYMENTFOLDERS = wwwDir xmlDir qmlDir # file1 dir1


greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QT += location
    QT += feedback
    QT += systeminfo
    QT += sensors
    QT += quick declarative
    OTHER_FILES += qml/main_qt5.qml \
        qml/cordova_wrapper.js
} else:!isEmpty(MEEGO_VERSION_MAJOR) {
    OTHER_FILES += qml/main_harmattan.qml \
        qml/cordova_wrapper.js

    QT += declarative
    CONFIG += mobility qdeclarative-boostable
    MOBILITY += feedback location systeminfo sensors
} else {
    OTHER_FILES += qml/main.qml \
        qml/cordova_wrapper.js

    symbian:TARGET.UID3 = 0xE3522943
    #symbian:DEPLOYMENT.installer_header = 0x2002CCCF
    symbian:TARGET.CAPABILITY += NetworkServices

    QT += declarative

    CONFIG += mobility
    MOBILITY += feedback location systeminfo sensors
}

QT += webkit

SOURCES += main.cpp \
    src/plugins/notification.cpp \
    src/plugins/geolocation.cpp \
    src/plugins/fileapi.cpp \
    src/plugins/device.cpp \
    src/pluginregistry.cpp \
    src/plugins/console.cpp \
    src/plugins/connection.cpp \
    src/plugins/compass.cpp \
    src/plugins/accelerometer.cpp \
    src/plugins/events.cpp \
    src/cordova.cpp \
    src/cplugin.cpp \
    src/cwebpage.cpp
HEADERS += \
    src/plugins/notification.h \
    src/plugins/geolocation.h \
    src/plugins/fileapi.h \
    src/plugins/device.h \
    src/pluginregistry.h \
    src/plugins/console.h \
    src/plugins/connection.h \
    src/plugins/compass.h \
    src/plugins/accelerometer.h \
    src/plugins/events.h \
    src/cordova.h \
    src/cplugin.h \
    src/cwebpage.h

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
