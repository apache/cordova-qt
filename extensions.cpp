#include "extensions.h"
#include "extensions/accelerometer.h"
#include "extensions/debugconsole.h"
#include "extensions/deviceinfo.h"
#include "extensions/geolocation.h"
#include "extensions/hash.h"
#include "extensions/notification.h"
#include "extensions/utility.h"

#ifdef Q_WS_S60
#include "extensions/camera.h"
#include "extensions/memorywatcher.h"
#endif

#include <QWebFrame>


Extensions::Extensions(QWebView *webView) :
    QObject(webView) {

    m_frame = webView->page()->mainFrame();
    connect(m_frame, SIGNAL(javaScriptWindowObjectCleared()), SLOT(attachExtensions()));

    m_extensions["GapAccelerometer"] = new Accelerometer(this);
    m_extensions["GapDebugConsole"] = new DebugConsole(this);
    m_extensions["GapDeviceInfo"] = new DeviceInfo(this);
    m_extensions["GapGeolocation"] = new Geolocation(this);
    m_extensions["GapHash"] = new Hash(this);
    m_extensions["GapNotification"] = new Notification(this);
    m_extensions["GapUtility"] = new Utility(this);

#ifdef Q_WS_S60
    m_extensions["GapCamera"] = new Camera(this);
    m_extensions["GapMemoryWatcher"] = new MemoryWatcher(this);
#endif

    attachExtensions();
}

void Extensions::attachExtensions() {

    foreach (QString name, m_extensions.keys()) {
        m_frame->addToJavaScriptWindowObject(name, m_extensions[name]);
    }
}
