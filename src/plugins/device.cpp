#include "device.h"

#include "../pluginregistry.h"

#include <QSystemDeviceInfo>
#include <QSystemInfo>

#include <QDebug>

#define PHONEGAP "1.0.0"

QTM_USE_NAMESPACE

// Create static instance of ourself
Device* Device::m_device = new Device();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Device::Device() : PGPlugin() {
    PluginRegistry::getRegistry()->registerPlugin( "com.phonegap.Device", this );
}

/**
 * Called by the javascript constructor in order to receive all required device info(s)
 */
void Device::getInfo( int scId, int ecId ) {
    Q_UNUSED(ecId)

    QSystemDeviceInfo *systemDeviceInfo = new QSystemDeviceInfo(this);
    QSystemInfo *systemInfo = new QSystemInfo(this);

#ifdef Q_OS_SYMBIAN
    QString platform = "Symbian";
#endif
#ifdef Q_OS_WIN
    QString platform = "Windows";
#endif
#ifdef Q_OS_WINCE
    QString platform = "Windows CE";
#endif
#ifdef Q_OS_LINUX
    QString platform = "Linux";
#endif

    this->callback( scId, "'" + systemDeviceInfo->model() + "', '" + PHONEGAP + "', '" + platform + "', '" + systemDeviceInfo->uniqueDeviceID() + "', '" + systemInfo->version( QSystemInfo::Os ) + "'" );
}
