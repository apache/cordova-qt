#include "pgplugin.h"

#include <QDebug>

PGPlugin::PGPlugin(QWebFrame *p_webFrame) :
    QObject(0)
{
    m_webFrame = p_webFrame;
}

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
