#ifndef CONSOLE_H
#define CONSOLE_H

#include "../pgplugin.h"

#include <QString>

class Console : public PGPlugin
{
    Q_OBJECT
public:
    explicit Console();

signals:

public slots:
    void log( int scId, int ecId, QString p_message );

private:
    static Console *m_console;
};

#endif // CONSOLE_H
