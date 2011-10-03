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
