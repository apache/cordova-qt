#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <QSysInfo>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = 0);
    QString name, platform, uuid, version;

signals:

public slots:


};

#endif // DEVICEINFO_H
