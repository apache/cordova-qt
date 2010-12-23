#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <QSysInfo>


class DeviceInfo : public QObject {

    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString platform READ platform)
    Q_PROPERTY(QString uuid READ uuid)
    Q_PROPERTY(QString version READ version)

    public:
        explicit DeviceInfo(QObject *parent = 0);

        const QString &name() const;
        const QString &platform() const;
        const QString &uuid() const;
        const QString &version() const;

    private:
        QString m_name;
        QString m_platform;
        QString m_uuid;
        QString m_version;
};

#endif // DEVICEINFO_H
