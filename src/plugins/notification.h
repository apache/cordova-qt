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
