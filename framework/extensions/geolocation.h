/*
 * Geolocation.h
 *
 *  Created on: Nov 24, 2009
 *      Author: nitobi-test
 */

#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include <QObject>
#include <QVariantMap>

#include <qgeopositioninfo.h>
#include <qgeopositioninfosource.h>

QTM_USE_NAMESPACE


class Geolocation : public QObject {

    Q_OBJECT

    public:
        Geolocation(QObject *parent = 0);

    public slots:
        void start(int interval = 1000);
        void stop();

    signals:
        void positionUpdated(const QVariantMap &position);
        void error(const QVariantMap &positionError);

    private:
        QGeoPositionInfoSource *m_source;

        void emitError(int code, const QString &message);

    private slots:
        void onPositionUpdated(const QGeoPositionInfo &info);
        void onUpdateTimeout();
};

#endif // GEOLOCATION_H
