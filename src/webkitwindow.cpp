#include "webkitwindow.h"
#include "ui_webkitwindow.h"

#include <QWebFrame>
#include <QWebSettings>
#include <QDir>
#include <QMessageBox>

WebKitWindow::WebKitWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebKitWindow)
{
    ui->setupUi(this);
    webView = new QWebView(this);
    setCentralWidget(webView);
    webView->page()->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);
    QDir www = QDir::current();
    www.cd("www");
    connect(webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(add_js_extentions()));
    webView->load(QUrl::fromUserInput(www.filePath("index.html")));
}

void WebKitWindow::add_js_extentions()
{
    QWebFrame *f = webView->page()->mainFrame();
    f->addToJavaScriptWindowObject("extentions", &(this->ext));
}

WebKitWindow::~WebKitWindow()
{
    delete ui;
}
