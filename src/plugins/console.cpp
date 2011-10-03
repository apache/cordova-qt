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
