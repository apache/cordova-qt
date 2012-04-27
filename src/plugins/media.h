/*
 *  Copyright 2011  Integrated Computer Solutions - http://www.ics.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef MEDIA_H
#define MEDIA_H

#include "../cplugin.h"
#include <QtMultimediaKit/QMediaPlayer>
#include <QtMultimediaKit/QMediaRecorder>
#include <QUrl>
#include <QtMultimediaKit/QAudioCaptureSource>
#include <QtMultimediaKit/QAudioEncoderSettings>

//#ifdef QTM_NAMESPACE
//QTM_USE_NAMESPACE
//#endif

class Media : public CPlugin
{
    Q_OBJECT
public:
    explicit Media();
    void init();


signals:

public slots:
    void openMedia( int scId, int ecId, QVariantMap p_options);
    void getCurrentPosition( int scId, int ecId, QVariantMap p_options);
    long getDuration( int scId, int ecId, QVariantMap p_options);
    void play( int scId, int ecId, QVariantMap p_options);
    void pause( int scId, int ecId, QVariantMap p_options);
    void relase( int scId, int ecId, QVariantMap p_options);
    void stop( int scId, int ecId, QVariantMap p_options);
    void startRecording( int scId, int ecId, QVariantMap p_options);
    void stopRecording( int scId, int ecId, QVariantMap p_options);
    void seekTo( int scId, int ecId, QVariantMap p_options);

protected slots:
    void updateCurrentPosition(long position);

private:
    static Media *m_media;

    long m_position;
    long m_duration;

    QUrl audioPlayerFile;
    QUrl audioRecorderFile;

    QMediaPlayer *player;
    QMediaRecorder *recorder;

    QList<int> m_successCallbacks;
    QList<int> m_errorCallbacks;
};

#endif // MEDIA_H
