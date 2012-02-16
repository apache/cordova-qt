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


#include <QApplication>

#include "src/cordova.h"

#if QT_VERSION < 0x050000
# include "mainwindow.h"
#else
# include <QQuickView>
# include <QDeclarativeContext>
# include <QDeclarativeEngine>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#if QT_VERSION < 0x050000
    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showExpanded();
#else
    QQuickView view;
    view.rootContext()->setContextProperty("cordova", Cordova::instance());
    view.setSource(QUrl(QString("%1/qml/main.qml").arg(QApplication::applicationDirPath())));
    view.show();
#endif

    return app.exec();
}
