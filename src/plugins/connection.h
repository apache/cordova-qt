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

#ifndef CONNECTION_H
#define CONNECTION_H

#include "../cplugin.h"

#if QT_VERSION < 0x050000
#include <QSystemNetworkInfo>
#else
#include <QtSystemInfo>
#endif

#ifdef QTM_NAMESPACE
QTM_USE_NAMESPACE
#endif

class Connection : public CPlugin
{
    Q_OBJECT
public:
    explicit Connection();

    void init();

signals:

public slots:
    void setChangeCallback( int scId, int ecId );

#if QT_VERSION < 0x050000
    void cellDataTechnologyChanged( QSystemNetworkInfo::CellDataTechnology cellTech );
    void networkModeChanged( QSystemNetworkInfo::NetworkMode mode );
    void networkStatusChanged( QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status );
#else
    void currentCellDataTechnologyChanged( int interface, QNetworkInfo::CellDataTechnology cellTech );
    void currentNetworkModeChanged( QNetworkInfo::NetworkMode mode );
    void networkStatusChanged( QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status );
#endif

private:
    void typeChanged();

    int m_changeCallback;
#if QT_VERSION < 0x050000
    QSystemNetworkInfo *m_systemNetworkInfo;
#else
    QNetworkInfo *m_systemNetworkInfo;
#endif
    bool m_bInitialized;

    static Connection *m_connection;

};

#endif // CONNECTION_H
