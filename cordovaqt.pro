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
    OTHER_FILES += \
        qml/main.qml \
        qml/cordova_wrapper.js
} else {
    SOURCES += mainwindow.cpp
    HEADERS += mainwindow.h
    FORMS += mainwindow.ui

    symbian:TARGET.UID3 = 0xE3522943
    #symbian:DEPLOYMENT.installer_header = 0x2002CCCF
    symbian:TARGET.CAPABILITY += NetworkServices

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
    src/cordova.cpp \
    src/cplugin.cpp \
    src/cwebpage.cpp
HEADERS += mainwindow.h \
    src/plugins/notification.h \
    src/plugins/geolocation.h \
    src/plugins/fileapi.h \
    src/plugins/device.h \
    src/pluginregistry.h \
    src/plugins/console.h \
    src/plugins/connection.h \
    src/plugins/compass.h \
    src/cordova.h \
    src/cplugin.h \
    src/cwebpage.h
FORMS += mainwindow.ui

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
