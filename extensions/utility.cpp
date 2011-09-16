#include "utility.h"

#include <QCoreApplication>


Utility::Utility(QObject *parent) :
    QObject(parent) {
}

void Utility::exit() {

    qApp->quit();
}
