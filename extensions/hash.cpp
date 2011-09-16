#include "hash.h"

#include <QCryptographicHash>


Hash::Hash(QObject *parent) :
    QObject(parent) {
}

QString Hash::calculateHMACSHA1Signature(const QString &baseString, const QString &_key) {

    QByteArray key = _key.toUtf8();
    if (key.size() > 64) {
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray okey = key.leftJustified(64, 0);
    QByteArray ikey = key.leftJustified(64, 0);

    for (int i = 0; i < 64; i++) {
        okey[i] = okey[i] ^ 0x5c;
        ikey[i] = ikey[i] ^ 0x36;
    }

    okey.append(QCryptographicHash::hash(ikey.append(baseString), QCryptographicHash::Sha1));
    return QCryptographicHash::hash(okey, QCryptographicHash::Sha1).toBase64();
}
