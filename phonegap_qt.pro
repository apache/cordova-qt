#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T22:43:53
#
#-------------------------------------------------

QT += core gui webkit network

TARGET = phonegap_qt
TEMPLATE = app

SOURCES += \
    cookiejar.cpp \
    extensions.cpp \
    main.cpp \
    mainwindow.cpp \
    networkaccessmanager.cpp \
    webpage.cpp \
    extensions/accelerometer.cpp \
    extensions/debugconsole.cpp \
    extensions/hash.cpp \
	extensions/utility.cpp \
    pgplugin.cpp \
    plugins/fileapi.cpp \
    plugins/notification.cpp \
    plugins/geolocation.cpp \
    plugins/powermanagement.cpp \
    plugins/device.cpp

HEADERS += \
    cookiejar.h \
    extensions.h \
    mainwindow.h \
    networkaccessmanager.h \
    webpage.h \
    extensions/accelerometer.h \
    extensions/debugconsole.h \
    extensions/hash.h \
	extensions/utility.h \
    pgplugin.h \
    plugins/fileapi.h \
    plugins/notification.h \
    plugins/geolocation.h \
    plugins/powermanagement.h \
    plugins/device.h

CONFIG += mobility
MOBILITY = bearer location systeminfo sensors feedback

symbian {
    TARGET.UID3 = 0xeceb982e
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.CAPABILITY = Location NetworkServices ReadUserData UserEnvironment ReadDeviceData WriteDeviceData WriteUserData PowerMgmt
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000

    htmlfiles.sources = ./../www/*
    phonegap_js.sources = ./../www/js/*
    phonegap_js.path = ./template/
    DEPLOYMENT += htmlfiles phonegap_js

    HEADERS += \
        extensions/camera.h \
        extensions/camerafileobserver.h \
        extensions/memorywatcher.h
    SOURCES += \
        extensions/camera.cpp \
        extensions/camerafileobserver.cpp \
        extensions/memorywatcher.cpp
    LIBS += -lcone -leikcore -lavkon -lhwrmvibraclient
}

OTHER_FILES += \
    ../www/index.html \
    ../www/basic.js \
    ../www/basic.css \
    ../www/js/phonegap.js \
    www/index.html \
    www/basic.js \
    www/basic.css \
    xml/plugins.xml \
    www/js/file.js \
    www/js/notification.js \
    www/js/phonegap.new.js \
    www/js/phonegap.js \
    www/js/phonegap.old.js \
    www/js/geolocation.js \
    www/js/powermanagement.js \
    www/js/phonegap.qt.js \
    www/js/device.js































































