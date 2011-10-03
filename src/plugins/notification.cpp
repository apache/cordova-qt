#include "notification.h"

#include "../pluginregistry.h"

#include <QApplication>
#include <QFeedbackHapticsEffect>

QTM_USE_NAMESPACE

// Create static instance of ourself
Notification* Notification::m_notification = new Notification();

/**
 * Constructor - NOTE: Never do anything except registering the plugin
 */
Notification::Notification() : PGPlugin(){
    PluginRegistry::getRegistry()->registerPlugin( "com.phonegap.Notification", this );
}

/**
 * Nofication.beep - http://docs.phonegap.com/phonegap_notification_notification.md.html#notification.beep
 */
void Notification::beep( int scId, int ecId, int p_times ) {
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    for( int i = 0; i < p_times; i++ ) {
        QApplication::beep();
    }
}

/**
 * Notification.vibrate - http://docs.phonegap.com/phonegap_notification_notification.md.html#notification.vibrate
 */
void Notification::vibrate( int scId, int ecId, int p_milliseconds ) {
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    QFeedbackHapticsEffect vibrate;
    vibrate.setIntensity(1.0);
    vibrate.setDuration(p_milliseconds);
    vibrate.start();
}
