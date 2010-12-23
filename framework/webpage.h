#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>


class WebPage : public QWebPage {

    Q_OBJECT

    public:
        explicit WebPage(QObject *parent = 0);

    protected:
        void javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceId);

    private slots:
        void loadFinished(bool ok);
};

#endif // WEBPAGE_H
