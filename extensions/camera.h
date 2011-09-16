/*
 * Camera.h
 *
 *  Created on: Jan 15, 2010
 *      Author: nitobi-test
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QSize>

#include "camerafileobserver.h"


class QImage;

class Camera: public QObject, MCameraFileObserver {

    Q_OBJECT

    public:
        explicit Camera(QObject *parent);
        virtual ~Camera();

        /**
         * Error codes passed with the error signal
         */
        enum ErrorCodes {
            ENoError = 0,
            ECameraObserverFailed,
            ECameraAppNotFound,
            ECameraAppFailed,
            ECaptureFileNotReadable,
            EVideoFileCaptured,
            EUnknownError
        };

        /**
         * Defines the how the captured picture is returned
         */
        enum EDestinationType {
            EDestinationDataUrl = 0,
            EDestinationFileUri = 1
        };

        /**
         * @Returns in which format a captured picture is returned to js-code
         */
        int destinationType() const;

        /**
         * Define in which format a captured picture is returned to js code
         * @param destination - valid values are defined in EDestinationType (C++) / camera.DestinationType (js)
         */
        void setDestinationType(int destination);

        Q_PROPERTY(int destinationType READ destinationType WRITE setDestinationType);

        /**
         * @Returns the image compression quality for captured pictures
         * @note Only for base64 encoded format, has no effect for EDestinationFileUri
         */
        int quality() const;

        /**
         * Sets the image compression quality for captured pictures
         * @param quality 0 - 100
         */
        void setQuality(int quality);

        Q_PROPERTY(int quality READ quality WRITE setQuality);

        /**
         * @Returns maximum width for the captured picture
         * @note If the picture taken by the camera is larger it will be scaled down (keeping it's aspect ratio)
         * but the picture will not be scaled up if it is smaller.
         * Only applies for base64 encoded format, ignored for EDestinationFileUri
         */
        int width() const;

        /**
         * Sets the maximum width for the captured picture
         * @param width
         */
        void setWidth(int width);

        Q_PROPERTY(int width READ width WRITE setWidth);

        /**
         * @Returns maximum height for the captured picture
         * @note If the picture taken by the camera is larger it will be scaled down (keeping it's aspect ratio)
         * but the picture will not be scaled up if it is smaller.
         * Only applies for base64 encoded format, ignored for EDestinationFileUri
         */
        int height() const;

        /**
         * Sets the maximum height for the captured picture
         * @param height
         */
        void setHeight(int height);
        Q_PROPERTY(int height READ height WRITE setHeight);

        /**
         * @reimp callback for CCameraFileObserver
         */
        virtual void NewCameraFileL(const TFileName &aFileName);

    public slots:
        void takePicture();
        void killCamera();

    signals:
        void pictureDataCaptured(const QString &data);
        void pictureFileCaptured(const QString &filename);
        void error(int errorCode, const QString &message);

    private:
        CCameraFileObserver *m_observer;
        bool m_waitingForPicture;
        int m_destination;
        int m_quality;
        QSize m_size;
};

#endif // CAMERA_H
