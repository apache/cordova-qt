#include "cookiejar.h"

#include <QByteArray>
#include <QSettings>
#include <QUrl>

CookieJar::CookieJar(QObject *parent) :
        QNetworkCookieJar(parent) {

    loadConfig();
}

CookieJar::~CookieJar() {

    saveConfig();
}

void CookieJar::saveConfig() const {

    QList<QNetworkCookie> cookies = allCookies();

    QSettings settings;
    settings.beginWriteArray("Cookies/cookie");
    int i = 0;
    foreach (QNetworkCookie cookie, cookies) {
        if (cookie.isSessionCookie()) {
            continue;
        }
        QByteArray cookieString = cookie.toRawForm();
        settings.setArrayIndex(i);
        settings.setValue("content", cookieString);
        i++;
    }
    settings.endArray();
}

void CookieJar::loadConfig() {

    QList<QNetworkCookie> cookies;

    QSettings settings;
    int size = settings.beginReadArray("Cookies/cookie");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        QByteArray cookieString = settings.value("content").toByteArray();
        QList<QNetworkCookie> cookieList = QNetworkCookie::parseCookies(cookieString);
        cookies += cookieList;
    }
    settings.endArray();

    setAllCookies(cookies);
}
