#ifndef PGPLUGIN_H
#define PGPLUGIN_H

#include <QObject>
#include <QWebFrame>

class PGPlugin : public QObject
{
    Q_OBJECT
public:
    explicit PGPlugin();

    void setWebFrame(QWebFrame *p_webFrame);

    // This function should be overwritten, if the Plugin requires startup initialisation
    virtual void init();

signals:

public slots:

protected:
    void callback( int p_callbackId, QString p_jsParameters );

    QWebFrame *m_webFrame;
};

#endif // PGPLUGIN_H
