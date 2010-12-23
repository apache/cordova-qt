#include "accelerometer.h"


Accelerometer::Accelerometer(QObject *parent) :
    QObject(parent),
    m_x(0),
    m_y(0),
    m_z(0) {

    m_accelerometer = new QAccelerometer(this);
    m_accelerometer->connectToBackend();
    m_accelerometer->start();

    connect(m_accelerometer, SIGNAL(readingChanged()), SLOT(updateSensor()));
}

QVariantMap Accelerometer::getCurrentAcceleration() const {

    QVariantMap map;
    map["x"] = m_x;
    map["y"] = m_y;
    map["z"] = m_z;
    return map;
}

void Accelerometer::updateSensor() {

    QAccelerometerReading *reading = m_accelerometer->reading();
    m_x = reading->x();
    m_y = reading->y();
    m_z = reading->z();
}
