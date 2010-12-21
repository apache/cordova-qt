#include "mobileextensions.h"
#include <vibra/XQVibra.h>
#include <QMessageBox>

MobileExtensions::MobileExtensions(QObject *parent, QWebView *webView) :
    QObject(parent)
{
#if defined(Q_WS_S60)
        vib = new XQVibra(this);
#endif
    this->webView = webView;
    accel = 0;
}

void MobileExtensions::vibrate(int milis)
{
#if defined(Q_WS_S60)
        vib->start(milis);
#else
        QMessageBox m(QMessageBox::Information, QString("vib"), QString("vibrate!"));
        m.exec();
#endif
}

QVariantList MobileExtensions::getCurrentAcceleration()
{
    QVariantList res;
    if (accel == 0) {
        accel = new Accelerometer(this);
    }
    res.append(accel->x);
    res.append(accel->y);
    res.append(accel->z);
    return res;
}
