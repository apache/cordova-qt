/*
 * Camera.cpp
 *
 *  Created on: Jan 15, 2010
 *      Author: nitobi-test
 */

#include "camera.h"

#include <e32std.h>

#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QPixmap>
#include <QProcess>
#include <QWidget>
#include <QWidgetList>


const QString CAMERA_APP_PATH = "Z:\\sys\\bin\\cameraapp.exe";

// default size for captured picture (applies to base64 encoded format only)
const int MAX_IMG_WIDTH = 1024;
const int MAX_IMG_HEIGHT = 768;

Camera::Camera(QObject *parent) :
    QObject(parent),
    MCameraFileObserver(),
    m_observer(0),
    m_waitingForPicture(false),
    m_destination(EDestinationDataUrl),
    m_quality(75),
    m_size(MAX_IMG_WIDTH, MAX_IMG_HEIGHT) {

    TRAPD(iError, m_observer = CCameraFileObserver::NewL(*this);)
    if (iError != KErrNone) {
        m_observer = 0;
        qDebug() << "Camera::Camera: failed to instantiate CCameraFileObserver: error " << iError;
        return;
    }
    m_observer->Start();
}

Camera::~Camera() {

    delete m_observer;
}

int Camera::destinationType() const {

    return m_destination;
}

void Camera::setDestinationType(int destination) {

    if (destination != EDestinationDataUrl && destination != EDestinationFileUri) {
        qDebug() << "Camera::setDestinationType: invalid value: " << destination;
        return;
    }

    m_destination = destination;
}

int Camera::quality() const {

    return m_quality;
}

void Camera::setQuality(int quality) {

    if (quality < 0 || quality > 100) {
        qDebug() << "Camera::setQuality: value out of range: " << quality;
        return;
    }

    m_quality = quality;
}

int Camera::width() const {

    return m_size.width();
}

void Camera::setWidth(int width) {

    m_size.setWidth(width);
}

int Camera::height() const {

    return m_size.height();
}

void Camera::setHeight(int height) {

    m_size.setHeight(height);
}

void Camera::takePicture() {

    if (!m_observer) {
        emit error(ECameraObserverFailed, "CameraFileObserver failed to register");
        return;
    }

    qDebug() << "Camera::takePicture: Starting camera app";
    if (!QProcess::startDetached(CAMERA_APP_PATH)) {
        QFile::exists(CAMERA_APP_PATH) ?
            emit error(ECameraAppFailed, "Camera application failed to start") :
            emit error(ECameraAppNotFound, "Could not find camera application");
        return;
    }
    m_waitingForPicture = true;
}

void Camera::killCamera() {

    m_waitingForPicture = false;
    TRAPD(iError,
        TFindProcess processFinder(_L("*"));
        TFullName result;
        RProcess processHandle;
        while (processFinder.Next(result) == KErrNone) {
            User::LeaveIfError(processHandle.Open(processFinder, EOwnerThread));
            TFileName fileName = processHandle.FileName();
            if (QString((QChar*) fileName.Ptr(), fileName.Length()) == CAMERA_APP_PATH) {
                qDebug() << "Camera::killCamera: killing process " << QString((QChar*) fileName.Ptr(), fileName.Length());
                processHandle.Kill(KErrNone);
            }
            processHandle.Close();
        })
    if (iError != KErrNone) {
        qDebug() << "Camera::killCamera: failed iError=" << iError;
    }
}

void Camera::NewCameraFileL(const TFileName& aFileName) {

    // we get events for ALL pictures taken by camera app - ignore them if camera app wasn't launched by us
    if (!m_waitingForPicture) {
        return;
    }

    killCamera();
    QString filename((QChar*) aFileName.Ptr(), aFileName.Length());

    if (filename.endsWith(".avi")) {
        qDebug() << "Camera::NewCameraFileL: video captured: " << filename << " (can handle images only)";
        emit error(EVideoFileCaptured, "A video was captured (can handle images only)");
        return;
    }

    qDebug() << "Camera::NewCameraFileL: file captured: " << filename;

    if (m_destination == EDestinationFileUri) {
        emit pictureFileCaptured(filename);
        return;
    }

    // read picture, resize and send to webkit as base64-encoded string
    QImageReader reader(filename);
    QSize maxSize(MAX_IMG_WIDTH, MAX_IMG_HEIGHT);
    QSize imgSize(reader.size());
    if (imgSize.width() > imgSize.height()) { // landscape/portrait format?
        maxSize.transpose();
    }
    if (imgSize.width() > maxSize.width() || imgSize.height() > maxSize.height()) { // scale image if necessary
        imgSize.scale(maxSize, Qt::KeepAspectRatio);
        reader.setScaledSize(imgSize);
    }

    QImage *img = new QImage();
    if (!reader.read(img)) {
        delete img;
        qDebug() << "Camera::NewCameraFileL: Failed to read captured file: " << reader.errorString() << " (" << reader.error() << ")";
        emit error(ECaptureFileNotReadable, "Failed to open/read captured file: " + reader.errorString());
        return;
    }

    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    img->save(&buffer, "JPG", m_quality);
    buffer.close();
    delete img;
    emit pictureDataCaptured(data.toBase64());
}
