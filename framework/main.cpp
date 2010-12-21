#include <QtGui/QApplication>
#include "webkitwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebKitWindow w;
#if defined(Q_WS_S60)
    w.showFullScreen();
#else
    w.show();
#endif

    return a.exec();
}
