#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include <QObject>


class DebugConsole : public QObject {

    Q_OBJECT

    public:
        DebugConsole(QObject *parent = 0);

        Q_INVOKABLE void log(const QString &message);
};

#endif // DEBUGCONSOLE_H
