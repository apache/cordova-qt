#ifndef MOBILEEXTENSIONS_H
#define MOBILEEXTENSIONS_H

#include <QObject>
#if defined(Q_WS_S60)
    #include <vibra/xqvibra.h>
#endif
#include <QWebView>
#include <QVariantList>
#include "deviceinfo.h"

#include "accelerometer.h"

class MobileExtensions : public QObject
{
    Q_OBJECT
public:
    explicit MobileExtensions(QObject *parent = 0, QWebView *webView = 0);

signals:

public slots:
    void vibrate(int milis);
    QVariantList getCurrentAcceleration();

private:
#if defined(Q_WS_S60)
        XQVibra *vib;
#endif
    QWebView *webView;
    Accelerometer *accel;
    DeviceInfo device_info;
};

#endif // MOBILEEXTENSIONS_H
