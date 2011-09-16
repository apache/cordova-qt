#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class NetworkAccessManager : public QNetworkAccessManager {

    Q_OBJECT

    public:
        explicit NetworkAccessManager(QObject *parent = 0);

    private slots:
        void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // NETWORKACCESSMANAGER_H
