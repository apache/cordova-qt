#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "../pgplugin.h"

class Notification : public PGPlugin
{
    Q_OBJECT
public:
    explicit Notification(QWebFrame *p_webFrame);

signals:

public slots:
    void beep( int scId, int ecId, int p_times );
    void vibrate( int scId, int ecId, int p_milliseconds );
};

#endif // NOTIFICATION_H
