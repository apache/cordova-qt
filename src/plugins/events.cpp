#include "events.h"

#include "../pluginregistry.h"
#include "../cordova.h"

#include <QKeyEvent>
#include <QNetworkConfigurationManager>

#include <QDebug>

// Create static instance of ourself
Events *Events::m_events = new Events();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Events::Events() :
    CPlugin()
{
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Events", this );
}

/**
 * Initialize the pugin
 */
void Events::init()
{
    m_previousPercent = 100;
    qDebug() << Q_FUNC_INFO;
#if QT_VERSION < 0x050000
    m_batteryInfo = new QSystemBatteryInfo(this);
    connect(m_batteryInfo, SIGNAL(remainingCapacityChanged(int)), this, SLOT(remainingCapacityChanged(int)));
    connect(m_batteryInfo, SIGNAL(chargerTypeChanged(QSystemBatteryInfo::ChargerType)), this, SLOT(chargerTypeChanged(QSystemBatteryInfo::ChargerType)));
#else
    m_batteryInfo = new QBatteryInfo(this);
    connect(m_batteryInfo, SIGNAL(remainingCapacityChanged(int,int)), this, SLOT(remainingCapacityChanged(int,int)));
    connect(m_batteryInfo, SIGNAL(chargerTypeChanged(QBatteryInfo::ChargerType)), this, SLOT(chargerTypeChanged(QBatteryInfo::ChargerType)));
#endif
    if (Cordova::instance()->topLevelEventsReceiver())
        Cordova::instance()->topLevelEventsReceiver()->installEventFilter(this);
    m_networkConfigurationManager = new QNetworkConfigurationManager(this);
    connect(m_networkConfigurationManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(onlineStatusChanged(bool)));
}

bool Events::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == Cordova::instance()->topLevelEventsReceiver()) {
        if (ev->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);
            if (!keyEvent)
                return false;
            switch (keyEvent->key()) {
            case Qt::Key_Menu:
                Cordova::instance()->execJS( QString("Cordova.menuKeyPressed();"));
                break;
            case Qt::Key_Back:
                Cordova::instance()->execJS( QString("Cordova.backKeyPressed();"));
                break;
            case Qt::Key_Search:
                Cordova::instance()->execJS( QString("Cordova.searchKeyPressed();"));
                break;
            case Qt::Key_Call:
                Cordova::instance()->execJS( QString("Cordova.callKeyPressed();"));
                break;
            case Qt::Key_Hangup:
                Cordova::instance()->execJS( QString("Cordova.hangupKeyPressed();"));
                break;
            default:
                break;
            }
        } else if (ev->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);
            if (!keyEvent)
                return false;
            switch (keyEvent->key()) {
            case Qt::Key_VolumeUp:
                Cordova::instance()->execJS( QString("Cordova.volumeUpKeyPressed();"));
                break;
            case Qt::Key_VolumeDown:
                Cordova::instance()->execJS( QString("Cordova.volumeDownKeyPressed();"));
                break;
            default:
                break;
            }
        } else if (ev->type() == QEvent::WindowActivate) {
            Cordova::instance()->execJS( QString("Cordova.resumeOccured();"));
        } else if (ev->type() == QEvent::WindowDeactivate) {
            Cordova::instance()->execJS( QString("Cordova.pauseOccured();"));
        }
    }
    return false;
}

#if QT_VERSION < 0x050000
void Events::remainingCapacityChanged(int capacity)
#else
void Events::remainingCapacityChanged(int battery, int capacity)
#endif
{
    qDebug() << Q_FUNC_INFO;
    int newPercent;

#if QT_VERSION < 0x050000
    if (m_batteryInfo->nominalCapacity() < 1)
        newPercent = 0;
    newPercent = capacity/((double)m_batteryInfo->nominalCapacity()/100);
#else
    if (m_batteryInfo->maximumCapacity(battery) < 1)
        newPercent = 0;
    newPercent = capacity/((double)m_batteryInfo->maximumCapacity(battery)/100);
#endif
    if (m_previousPercent == newPercent)
        return;
    m_previousPercent = newPercent;
#if QT_VERSION < 0x050000
    bool onBattery = m_batteryInfo->chargerType() == QSystemBatteryInfo::UnknownCharger ||
            m_batteryInfo->chargerType() == QSystemBatteryInfo::NoCharger ||
            m_batteryInfo->chargerType() == QSystemBatteryInfo::VariableCurrentCharger;
#else
    bool onBattery = m_batteryInfo->chargerType() == QBatteryInfo::UnknownCharger ||
            m_batteryInfo->chargerType() == QBatteryInfo::VariableCurrentCharger;
#endif
    Cordova::instance()->execJS( QString("Cordova.batteryStatusChanged(%1, %2, false);")
                                  .arg(m_previousPercent)
                                  .arg(!onBattery));
}

#if QT_VERSION < 0x050000
void Events::chargerTypeChanged(QSystemBatteryInfo::ChargerType type)
#else
void Events::chargerTypeChanged(QBatteryInfo::ChargerType type)
#endif
{
    Q_UNUSED(type);
    qDebug() << Q_FUNC_INFO;
#if QT_VERSION < 0x050000
    bool isPlugged = m_batteryInfo->chargerType() == QSystemBatteryInfo::UnknownCharger ||
            m_batteryInfo->chargerType() == QSystemBatteryInfo::NoCharger ||
            m_batteryInfo->chargerType() == QSystemBatteryInfo::VariableCurrentCharger;
#else
    bool isPlugged = m_batteryInfo->chargerType() == QBatteryInfo::UnknownCharger ||
            m_batteryInfo->chargerType() == QBatteryInfo::VariableCurrentCharger;
#endif
    Cordova::instance()->execJS( QString("Cordova.batteryStatusChanged(%1, %2, true);")
                                    .arg(m_previousPercent)
                                 .arg(isPlugged));
}

void Events::onlineStatusChanged(bool isOnline)
{
    if (isOnline)
        Cordova::instance()->execJS( QString("Cordova.onlineOccured();"));
    else
        Cordova::instance()->execJS( QString("Cordova.offlineOccured();"));
}
