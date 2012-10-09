/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

#include "cameraresolution.h"
#include "camera.h"


#ifdef Q_OS_SYMBIAN
# include <ecam.h>
# include <NewFileServiceClient.h>
# include <AiwServiceHandler.h>
# include <AiwCommon.hrh>
# include <AiwGenericParam.hrh>
#elif defined MEEGO_EDITION_HARMATTAN
# include <QDBusMessage>
# include <QDBusConnection>
#endif


#include <QDebug>

// This is returned as the path to the captured image on platforms other than Symbian
#define DUMMY_IMAGE "dummy.jpg"

// These define the (only) supported resolution on platforms other than Symbian
#define DUMMY_WIDTH 1024
#define DUMMY_HEIGHT 768



Camera *Camera::m_camera = new Camera;

Camera::Camera() : CPlugin(){
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Camera", this );
}

Camera::~Camera()
{
    qDeleteAll(m_supportedResolutionObjects);
}

void Camera::init(){
    getSupportedResolutions();
#ifdef MEEGO_EDITION_HARMATTAN
    m_lastScId = 0;
    m_lastEcId = 0;
#endif
}

void Camera::getPicture( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options);

    // Should we select the best resolution here?
#ifdef MEEGO_EDITION_HARMATTAN
    m_lastScId = scId;
    m_lastEcId = ecId;
    startCamera();
#else
    Q_UNUSED(ecId);

    QString callbackArguments = newImageFile(m_supportedResolutions.at(0).width(),m_supportedResolutions.at(0).height());

    this->callback( scId, callbackArguments );
#endif
}

#ifdef MEEGO_EDITION_HARMATTAN
void Camera::captureCanceled(const QString &mode)
{
    Q_UNUSED(mode);
    qDebug() << Q_FUNC_INFO;
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.disconnect("com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "captureCanceled",
                this, SLOT(captureCanceled(QString)));

    bus.disconnect("com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "captureCompleted",
                this, SLOT(captureCompleted(QString,QString)));
    if (m_lastEcId)
        this->callback(m_lastEcId, "");
}

void Camera::captureCompleted(const QString &mode, const QString &fileName)
{
    Q_UNUSED(mode);
    qDebug() << Q_FUNC_INFO;
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.disconnect("com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "captureCanceled",
                this, SLOT(captureCanceled(QString)));

    bus.disconnect("com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "captureCompleted",
                this, SLOT(captureCompleted(QString,QString)));
    if (m_lastScId)
        this->callback(m_lastScId, QString("\"%1\"").arg(fileName));
}

void Camera::startCamera()
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    bus.connect("com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "captureCanceled",
                this, SLOT(captureCanceled(QString)));

    bus.connect("com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "captureCompleted",
                this, SLOT(captureCompleted(QString,QString)));

    QDBusMessage message = QDBusMessage::createMethodCall(
                "com.nokia.maemo.CameraService",
                "/",
                "com.nokia.maemo.meegotouch.CameraInterface",
                "showCamera");

    QList<QVariant> args;
    args << (uint)0 << "" << "still-capture" << true;
    message.setArguments(args);

    QDBusMessage reply = bus.call(message);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "reply.type == errormessage; name=" << reply.errorName() << "; message=" << reply.errorMessage();
        bus.disconnect("com.nokia.maemo.CameraService",
                    "/",
                    "com.nokia.maemo.meegotouch.CameraInterface",
                    "captureCanceled",
                    this, SLOT(captureCanceled(QString)));

        bus.disconnect("com.nokia.maemo.CameraService",
                    "/",
                    "com.nokia.maemo.meegotouch.CameraInterface",
                    "captureCompleted",
                    this, SLOT(captureCompleted(QString,QString)));
        if (m_lastEcId)
            this->callback(m_lastEcId, "");
    }
}
#endif

QString Camera::newImageFile(int width, int height)
{
    qDebug() << Q_FUNC_INFO;
#ifndef Q_OS_SYMBIAN
    Q_UNUSED(width);
    Q_UNUSED(height);
#endif
#ifdef Q_OS_SYMBIAN
    QString filename;
//    TRAPD(err, filename = symbianCapture(width, height));
    filename = symbianCapture(width, height);
//    if (err != KErrNone)
//        emit error(err);
        qDebug() << "Error";
    return filename;
#else
    return DUMMY_IMAGE;
#endif
}

QUrl Camera::newImageUrl(int width, int height)
{
    QString filename = newImageFile(width, height);
    if (filename.isEmpty())
        return QUrl();
    else
        return QUrl::fromLocalFile(filename);
}


QList<QSize> Camera::supportedResolutions()
{
    if (m_supportedResolutions.isEmpty())
        getSupportedResolutions();
    return m_supportedResolutions;
}

void Camera::getSupportedResolutions()
{
#ifdef Q_OS_SYMBIAN
    TRAPD(err, symbianEnumerateResolutions());
#else
    m_supportedResolutions.append(QSize(DUMMY_WIDTH, DUMMY_HEIGHT));
    m_supportedResolutionObjects.append(new CameraResolution(DUMMY_WIDTH, DUMMY_HEIGHT));
#endif
}

QList<QObject*> Camera::supportedResolutionObjects()
{
    if (m_supportedResolutionObjects.isEmpty())
        getSupportedResolutions();
    return m_supportedResolutionObjects;
}

#ifdef Q_OS_SYMBIAN
QString Camera::symbianCapture(int width, int height)
{
    CNewFileServiceClient* fileClient = NewFileServiceFactory::NewClientL();
    CleanupStack::PushL(fileClient);

    CDesCArray* fileNames = new (ELeave) CDesCArrayFlat(1);
    CleanupStack::PushL(fileNames);

    CAiwGenericParamList* paramList = CAiwGenericParamList::NewLC();

    TSize resolution = TSize(width, height);
    TPckgBuf<TSize> buffer( resolution );
    TAiwVariant resolutionVariant( buffer );
    TAiwGenericParam param( EGenericParamResolution, resolutionVariant );
    paramList->AppendL( param );

    const TUid KUidCamera = { 0x101F857A }; // Camera UID for S60 5th edition

    TBool result = fileClient->NewFileL( KUidCamera, *fileNames, paramList,
                               ENewFileServiceImage, EFalse );

    QString ret;

    if (result) {
        TPtrC fileName=fileNames->MdcaPoint(0);
        ret = QString((QChar*) fileName.Ptr(), fileName.Length());
    }
     qDebug() << ret;
    CleanupStack::PopAndDestroy(3);

    return ret;
}

void Camera::symbianEnumerateResolutions()
{
    CCamera* camera = CCamera::NewL(*this, 0);
    TCameraInfo info;
    camera->CameraInfo(info);
    int resolutionCount = info.iNumImageSizesSupported;
    qDebug() << resolutionCount;
    for (int i=0; i < resolutionCount; i++) {
        TSize size;
        camera->EnumerateCaptureSizes(size, i, CCamera::EFormatExif);
        qDebug() << size.iWidth << size.iHeight;
        m_supportedResolutions.append(QSize(size.iWidth, size.iHeight));
        m_supportedResolutionObjects.append(new CameraResolution(size.iWidth, size.iHeight));
    }
    qDebug() << m_supportedResolutionObjects.length();
    delete camera;

}
#endif

