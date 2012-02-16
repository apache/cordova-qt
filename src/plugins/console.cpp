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

#include "console.h"

#include "../pluginregistry.h"

#include <QDebug>

// Create static instance of ourself
Console *Console::m_console = new Console();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Console::Console() : CPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Console", this );
}

void Console::log( int scId, int ecId, QString p_message ) {
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    qDebug() << "[LOG]" << p_message;
}
