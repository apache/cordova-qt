/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include "../pgplugin.h"

#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QList>

QTM_USE_NAMESPACE

class Geolocation : public PGPlugin
{
    Q_OBJECT
public:
    explicit Geolocation(QWebFrame *p_webFrame);

signals:

public slots:
    void getCurrentPosition( int scId, int ecId, QVariantMap p_options );

protected slots:
    void positionUpdated( const QGeoPositionInfo &update );
    void updateTimeout();

private:
    QGeoPositionInfoSource *m_geoPositionInfoSource;

    QList<int> m_successCallbacks;
    QList<int> m_errorCallbacks;
};

#endif // GEOLOCATION_H
