/*
 *  Copyright 2011  Integrated Computer Solutions - http://www.ics.com
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

#include "compass.h"
#include "../pluginregistry.h"
#include <QCompass>

Compass* Compass::m_compass = new Compass();


Compass::Compass() :CPlugin()
{
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Compass", this );
}

void Compass::init() {
    m_azymuth = 0;
    m_accuracy = 0;
    m_compassSource = new QCompass(this);
    m_compassSource->connectToBackend();
    m_compassSource->start();
    connect(m_compassSource,SIGNAL(readingChanged()), SLOT(updateSensor()));
}

void Compass::getCurrentHeading(int scId, int ecId, QVariantMap p_options) {
    Q_UNUSED(p_options)
    m_successCallbacks << scId;
    m_errorCallbacks << ecId;

    QString callbackArguments = "Heading.cast( " + QString::number(m_azymuth) + " )";

    for( int i = 0; i < m_successCallbacks.size(); i++ ) {
        this->callback( m_successCallbacks.at( i ), callbackArguments );
    }

    m_errorCallbacks.clear();
    m_successCallbacks.clear();
}

void Compass::updateSensor(){
    QCompassReading *heading = m_compassSource->reading();
    m_azymuth = heading->azimuth();
    m_accuracy = heading->calibrationLevel();
}
