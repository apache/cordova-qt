/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

#ifndef CONTACTS_H
#define CONTACTS_H

#include "../cplugin.h"

#include <QHash>
#include <QSet>
#include <QVariantMap>
#include <QStringList>
#if QT_VERSION < 0x050000
# include <qmobilityglobal.h>
#else
# include <qcontactsglobal.h>
# include <QContactDetail>
#endif

#ifdef QTM_NAMESPACE
QTM_BEGIN_NAMESPACE
class QContact;
class QContactManager;
QTM_END_NAMESPACE
QTM_USE_NAMESPACE
#elif defined QTCONTACTS_USE_NAMESPACE
QTCONTACTS_BEGIN_NAMESPACE
class QContact;
class QContactManager;
QTCONTACTS_END_NAMESPACE
QTCONTACTS_USE_NAMESPACE
#endif


class Contacts : public CPlugin
{
    Q_OBJECT
public:
    explicit Contacts();

    void init();

signals:

public slots:
    void saveContact(int scId, int ecId, const QVariantMap &params);
    void removeContact(int scId, int ecId, const QString &localId);
    void findContacts(int scId, int ecId, const QStringList &fields, const QString &filter, bool multiple);

private:
    static Contacts *m_contacts;

#if QT_VERSION < 0x050000
    QString cordovaFieldNameToQtDefinition(const QString &cordovaFieldName) const;
#else
    QContactDetail::DetailType cordovaFieldNameToQtDefinition(const QString &cordovaFieldName) const;
    int subTypePhoneFromString(const QString &cordovaSubType) const;
    int subTypeOnlineAccountFromString(const QString &cordovaSubType) const;
    int subTypeUrlFromString(const QString &cordovaSubType) const;
    QString subTypePhoneToString(int qtSubType) const;
    QString subTypeOnlineAccountToString(int qtSubType) const;
    QString subTypeUrlToString(int qtSubType) const;
#endif
    QString jsonedContact(const QContact &contact, const QStringList &fields = QStringList()) const;

#if QT_VERSION < 0x050000
    QHash<QString, QString> m_fieldNamePairs;
#else
    QHash<QString, QContactDetail::DetailType> m_fieldNamePairs;
#endif
    QSet<QString> m_notSupportedFields;
    QContactManager *m_manager;
};

#endif // CONTACTS_H
