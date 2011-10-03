/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

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
