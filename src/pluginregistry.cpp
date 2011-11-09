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

#include "pluginregistry.h"

PluginRegistry* PluginRegistry::m_pluginRegistry = NULL;

PluginRegistry::PluginRegistry() {
    m_plugins = new QMap<QString,PGPlugin*>();
}

PluginRegistry *PluginRegistry::getRegistry() {
    if( PluginRegistry::m_pluginRegistry == NULL ) {
        PluginRegistry::m_pluginRegistry = new PluginRegistry();
    }

    return PluginRegistry::m_pluginRegistry;
}

void PluginRegistry::registerPlugin( QString p_name, PGPlugin *p_plugin ) {
    m_plugins->insert( p_name, p_plugin );
}

PGPlugin* PluginRegistry::getPlugin( QString p_name ) {
    return m_plugins->value( p_name, NULL );
}
