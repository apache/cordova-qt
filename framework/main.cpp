#include "mainwindow.h"

#include <QApplication>


bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event) {

    return QCoreApplication::sendSpontaneousEvent(receiver, event);
}

int main(int argc, char *argv[]) {

#if !defined(Q_WS_S60)
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication app(argc, argv);
    app.setApplicationName("PhoneGap");
    app.setApplicationVersion("0.0.0");

    MainWindow window;
#ifdef Q_OS_SYMBIAN
    window.showMaximized();
#else
    window.resize(360, 640);
    window.show();
#endif

#ifdef QT_KEYPAD_NAVIGATION
    QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
#endif

    return app.exec();
}
