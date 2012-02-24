/*
 *  Copyright 2012  Integrated Computer Solutions - http://www.ics.com
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

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "../cplugin.h"
#include "../pluginregistry.h"
#include <QAccelerometer>
#include <QObject>
#include <QMap>
#include <QDebug>

#ifdef QTM_NAMESPACE
QTM_USE_NAMESPACE
#endif

class Accelerometer : public CPlugin
{
    Q_OBJECT
public:
    explicit Accelerometer();
    void init();
    ~Accelerometer();

signals:

public slots:
    void getCurrentAcceleration( int scId, int ecId, QVariantMap p_options);

protected slots:
    void updateSensor();

private:
    static Accelerometer *m_accelerometer;
    QAccelerometer* m_accelerometerSource;
    double m_x_accel;
    double m_y_accel;
    double m_z_accel;
    unsigned long m_timestamp;
};

#endif // ACCELEROMETER_H
