#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T22:43:53
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = phonegap_qt
TEMPLATE = app

SOURCES += main.cpp\
        webkitwindow.cpp \
    mobileextensions.cpp \
    accelerometer.cpp \
    deviceinfo.cpp

HEADERS  += webkitwindow.h \
    mobileextensions.h \
    accelerometer.h \
    deviceinfo.h

FORMS    += webkitwindow.ui

CONFIG += mobility
MOBILITY = sensors

symbian {
    TARGET.UID3 = 0xeceb982e
    TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData
    LIBS += -lcone -leikcore -lavkon -lhwrmvibraclient
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
    htmlfiles.sources = ./../template/
    phonegap_js.sources = ./../js/*
    phonegap_js.path = ./template/
    DEPLOYMENT += htmlfiles phonegap_js
    include(vibra/vibra.pri)
}
