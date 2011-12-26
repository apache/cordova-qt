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

#ifndef PHONEGAP_H
#define PHONEGAP_H

#include <QObject>
#include <QWebView>
#include <QMap>
#include <QDir>

#include "pgplugin.h"

class PhoneGap : public QObject
{
    Q_OBJECT
public:
    explicit PhoneGap(QWebView *webView);

signals:

public slots:
    void loadFinished( bool ok );

private:
    QWebView *m_webView;
    QDir m_workingDir;
};

#endif // PHONEGAP_H
