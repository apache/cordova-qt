#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>


class CookieJar : public QNetworkCookieJar {

    Q_OBJECT

    public:
        explicit CookieJar(QObject *parent = 0);
        virtual ~CookieJar();

    private:
        void saveConfig() const;
        void loadConfig();
};

#endif // COOKIEJAR_H
