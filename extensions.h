#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <QMap>
#include <QWebView>


class Extensions : public QObject {

    Q_OBJECT

    public:
        explicit Extensions(QWebView *webView = 0);

    protected slots:
        void attachExtensions();

    private:
        QWebFrame *m_frame;
        QMap<QString, QObject *> m_extensions;
};

#endif // EXTENSIONS_H
