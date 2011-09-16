#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <QAccelerometer>
#include <QObject>

QTM_USE_NAMESPACE


class Accelerometer : public QObject {

    Q_OBJECT

    public:
        explicit Accelerometer(QObject *parent = 0);

        Q_INVOKABLE QVariantMap getCurrentAcceleration() const;

    protected slots:
        void updateSensor();

    private:
        QAccelerometer *m_accelerometer;

        double m_x;
        double m_y;
        double m_z;
};

#endif // ACCELEROMETER_H
