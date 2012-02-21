#ifndef EVENTS_H
#define EVENTS_H

#include "../cplugin.h"

#if QT_VERSION < 0x050000
# include <QSystemNetworkInfo>
# include <QSystemBatteryInfo>
#else
# include <QtSystemInfo>
# include <QBatteryInfo>
#endif

#ifdef QTM_NAMESPACE
QTM_USE_NAMESPACE
#endif

class QNetworkConfigurationManager;

class Events : public CPlugin
{
    Q_OBJECT
public:
    explicit Events();

    void init();

signals:

public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
#if QT_VERSION < 0x050000
    void remainingCapacityChanged(int capacity);
    void chargerTypeChanged(QSystemBatteryInfo::ChargerType type);
#else
    void remainingCapacityChanged(int battery, int capacity);
    void chargerTypeChanged(QBatteryInfo::ChargerType type);
#endif
    void onlineStatusChanged(bool isOnline);

private:
    static Events *m_events;

#if QT_VERSION < 0x050000
    QSystemBatteryInfo *m_batteryInfo;
#else
    QBatteryInfo *m_batteryInfo;
#endif

    int m_previousPercent;

    QNetworkConfigurationManager *m_networkConfigurationManager;
};

#endif // EVENTS_H
