#ifndef DEVICE_H
#define DEVICE_H

#include "../pgplugin.h"

class Device : public PGPlugin
{
    Q_OBJECT
public:
    explicit Device();

signals:

public slots:
    void getInfo( int scId, int ecId );

private:
    static Device* m_device;
};

#endif // DEVICE_H
