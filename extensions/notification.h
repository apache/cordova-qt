#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>


class Notification : public QObject {

    Q_OBJECT

    public:
        explicit Notification(QObject *parent = 0);

    public slots:
        void vibrate(int duration = 5000, int intensity = 50);

    private:
        QObject *m_vibra;
};

#endif // NOTIFICATION_H
