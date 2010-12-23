#include "cookiejar.h"
#include "networkaccessmanager.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QSslError>


NetworkAccessManager::NetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent) {

    QNetworkDiskCache *cache = new QNetworkDiskCache(this);
    cache->setMaximumCacheSize(50 * 1024 * 1024);
    cache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/cache");
    setCache(cache);

    setCookieJar(new CookieJar(this));

    connect(this, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &)));
}

void NetworkAccessManager::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {

    foreach (const QSslError &error, errors) {
        QMessageBox::warning(0, tr("SSL error"), error.errorString());
    }

    reply->ignoreSslErrors();
}
