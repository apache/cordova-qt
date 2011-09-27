/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#include "geolocation.h"

#include <QGeoPositionInfo>
#include <QGeoCoordinate>

Geolocation::Geolocation(QWebFrame *p_webFrame) :
    PGPlugin(p_webFrame)
{
    m_geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource( p_webFrame );
    if( m_geoPositionInfoSource != 0 ) {
        QObject::connect( m_geoPositionInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)) );
        QObject::connect( m_geoPositionInfoSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()) );
    }
}

void Geolocation::getCurrentPosition( int scId, int ecId, QVariantMap p_options ) {
    m_successCallbacks << scId;
    m_errorCallbacks << ecId;

    // Check if we have a valid source
    if( m_geoPositionInfoSource != 0 ) {
        m_geoPositionInfoSource->requestUpdate();
    }
    else {
        // TODO: Replace with correct error code
        this->updateTimeout();
    }
}

void Geolocation::positionUpdated( const QGeoPositionInfo &update ) {
    QGeoCoordinate coordinate = update.coordinate();
    QString callbackArguments = "Position.cast( Coordinates.cast( " + QString::number(coordinate.latitude()) +
            ", " + QString::number(coordinate.longitude()) +
            ", " + QString::number(coordinate.altitude()) +
            ", " + QString::number(update.attribute(QGeoPositionInfo::HorizontalAccuracy)) +
            ", " + QString::number(update.attribute(QGeoPositionInfo::VerticalAccuracy)) +
            ", " + QString::number(update.attribute(QGeoPositionInfo::Direction)) +
            ", " + QString::number(update.attribute(QGeoPositionInfo::GroundSpeed)) +
            " ), " + QString::number(update.timestamp().toMSecsSinceEpoch()) + " )";

    for( int i = 0; i < m_successCallbacks.size(); i++ ) {
        this->callback( m_successCallbacks.at( i ), callbackArguments );
    }

    m_errorCallbacks.clear();
    m_successCallbacks.clear();
}

void Geolocation::updateTimeout() {
    for( int i = 0; i < m_errorCallbacks.size(); i++ ) {
        this->callback( m_errorCallbacks.at( i ), "PositionError.cast( PositionError.TIMEOUT, 'Position request timed out.' )" );
    }

    m_errorCallbacks.clear();
    m_successCallbacks.clear();
}
