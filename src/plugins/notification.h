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

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "../cplugin.h"

class Notification : public CPlugin
{
    Q_OBJECT
public:
    explicit Notification();

signals:

public slots:
    void beep( int scId, int ecId, int p_times );
    void vibrate( int scId, int ecId, int p_milliseconds );

private slots:
    void deleteEffectAtStateStopped();

private:
    static Notification *m_notification;
};

#endif // NOTIFICATION_H
