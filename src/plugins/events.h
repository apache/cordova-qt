/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

#ifndef EVENTS_H
#define EVENTS_H

#include "../cplugin.h"

#if QT_VERSION < 0x050000
# include <QSystemNetworkInfo>
# include <QSystemBatteryInfo>
#else
# include <QtSystemInfo>
# include <QBatteryInfo>
#endif

#ifdef QTM_NAMESPACE
QTM_USE_NAMESPACE
#endif

class QNetworkConfigurationManager;

class Events : public CPlugin
{
    Q_OBJECT
public:
    explicit Events();

    void init();

signals:

public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
#if QT_VERSION < 0x050000
    void remainingCapacityChanged(int capacity);
    void chargerTypeChanged(QSystemBatteryInfo::ChargerType type);
#else
    void remainingCapacityChanged(int battery, int capacity);
    void chargerTypeChanged(QBatteryInfo::ChargerType type);
#endif
    void onlineStatusChanged(bool isOnline);

private:
    static Events *m_events;

#if QT_VERSION < 0x050000
    QSystemBatteryInfo *m_batteryInfo;
#else
    QBatteryInfo *m_batteryInfo;
#endif

    int m_previousPercent;

    QNetworkConfigurationManager *m_networkConfigurationManager;
};

#endif // EVENTS_H
