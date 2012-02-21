/*
 *  Copyright 2011 Wolfgang Koller - http://www.gofg.at/
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

#ifndef CORDOVA_H
#define CORDOVA_H

#include <QObject>
#include <QMap>
#include <QDir>

#if QT_VERSION < 0x050000
# include <QWebSettings>
#endif

#include "cplugin.h"

class Cordova : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mainUrl READ mainUrl CONSTANT)

public:
    static Cordova *instance();

    QString workingDir() const;
    QString mainUrl() const;
    void setTopLevelEventsReceiver(QObject *obj);
    QObject *topLevelEventsReceiver();

signals:
    void javaScriptExecNeeded(const QString &js);
    void pluginWantsToBeAdded(const QString &pluginName, QObject *pluginObject, const QString &pluginShortName);
#if QT_VERSION < 0x050000
    void webViewAttributeChanged(QWebSettings::WebAttribute attribute, bool on);
#endif

public slots:
    void loadFinished( bool ok );
    void execJS(const QString &js);

private:
    explicit Cordova(QObject *parent = 0);
    Q_DISABLE_COPY(Cordova)

    static Cordova *m_instance;

    QObject *m_topLevelEventsReceiver;

    QDir m_workingDir;
    QString m_mainUrl;
};

#endif // CORDOVA_H
