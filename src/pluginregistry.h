#ifndef PLUGINREGISTRY_H
#define PLUGINREGISTRY_H

#include <QMap>
#include <QString>

#include "pgplugin.h"

class PluginRegistry
{
public:
    PluginRegistry();

    static PluginRegistry* getRegistry();

    void registerPlugin( QString p_name, PGPlugin *p_plugin );

    PGPlugin* getPlugin( QString p_name );

private:
    static PluginRegistry* m_pluginRegistry;
    QMap<QString,PGPlugin*> *m_plugins;
};

#endif // PLUGINREGISTRY_H
