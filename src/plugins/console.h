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
