/*
 *  Copyright 2011 Wolfgang Koller - http://www.gofg.at/
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
