#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <QObject>
#include <QAccelerometer>

class Accelerometer : public QObject
{
    Q_OBJECT
public:
    explicit Accelerometer(QObject *parent = 0);
    double x, y, z;

signals:

public slots:
    void update_sensor();

private:
    QtMobility::QAccelerometer *accel;

};

#endif // ACCELEROMETER_H
