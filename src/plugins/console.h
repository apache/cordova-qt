/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

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
