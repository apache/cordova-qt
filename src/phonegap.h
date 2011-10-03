/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#ifndef PHONEGAP_H
#define PHONEGAP_H

#include <QObject>
#include <QWebView>
#include <QMap>
#include <QDir>

#include "src/pgplugin.h"

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
