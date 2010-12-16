#include "accelerometer.h"

Accelerometer::Accelerometer(QObject *parent) :
    QObject(parent)
{
    accel = new QtMobility::QAccelerometer(this);
    accel->connectToBackend();
    connect(accel, SIGNAL(readingChanged()), this, SLOT(update_sensor()));
    accel->start();
    x = y = z = 0;
}

void Accelerometer::update_sensor()
{
    QtMobility::QAccelerometerReading *reading;
    reading = accel->reading();
    x = reading->x();
    y = reading->y();
    z = reading->z();
}
