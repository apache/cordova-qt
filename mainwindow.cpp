#include "extensions.h"
#include "mainwindow.h"
#include "webpage.h"

#include <QDebug>
#include <QDir>
#include <QWebSettings>
#include <QWebView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    QWebView *webView = new QWebView(this);
    webView->setPage(new WebPage());

    setCentralWidget(webView);
    webView->page()->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);

    new Extensions(webView);

    QDir templateDir = QDir::current();
#ifdef Q_WS_SIMULATOR
    templateDir.cdUp();
#endif
    templateDir.cd("template");

    qDebug() << "Loading file: " << templateDir.filePath("index.html");

    webView->load(QUrl::fromUserInput(templateDir.filePath("index.html")));
    webView->settings()->enablePersistentStorage();
}

MainWindow::~MainWindow() {
}
