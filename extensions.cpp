#include "extensions.h"
#include "extensions/accelerometer.h"
#include "extensions/debugconsole.h"
#include "extensions/deviceinfo.h"
#include "extensions/geolocation.h"
#include "extensions/hash.h"
#include "extensions/utility.h"

#ifdef Q_WS_S60
#include "extensions/camera.h"
#include "extensions/memorywatcher.h"
#endif

#include <QWebFrame>
#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "plugins/fileapi.h"
#include "plugins/notification.h"

Extensions::Extensions(QWebView *webView) :
    QObject(webView) {

    m_frame = webView->page()->mainFrame();
    connect(m_frame, SIGNAL(loadFinished(bool)), SLOT(attachExtensions()));

    m_extensions["GapAccelerometer"] = new Accelerometer(this);
    m_extensions["GapDeviceInfo"] = new DeviceInfo(this);
    m_extensions["GapGeolocation"] = new Geolocation(this);

    m_extensions["GapDebugConsole"] = new DebugConsole(this);
    m_extensions["GapHash"] = new Hash(this);
    m_extensions["GapUtility"] = new Utility(this);
    m_extensions["File"] = new FileAPI(m_frame);
    m_extensions["Notification"] = new Notification(m_frame);

#ifdef Q_WS_S60
    m_extensions["GapCamera"] = new Camera(this);
    m_extensions["GapMemoryWatcher"] = new MemoryWatcher(this);
#endif

    //attachExtensions();
}

void Extensions::attachExtensions() {
    QDir templateDir = QDir::current();
#ifdef Q_WS_SIMULATOR
    templateDir.cdUp();
    templateDir.cd("phonegap-qt");      // Dirty hack for now...
#endif
    templateDir.cd("xml");

    QFile pluginsXml( templateDir.filePath("plugins.xml") );
    if( !pluginsXml.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        qDebug() << "Error loading plugins config!";
        return;
    }

    QXmlStreamReader plugins;
    plugins.setDevice( &pluginsXml );

    while(!plugins.atEnd()) {
        if( plugins.readNext() == QXmlStreamReader::StartElement ) {
            if( plugins.name() == "plugin" ) {
                QXmlStreamAttributes attribs = plugins.attributes();
                if( attribs.hasAttribute("name") && attribs.hasAttribute("value") ) {
                    QString attribName = attribs.value( "name" ).toString();
                    QString attribValue = attribs.value( "value" ).toString();

                    qDebug() << "Adding Plugin '" << attribName << "' with '" << attribValue << "'";
                    if( m_extensions.contains( attribName ) ) {
                        m_frame->addToJavaScriptWindowObject(attribName, m_extensions[attribName]);
                        m_frame->evaluateJavaScript( "PhoneGap.registerPlugin( '" + attribValue + "', " + attribName + " )" );
                    }
                    else {
                        qDebug() << "Unknown Plugin " << attribName;
                    }
                }
            }
        }
    }
}
