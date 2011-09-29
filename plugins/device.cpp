#include "device.h"

#include <QSystemDeviceInfo>
#include <QSystemInfo>

#define PHONEGAP "1.0.0"

QTM_USE_NAMESPACE

Device::Device(QWebFrame *p_webFrame) :
    PGPlugin(p_webFrame)
{
}

void Device::init( int scId, int ecId ) {
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

    this->callback( scId, "'" + systemDeviceInfo->model() + "', '" + PHONEGAP + "', '" + platform + "', '" + systemDeviceInfo->uniqueDeviceID() + "', '" + systemInfo->version( QSystemInfo::Os ) + "'" );
}
