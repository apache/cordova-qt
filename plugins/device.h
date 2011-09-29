#ifndef DEVICE_H
#define DEVICE_H

#include "../pgplugin.h"

class Device : public PGPlugin
{
    Q_OBJECT
public:
    explicit Device(QWebFrame *p_webFrame);

signals:

public slots:
    void init( int scId, int ecId );
};

#endif // DEVICE_H
