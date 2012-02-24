/*
 *  Copyright 2011  Integrated Computer Solutions - http://www.ics.com
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

#ifndef COMPASS_H
#define COMPASS_H

#include "../cplugin.h"
#include <QCompass>
#include <QList>

#ifdef QTM_NAMESPACE
QTM_USE_NAMESPACE
#endif


class Compass : public CPlugin
{
    Q_OBJECT
public:
    explicit Compass();
    void init();


signals:

public slots:
    void getCurrentHeading( int scId, int ecId, QVariantMap p_options);

protected slots:
    void updateSensor();

private:
    static Compass *m_compass;
    QCompass* m_compassSource;
    QList<int> m_successCallbacks;
    QList<int> m_errorCallbacks;
    double m_azymuth;
    double m_accuracy;
};

#endif // COMPASS_H
