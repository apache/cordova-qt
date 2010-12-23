/*
 * Geolocation.cpp
 *
 *  Created on: Nov 24, 2009
 *      Author: nitobi-test
 */

#include "geolocation.h"


Geolocation::Geolocation(QObject *parent) :
    QObject(parent),
    m_source(0) {

    m_source = QGeoPositionInfoSource::createDefaultSource(this);
    if (m_source) {
        connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(onPositionUpdated(QGeoPositionInfo)));
        connect(m_source, SIGNAL(updateTimeout()),
                this, SLOT(onUpdateTimeout()));
    } else {
        qWarning("No GeoPosition source available.");
    }
}

void Geolocation::start(int interval) {
    Q_UNUSED(interval);

    if (m_source) {
        //m_source->setUpdateInterval(10000);
        //m_source->startUpdates();
        onPositionUpdated(m_source->lastKnownPosition());
    } else {
        emitError(2, "The device does not support position location.");
    }
}

void Geolocation::stop() {

    if (m_source) {
        m_source->stopUpdates();
    } else {
        emitError(2, "The device does not support position location.");
    }
}

void Geolocation::emitError(int code, const QString &message) {

    QVariantMap positionError;
    positionError["code"] = code;
    positionError["message"] = message;
    emit error(positionError);
}

void Geolocation::onPositionUpdated(const QGeoPositionInfo &info) {

    QVariantMap coords;
    coords["latitude"] = info.coordinate().latitude();
    coords["longitude"] = info.coordinate().longitude();
    coords["altitude"] = info.coordinate().altitude();
    coords["accuracy"] = info.attribute(QGeoPositionInfo::HorizontalAccuracy);
    coords["altitudeAccurary"] = info.attribute(QGeoPositionInfo::VerticalAccuracy);
    coords["heading"] = info.attribute(QGeoPositionInfo::Direction);
    coords["speed"] = info.attribute(QGeoPositionInfo::GroundSpeed);
    QVariantMap position;
    position["coords"] = coords;
    position["timestamp"] = QDateTime::currentDateTime(); //info.timestamp();

    emit positionUpdated(position);
}

void Geolocation::onUpdateTimeout() {

    emitError(3, "Timeout occurred.");
}
