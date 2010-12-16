#include "deviceinfo.h"

DeviceInfo::DeviceInfo(QObject *parent) :
    QObject(parent)
{
    QSysInfo *sys_info = new QSysInfo();
    name = "Symbian";
    platform = "Symbian";
    uuid = "";
#if defined(Q_WS_S60)
    QSysInfo::SymbianVersion v;
    v = sys_info->symbianVersion();
    if (v < QSysInfo::SV_9_4) {
        version = "Symbian Non-Touch";
    } else if (v == QSysInfo::SV_9_4) {
        version = "Symbian Touch";
    }else {
        version = "Symbian^3";
    }
#endif
}

