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

#include "accelerometer.h"

Accelerometer* Accelerometer::m_accelerometer = new Accelerometer();

Accelerometer::Accelerometer() : CPlugin()
{   
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Accelerometer", this );
}

Accelerometer::~Accelerometer(){
   delete m_accelerometerSource;
}

void Accelerometer::init(){
    m_x_accel = 0;
    m_y_accel = 0;
    m_z_accel = 0;

    m_timestamp = 0;

    m_accelerometerSource = new QAccelerometer();
    m_accelerometerSource->start();
    connect(m_accelerometerSource,SIGNAL(readingChanged()), SLOT(updateSensor()));
}

void Accelerometer::getCurrentAcceleration( int scId, int ecId, QVariantMap p_options) {
    Q_UNUSED(ecId);
    Q_UNUSED(p_options);

    QString callbackArguments = "Acceleration.cast({ x: " + QString::number(m_x_accel) + "," +
                                "                    y: " + QString::number(m_y_accel) + "," +
                                "                    z: " + QString::number(m_z_accel) + "," +
                                "                    timestamp: " + QString::number(m_timestamp) + "})";

    this->callback( scId, callbackArguments );
}

void Accelerometer::updateSensor(){
    QAccelerometerReading *accelerometer = m_accelerometerSource->reading();
    m_x_accel = accelerometer->x();
    m_y_accel = accelerometer->y();
    m_z_accel = accelerometer->z();
    m_timestamp = accelerometer->timestamp();
}
