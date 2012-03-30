#ifndef CAMERARESOLUTION_H
#define CAMERARESOLUTION_H

#include <QObject>

class CameraResolution : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)

public:
    CameraResolution(int width, int height) : m_width(width), m_height(height) {}
    int width() { return m_width; }
    int height() { return m_height; }

    int m_width;
    int m_height;
};

#endif // CAMERARESOLUTION_H
