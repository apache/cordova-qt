/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2011, Wolfgang Koller - http://www.gofg.at/
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "../pgplugin.h"

class Device : public PGPlugin
{
    Q_OBJECT
public:
    explicit Device();

signals:

public slots:
    void getInfo( int scId, int ecId );

private:
    static Device* m_device;
};

#endif // DEVICE_H
