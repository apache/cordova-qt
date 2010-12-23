#include "notification.h"

#ifdef Q_WS_S60
#include <vibra/XQVibra.h>
#endif


Notification::Notification(QObject *parent) :
    QObject(parent),
    m_vibra(0) {

#ifdef Q_WS_S60
    m_vibra = new XQVibra(this);
#endif
}

void Notification::vibrate(int duration, int intensity) {

#ifdef Q_WS_S60
    XQVibra *vibra = qobject_cast<XQVibra *>(m_vibra);
    vibra->setIntensity(intensity);
    vibra->start(duration);
#else
    Q_UNUSED(duration);
    Q_UNUSED(intensity);
#endif
}
