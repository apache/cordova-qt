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

#include "connection.h"

#include "../pluginregistry.h"

#include <QDebug>

// Create static instance of ourself
Connection *Connection::m_connection = new Connection();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Connection::Connection() : CPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Connection", this );
}

/**
 * Initialize the pugin
 */
void Connection::init() {
    m_changeCallback = -1;

#if QT_VERSION < 0x050000
    m_systemNetworkInfo = new QSystemNetworkInfo();
#else
    m_systemNetworkInfo = new QNetworkInfo();
#endif
    m_bInitialized = false;

    // Listen to changes in the network state
#if QT_VERSION < 0x050000
    QObject::connect(m_systemNetworkInfo,SIGNAL(cellDataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology)),this,SLOT(cellDataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology)));
    QObject::connect(m_systemNetworkInfo,SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)),this,SLOT(networkModeChanged(QSystemNetworkInfo::NetworkMode)));
    QObject::connect(m_systemNetworkInfo,SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)),this,SLOT(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)));
#else
    QObject::connect(m_systemNetworkInfo,SIGNAL(currentCellDataTechnologyChanged(int, QNetworkInfo::CellDataTechnology)),this,SLOT(currentCellDataTechnologyChanged(int, QNetworkInfo::CellDataTechnology)));
    QObject::connect(m_systemNetworkInfo,SIGNAL(currentNetworkModeChanged(QNetworkInfo::NetworkMode)),this,SLOT(currentNetworkModeChanged(QNetworkInfo::NetworkMode)));
    QObject::connect(m_systemNetworkInfo,SIGNAL(networkStatusChanged(QNetworkInfo::NetworkMode, int, QNetworkInfo::NetworkStatus)),this,SLOT(networkStatusChanged(QNetworkInfo::NetworkMode, int, QNetworkInfo::NetworkStatus)));
#endif
}

void Connection::setChangeCallback( int scId, int ecId ) {
    Q_UNUSED(ecId);
    m_changeCallback = scId;

    if( !m_bInitialized ) {
        m_bInitialized = true;
        typeChanged();
    }
}

/**
 * Callbacks for handling changes in the network type
 */
#if QT_VERSION < 0x050000
void Connection::cellDataTechnologyChanged( QSystemNetworkInfo::CellDataTechnology cellTech ) {
    Q_UNUSED(cellTech);
    typeChanged();
}
void Connection::networkModeChanged( QSystemNetworkInfo::NetworkMode mode ) {
    Q_UNUSED(mode);
    typeChanged();
}
void Connection::networkStatusChanged( QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status ) {
    Q_UNUSED(mode);
    Q_UNUSED(status);
    typeChanged();
}
#else
void Connection::currentCellDataTechnologyChanged( int interface, QNetworkInfo::CellDataTechnology cellTech ) {
    Q_UNUSED(interface);
    Q_UNUSED(cellTech);
    typeChanged();
}
void Connection::currentNetworkModeChanged( QNetworkInfo::NetworkMode mode ) {
    Q_UNUSED(mode);
    typeChanged();
}
void Connection::networkStatusChanged( QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status ) {
    Q_UNUSED(mode);
    Q_UNUSED(interface);
    Q_UNUSED(status);
    typeChanged();
}
#endif // QT_VERSION < 0x050000

/**
 * Function for determining the current network type and updating the javascript end
 */
void Connection::typeChanged() {
    if( m_changeCallback < 0 ) return;

    int changeCallback = m_changeCallback;
    m_changeCallback = -1;

#if QT_VERSION < 0x050000
    QSystemNetworkInfo::NetworkMode networkMode = m_systemNetworkInfo->currentMode();
    QSystemNetworkInfo::NetworkStatus networkStatus = m_systemNetworkInfo->networkStatus(networkMode);
    QSystemNetworkInfo::CellDataTechnology cellDataTechnology = m_systemNetworkInfo->cellDataTechnology();
#else
    QNetworkInfo::NetworkMode networkMode = m_systemNetworkInfo->currentNetworkMode();
    QNetworkInfo::NetworkStatus networkStatus = m_systemNetworkInfo->networkStatus(networkMode, 0);
    QNetworkInfo::CellDataTechnology cellDataTechnology = m_systemNetworkInfo->currentCellDataTechnology(0);
#endif

    // First of all check the status
#if QT_VERSION < 0x050000
    if( networkStatus == QSystemNetworkInfo::UndefinedStatus ) {
#else
    if( networkStatus == QNetworkInfo::UnknownStatus ) {
#endif
        this->callback( changeCallback, "Connection.UNKNOWN" );
    }
#if QT_VERSION < 0x050000
    else if( networkStatus == QSystemNetworkInfo::NoNetworkAvailable ) {
#else
    else if( networkStatus == QNetworkInfo::NoNetworkAvailable ) {
#endif
        this->callback( changeCallback, "Connection.NONE" );
    }
    // Now find the type of connection by networkMode & cellDataTechnology
    else {
#if QT_VERSION < 0x050000
        if( networkMode == QSystemNetworkInfo::WlanMode ) {
#else
        if( networkMode == QNetworkInfo::WlanMode ) {
#endif
            this->callback( changeCallback, "Connection.WIFI" );
        }
#if QT_VERSION < 0x050000
        else if( networkMode == QSystemNetworkInfo::EthernetMode ) {
#else
        else if( networkMode == QNetworkInfo::EthernetMode ) {
            this->callback( changeCallback, "Connection.ETHERNET" );
#endif
        }
#if QT_VERSION < 0x050000
        else if( networkMode == QSystemNetworkInfo::LteMode ) {
#else
        else if( networkMode == QNetworkInfo::LteMode ) {
#endif
            this->callback( changeCallback, "Connection.CELL_4G" );
        }
#if QT_VERSION < 0x050000
        else if( cellDataTechnology == QSystemNetworkInfo::UmtsDataTechnology ) {
#else
        else if( cellDataTechnology == QNetworkInfo::UmtsDataTechnology ) {
#endif
            this->callback( changeCallback, "Connection.CELL_3G" );
        }
        else {
            this->callback( changeCallback, "Connection.CELL_2G" );
        }
    }
}
