#include "notification.h"

#include <QApplication>
#include <QFeedbackHapticsEffect>

QTM_USE_NAMESPACE

Notification::Notification(QWebFrame *p_webFrame) :
    PGPlugin(p_webFrame)
{
}

/**
 * Nofication.beep - http://docs.phonegap.com/phonegap_notification_notification.md.html#notification.beep
 */
void Notification::beep( int scId, int ecId, int p_times ) {
    for( int i = 0; i < p_times; i++ ) {
        QApplication::beep();
    }
}

/**
 * Notification.vibrate - http://docs.phonegap.com/phonegap_notification_notification.md.html#notification.vibrate
 */
void Notification::vibrate( int scId, int ecId, int p_milliseconds ) {
    QFeedbackHapticsEffect vibrate;
    vibrate.setIntensity(1.0);
    vibrate.setDuration(p_milliseconds);
    vibrate.start();
}
