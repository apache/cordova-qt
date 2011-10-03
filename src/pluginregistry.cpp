/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
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
