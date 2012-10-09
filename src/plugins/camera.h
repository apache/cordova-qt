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

#ifndef CAMERA_H
#define CAMERA_H

#include "../cplugin.h"
#include "../pluginregistry.h"

#include <QObject>
#include <QUrl>
#include <QList>
#include <QSize>
#include <QVariantMap>

#include <qplatformdefs.h>

#ifdef Q_OS_SYMBIAN
#include <ecam.h>
#endif

#ifdef QTM_NAMESPACE
QTM_USE_NAMESPACE
#endif

class Camera : public CPlugin
#ifdef Q_OS_SYMBIAN
  , public MCameraObserver
#endif
{

    Q_OBJECT
public:
    explicit Camera();
    void init();
    ~Camera();

    QString newImageFile(int width, int height);
    QUrl newImageUrl(int width, int height);
    QList<QSize> supportedResolutions();


signals:

public slots:
    void getPicture( int scId, int ecId, QVariantMap p_options);

private slots:
#ifdef MEEGO_EDITION_HARMATTAN
    void captureCanceled(const QString &mode);
    void captureCompleted(const QString &mode, const QString &fileName);
#endif

private:
#ifdef Q_OS_SYMBIAN
    QString symbianCapture(int width, int height);
    void symbianEnumerateResolutions();
    void FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError) {}
    void ImageReady(CFbsBitmap* aBitmap,HBufC8* aData,TInt aError) {}
    void ViewFinderFrameReady(CFbsBitmap& aFrame) {}
    void PowerOnComplete(TInt aError) {};
    void ReserveComplete(TInt aError) {};
#elif defined MEEGO_EDITION_HARMATTAN
    void startCamera();
    int m_lastScId;
    int m_lastEcId;
#endif
    void getSupportedResolutions();
    QList<QObject*> supportedResolutionObjects();

    QList<QSize> m_supportedResolutions;
    QList<QObject*> m_supportedResolutionObjects;

    static Camera *m_camera;
};

#endif // CAMERA_H
