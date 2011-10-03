/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "../pgplugin.h"

class Notification : public PGPlugin
{
    Q_OBJECT
public:
    explicit Notification();

signals:

public slots:
    void beep( int scId, int ecId, int p_times );
    void vibrate( int scId, int ecId, int p_milliseconds );

private:
    static Notification *m_notification;
};

#endif // NOTIFICATION_H
