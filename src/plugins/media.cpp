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

#include "media.h"
#include "../pluginregistry.h"
#include <QDebug>

Media* Media::m_media= new Media();


Media::Media() :CPlugin()
{
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Media", this );
}

void Media::init() {
    m_position=0;
    m_duration=0;
}

void Media::openMedia( int scId, int ecId, QVariantMap p_options){
//    audioPlayerFile=new QUrl(p_options["source"].toString());
    audioPlayerFile = QUrl::fromLocalFile(p_options["source"].toString());


    // Create and initialize the player
    player = new QMediaPlayer;
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updateCurrentPosition(long)));
    player->setMedia(audioPlayerFile);
    player->setVolume(50);
    m_duration=player->duration();

    // Create and initialize the recorder
    QAudioCaptureSource *audioSource = new QAudioCaptureSource();
    recorder = new QMediaRecorder(audioSource);

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/vorbis");
    //audioSettings.setQuality(QtMedia::HighQuality);

    recorder->setEncodingSettings(audioSettings);
    recorder->setOutputLocation(audioPlayerFile);
}

void Media::updateCurrentPosition(long position){
    m_position=position;
}

void Media::getCurrentPosition( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    m_successCallbacks << scId;
    m_errorCallbacks << ecId;

    QString callbackArguments = QString::number(m_position);

    for( int i = 0; i < m_successCallbacks.size(); i++ ) {
        this->callback( m_successCallbacks.at( i ), callbackArguments );
    }

    m_errorCallbacks.clear();
    m_successCallbacks.clear();
}

long Media::getDuration( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    return m_duration;
}

void Media::play( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    player->play();
}

void Media::pause( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    player->pause();
}

void Media::relase( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

}

void Media::stop( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    player->stop();
}

void Media::startRecording( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    recorder->record();
}

void Media::stopRecording( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    recorder->stop();
}

void Media::seekTo( int scId, int ecId, QVariantMap p_options){
    Q_UNUSED(p_options)
    Q_UNUSED(scId)
    Q_UNUSED(ecId)

    player->setPosition((qint64)p_options["source"].toInt());
}
