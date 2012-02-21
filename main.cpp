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
# include <QDeclarativeView>
#else
# include <QQuickView>
#endif

#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <qplatformdefs.h>

#ifdef MEEGO_EDITION_HARMATTAN
# include <MDeclarativeCache>
#endif

#ifdef MEEGO_EDITION_HARMATTAN
Q_DECL_EXPORT int main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{

#ifdef MEEGO_EDITION_HARMATTAN
    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
#else
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
#endif

#if QT_VERSION < 0x050000
# ifdef MEEGO_EDITION_HARMATTAN
    QScopedPointer<QDeclarativeView> view(MDeclarativeCache::qDeclarativeView());
# else
    QScopedPointer<QDeclarativeView> view(new QDeclarativeView());
# endif
    Cordova::instance()->setTopLevelEventsReceiver(view.data());
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty("cordova", Cordova::instance());
# ifdef MEEGO_EDITION_HARMATTAN
    view->setSource(QUrl(QString("%1/qml/main_harmattan.qml").arg(Cordova::instance()->workingDir())));
    view->showFullScreen();
# else
    view->setSource(QUrl(QString("%1/qml/main.qml").arg(Cordova::instance()->workingDir())));
    view->show();
# endif
#else // QT_VERSION >= 0x050000
    QQuickView view;
    Cordova::instance()->setTopLevelEventsReceiver(&view);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("cordova", Cordova::instance());
    view.setSource(QUrl(QString("%1/qml/main_qt5.qml").arg(Cordova::instance()->workingDir())));
    view.show();
#endif

    return app->exec();
}
