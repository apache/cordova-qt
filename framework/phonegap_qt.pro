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
    extensions/deviceinfo.cpp \
    extensions/geolocation.cpp \
    extensions/hash.cpp \
    extensions/notification.cpp \
	extensions/utility.cpp

HEADERS += \
    cookiejar.h \
    extensions.h \
    mainwindow.h \
    networkaccessmanager.h \
    webpage.h \
    extensions/accelerometer.h \
    extensions/debugconsole.h \
    extensions/deviceinfo.h \
    extensions/geolocation.h \
    extensions/hash.h \
    extensions/notification.h \
	extensions/utility.h

CONFIG += mobility
MOBILITY = bearer location sensors

symbian {
    TARGET.UID3 = 0xeceb982e
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.CAPABILITY = Location NetworkServices ReadUserData UserEnvironment ReadDeviceData WriteDeviceData WriteUserData PowerMgmt
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000

    htmlfiles.sources = ./../template/
    phonegap_js.sources = ./../js/*
    phonegap_js.path = ./template/
    DEPLOYMENT += htmlfiles phonegap_js

    include(vibra/vibra.pri)

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
