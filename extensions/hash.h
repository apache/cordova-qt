#ifndef HASH_H
#define HASH_H

#include <QObject>


class Hash : public QObject {

    Q_OBJECT

    public:
        explicit Hash(QObject *parent = 0);

        Q_INVOKABLE QString calculateHMACSHA1Signature(const QString &baseString, const QString &key);
};

#endif // HASH_H
