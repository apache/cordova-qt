/*
 *  Copyright 2011 Wolfgang Koller - http://www.gofg.at/
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "geolocation.h"

#include "../pluginregistry.h"

#include <QGeoPositionInfo>
#include <QGeoCoordinate>

Geolocation* Geolocation::m_geolocation = new Geolocation();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Geolocation::Geolocation() : CPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Geolocation", this );
}

void Geolocation::init() {
    m_geoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource( this );
    if( m_geoPositionInfoSource != 0 ) {
        QObject::connect( m_geoPositionInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)) );
        QObject::connect( m_geoPositionInfoSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()) );
    }
}

void Geolocation::getCurrentPosition( int scId, int ecId, QVariantMap p_options ) {
    Q_UNUSED(p_options)

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
