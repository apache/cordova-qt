/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#include "console.h"

#include "../pluginregistry.h"

#include <QDebug>

// Create static instance of ourself
Console *Console::m_console = new Console();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Console::Console() : PGPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.phonegap.Console", this );
}

void Console::log( int scId, int ecId, QString p_message ) {
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    qDebug() << "[LOG]" << p_message;
}
