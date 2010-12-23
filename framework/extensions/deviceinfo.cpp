#include "deviceinfo.h"


DeviceInfo::DeviceInfo(QObject *parent) :
    QObject(parent) {

    m_name = "Symbian";
    m_platform = "Symbian";
    m_uuid = "";

#if defined(Q_WS_S60)
    QSysInfo info;
    QSysInfo::SymbianVersion v = info.symbianVersion();
    if (v < QSysInfo::SV_9_4) {
        m_version = "Symbian Non-Touch";
    } else if (v == QSysInfo::SV_9_4) {
        m_version = "Symbian Touch";
    } else {
        m_version = "Symbian^3";
    }
#endif
}

const QString &DeviceInfo::name() const {

    return m_name;
}

const QString &DeviceInfo::platform() const {

    return m_platform;
}

const QString &DeviceInfo::uuid() const {

    return m_uuid;
}

const QString &DeviceInfo::version() const {

    return m_version;
}
