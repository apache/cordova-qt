#ifndef PGPLUGIN_H
#define PGPLUGIN_H

#include <QObject>
#include <QWebFrame>

class PGPlugin : public QObject
{
    Q_OBJECT
public:
    explicit PGPlugin(QWebFrame *p_webFrame);

signals:

public slots:

protected:
    void callback( int p_callbackId, QString p_jsParameters );

    QWebFrame *m_webFrame;
};

#endif // PGPLUGIN_H
