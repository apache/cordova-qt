#include "pgplugin.h"

#include <QDebug>

PGPlugin::PGPlugin() : QObject(0) {
}

/**
 * Receive a reference to the currently active webframe
 */
void PGPlugin::setWebFrame(QWebFrame *p_webFrame) {
    m_webFrame = p_webFrame;

    // Initialize the plugin
    init();
}

/**
 * Override this function if your plugin requires startup initialisation
 */
void PGPlugin::init() {
}

/**
 * Execute a callback on the javascript end
 */
void PGPlugin::callback( int p_callbackId, QString p_jsParameters ) {
    QString javascript;

    if( p_jsParameters.length() > 0 ) {
        javascript = "PhoneGap.callback( " + QString::number( p_callbackId ) + ", " + p_jsParameters + " );";
    }
    else {
        javascript = "PhoneGap.callback( " + QString::number( p_callbackId ) + " );";
    }

    qDebug() << "Running: " << javascript;
    m_webFrame->evaluateJavaScript( javascript );
}
