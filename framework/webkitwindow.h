#ifndef WEBKITWINDOW_H
#define WEBKITWINDOW_H

#include <QMainWindow>
#include "mobileextensions.h"

namespace Ui {
    class WebKitWindow;
}

class WebKitWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WebKitWindow(QWidget *parent = 0);
    MobileExtensions ext;
    ~WebKitWindow();

public slots:
    void add_js_extentions();

private:
    Ui::WebKitWindow *ui;
    QWebView *webView;
};

#endif // WEBKITWINDOW_H
