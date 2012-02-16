/*
 *  Copyright 2011 Wolfgang Koller - http://www.gofg.at/
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/cwebpage.h"

#include <QtCore/QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure web view
    ui->webView->settings()->enablePersistentStorage();
    ui->webView->settings()->setAttribute( QWebSettings::LocalStorageEnabled, true );
    ui->webView->settings()->setAttribute( QWebSettings::OfflineStorageDatabaseEnabled, true );
    ui->webView->settings()->setAttribute( QWebSettings::LocalContentCanAccessRemoteUrls, true );
    ui->webView->setPage( new CWebPage() );

    // Listen to load finished signal
    connect(ui->webView, SIGNAL(loadFinished(bool)),
            Cordova::instance(), SLOT(loadFinished(bool)));
    connect(Cordova::instance(), SIGNAL(javaScriptExecNeeded(QString)),
            this, SLOT(onJavaScriptExecNeeded(QString)));
    connect(Cordova::instance(), SIGNAL(pluginWantsToBeAdded(QString,QObject*,QString)),
            this, SLOT(onPluginWantsToBeAdded(QString,QObject*,QString)));
    connect(Cordova::instance(), SIGNAL(webViewAttributeChanged(QWebSettings::WebAttribute,bool)),
            this, SLOT(onWebViewAttributeChanged(QWebSettings::WebAttribute,bool)));
    ui->webView->load( Cordova::instance()->mainUrl() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

#if QT_VERSION < 0x050000
    Qt::WidgetAttribute attribute;
#endif
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#elif QT_VERSION < 0x050000
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#else // Qt 5
    // TODO: Implement for Qt 5
    case ScreenOrientationLockPortrait:
        break;
    case ScreenOrientationLockLandscape:
        break;
    default:
    case ScreenOrientationAuto:
        break;
#endif // QT_VERSION < 0x040702
    };
#if QT_VERSION < 0x050000
    setAttribute(attribute, true);
#endif
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::onJavaScriptExecNeeded(const QString &js)
{
    ui->webView->page()->mainFrame()->evaluateJavaScript(js);
}

void MainWindow::onPluginWantsToBeAdded(const QString &pluginName, QObject *pluginObject, const QString &pluginShortName)
{
    QString objectName = pluginShortName + "_native";
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject( objectName, pluginObject );
    ui->webView->page()->mainFrame()->evaluateJavaScript( "Cordova.Qt.registerObject( '" + pluginName + "', " + objectName + " )" );
}

void MainWindow::onWebViewAttributeChanged(QWebSettings::WebAttribute attribute, bool on)
{
    ui->webView->settings()->setAttribute(attribute, on);
}
