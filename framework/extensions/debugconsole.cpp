#include "debugconsole.h"

#include <QDebug>


DebugConsole::DebugConsole(QObject *parent) :
    QObject(parent) {
}

void DebugConsole::log(const QString &message) {

    qDebug() << message;
}
