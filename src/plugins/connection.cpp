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
Connection::Connection() : PGPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.phonegap.Connection", this );
}

/**
 * Initialize the pugin
 */
void Connection::init() {
    m_changeCallback = -1;
    m_systemNetworkInfo = new QSystemNetworkInfo();
    m_bInitialized = false;

    // Listen to changes in the network state
    QObject::connect(m_systemNetworkInfo,SIGNAL(cellDataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology)),this,SLOT(cellDataTechnologyChanged(QSystemNetworkInfo::CellDataTechnology)));
    QObject::connect(m_systemNetworkInfo,SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)),this,SLOT(networkModeChanged(QSystemNetworkInfo::NetworkMode)));
    QObject::connect(m_systemNetworkInfo,SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)),this,SLOT(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)));
}

void Connection::setChangeCallback( int scId, int ecId ) {
    m_changeCallback = scId;

    if( !m_bInitialized ) {
        m_bInitialized = true;
        typeChanged();
    }
}

/**
 * Callbacks for handling changes in the network type
 */
void Connection::cellDataTechnologyChanged( QSystemNetworkInfo::CellDataTechnology cellTech ) {
    typeChanged();
}
void Connection::networkModeChanged( QSystemNetworkInfo::NetworkMode mode ) {
    typeChanged();
}
void Connection::networkStatusChanged( QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status ) {
    typeChanged();
}

/**
 * Function for determining the current network type and updating the javascript end
 */
void Connection::typeChanged() {
    if( m_changeCallback < 0 ) return;

    int changeCallback = m_changeCallback;
    m_changeCallback = -1;

    QSystemNetworkInfo::NetworkMode networkMode = m_systemNetworkInfo->currentMode();
    QSystemNetworkInfo::NetworkStatus networkStatus = m_systemNetworkInfo->networkStatus(networkMode);
    QSystemNetworkInfo::CellDataTechnology cellDataTechnology = m_systemNetworkInfo->cellDataTechnology();

    // First of all check the status
    if( networkStatus == QSystemNetworkInfo::UndefinedStatus ) {
        this->callback( changeCallback, "Connection.UNKNOWN" );
    }
    else if( networkStatus == QSystemNetworkInfo::NoNetworkAvailable ) {
        this->callback( changeCallback, "Connection.NONE" );
    }
    // Now find the type of connection by networkMode & cellDataTechnology
    else {
        if( networkMode == QSystemNetworkInfo::WlanMode ) {
            this->callback( changeCallback, "Connection.WIFI" );
        }
        else if( networkMode == QSystemNetworkInfo::EthernetMode ) {
            this->callback( changeCallback, "Connection.ETHERNET" );
        }
        else if( networkMode == QSystemNetworkInfo::LteMode ) {
            this->callback( changeCallback, "Connection.CELL_4G" );
        }
        else if( cellDataTechnology == QSystemNetworkInfo::UmtsDataTechnology ) {
            this->callback( changeCallback, "Connection.CELL_3G" );
        }
        else {
            this->callback( changeCallback, "Connection.CELL_2G" );
        }
    }
}
