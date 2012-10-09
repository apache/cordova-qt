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

#include "contacts.h"
#include "../pluginregistry.h"

#include <QMap>
#include <QContactDisplayLabel>
#include <QContactName>
#include <QContactNickname>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactAddress>
#include <QContactOnlineAccount>
#include <QContactOrganization>
#include <QContactBirthday>
#include <QContactNote>
#include <QContactAvatar>
#include <QContactUrl>
#include <QContact>
#include <QContactManager>
#include <QContactUnionFilter>
#include <QContactDetailFilter>
#include <QDebug>

#ifdef QTM_USE_NAMESPACE
QTM_USE_NAMESPACE
#elif defined QTCONTACTS_USE_NAMESPACE
QTCONTACTS_USE_NAMESPACE
#endif



Contacts *Contacts::m_contacts = new Contacts();

Contacts::Contacts() :
    CPlugin()
{
    PluginRegistry::getRegistry()->registerPlugin( "com.cordova.Contacts", this );
}

void Contacts::init()
{
    m_fieldNamePairs.clear();

#if QT_VERSION < 0x050000
    m_fieldNamePairs["displayName"] = (QLatin1String)QContactDisplayLabel::DefinitionName;
    m_fieldNamePairs["name"] = (QLatin1String)QContactName::DefinitionName;
    m_fieldNamePairs["nickname"] = (QLatin1String)QContactNickname::DefinitionName;
    m_fieldNamePairs["phoneNumbers"] = (QLatin1String)QContactPhoneNumber::DefinitionName;
    m_fieldNamePairs["emails"] = (QLatin1String)QContactEmailAddress::DefinitionName;
    m_fieldNamePairs["addresses"] = (QLatin1String)QContactAddress::DefinitionName;
    m_fieldNamePairs["ims"] = (QLatin1String)QContactOnlineAccount::DefinitionName;
    m_fieldNamePairs["organizations"] = (QLatin1String)QContactOrganization::DefinitionName;
    m_fieldNamePairs["birthday"] = (QLatin1String)QContactBirthday::DefinitionName;
    m_fieldNamePairs["note"] = (QLatin1String)QContactNote::DefinitionName;
    m_fieldNamePairs["photos"] = (QLatin1String)QContactAvatar::DefinitionName;
    m_fieldNamePairs["urls"] = (QLatin1String)QContactUrl::DefinitionName;
#else
    m_fieldNamePairs["displayName"] = QContactDetail::TypeDisplayLabel;
    m_fieldNamePairs["name"] = QContactDetail::TypeName;
    m_fieldNamePairs["nickname"] = QContactDetail::TypeNickname;
    m_fieldNamePairs["phoneNumbers"] = QContactDetail::TypePhoneNumber;
    m_fieldNamePairs["emails"] = QContactDetail::TypeEmailAddress;
    m_fieldNamePairs["addresses"] = QContactDetail::TypeAddress;
    m_fieldNamePairs["ims"] = QContactDetail::TypeOnlineAccount;
    m_fieldNamePairs["organizations"] = QContactDetail::TypeOrganization;
    m_fieldNamePairs["birthday"] = QContactDetail::TypeBirthday;
    m_fieldNamePairs["note"] = QContactDetail::TypeNote;
    m_fieldNamePairs["photos"] = QContactDetail::TypeAvatar;
    m_fieldNamePairs["urls"] = QContactDetail::TypeUrl;
#endif

    m_notSupportedFields.clear();
    m_notSupportedFields << "categories";
    m_manager = new QContactManager();
}

void Contacts::saveContact(int scId, int ecId, const QVariantMap &params)
{
    qDebug() << "saveContact" ;
    QContact result;
    QList<QContactDetail *> detailsToDelete;
    foreach( QString key, params.keys()){
        qDebug() << key << "==" << params[key];
    }
    foreach (QString field, params.keys()) {
#if QT_VERSION < 0x050000
        QString qtDefinition = cordovaFieldNameToQtDefinition(field);
        if (qtDefinition.isEmpty())
            continue;
#else
        QContactDetail::DetailType qtDefinition = cordovaFieldNameToQtDefinition(field);
        if (qtDefinition == QContactDetail::TypeUndefined)
            continue;
#endif

        if (field == "nickname") {
            QContactNickname *detail = new QContactNickname;
            detail->setNickname(params[field].toString());
            detailsToDelete << detail;
            result.saveDetail(detail);
        } else if (field == "note") {
            QContactNote *detail = new QContactNote;
            detail->setNote(params[field].toString());
            detailsToDelete << detail;
            result.saveDetail(detail);
        } else if (field == "phoneNumbers") {
            if (params[field].type() != QVariant::List)
                continue;
            QVariantList phonesList = params[field].toList();
            foreach (const QVariant &phoneDesc, phonesList) {
                if (phoneDesc.type() != QVariant::Map)
                    continue;
                QContactPhoneNumber *detail = new QContactPhoneNumber;
                detail->setNumber(phoneDesc.toMap()["value"].toString());
                if (!phoneDesc.toMap()["type"].toString().isEmpty() &&
                        phoneDesc.toMap()["type"].toString() != "phone")
#if QT_VERSION < 0x050000
                    detail->setSubTypes(phoneDesc.toMap()["type"].toString());
#else
                    detail->setSubTypes(QList<int>() <<
                                        subTypePhoneFromString(phoneDesc.toMap()["type"].toString()));
#endif
                detailsToDelete << detail;
                result.saveDetail(detail);
            }
        } else if (field == "emails") {
            if (params[field].type() != QVariant::List)
                continue;
            QVariantList emailsList = params[field].toList();
            foreach (const QVariant &emailDesc, emailsList) {
                if (emailDesc.type() != QVariant::Map)
                    continue;
                QContactEmailAddress *detail = new QContactEmailAddress;
                detail->setEmailAddress(emailDesc.toMap()["value"].toString());
                detailsToDelete << detail;
                result.saveDetail(detail);
            }
        } else if (field == "ims") {
            if (params[field].type() != QVariant::List)
                continue;
            QVariantList imsList = params[field].toList();
            foreach (const QVariant &imDesc, imsList) {
                if (imDesc.type() != QVariant::Map)
                    continue;
                QContactOnlineAccount *detail = new QContactOnlineAccount;
                detail->setAccountUri(imDesc.toMap()["value"].toString());
                if (!imDesc.toMap()["type"].toString().isEmpty())
#if QT_VERSION < 0x050000
                    detail->setSubTypes(imDesc.toMap()["type"].toString());
#else
                    detail->setSubTypes(QList<int>() <<
                                        subTypeOnlineAccountFromString(imDesc.toMap()["type"].toString()));
#endif
                detailsToDelete << detail;
                result.saveDetail(detail);
            }
        } else if (field == "photos") {
            if (params[field].type() != QVariant::List)
                continue;
            QVariantList photosList = params[field].toList();
            foreach (const QVariant &photoDesc, photosList) {
                if (photoDesc.type() != QVariant::Map)
                    continue;
                //TODO: we need to decide should we support base64 images or not
                if (photoDesc.toMap()["type"].toString() != "url")
                    continue;
                QContactAvatar *detail = new QContactAvatar;
                detail->setImageUrl(QUrl(photoDesc.toMap()["value"].toString()));
                detailsToDelete << detail;
                result.saveDetail(detail);
            }
        } else if (field == "urls") {
            if (params[field].type() != QVariant::List)
                continue;
            QVariantList urlsList = params[field].toList();
            foreach (const QVariant &urlDesc, urlsList) {
                if (urlDesc.type() != QVariant::Map)
                    continue;
                QContactUrl *detail = new QContactUrl;
                detail->setUrl(urlDesc.toMap()["value"].toString());
                if (!urlDesc.toMap()["type"].toString().isEmpty())
#if QT_VERSION < 0x050000
                    detail->setSubType(urlDesc.toMap()["type"].toString());
#else
                    detail->setSubType((QContactUrl::SubType) subTypeUrlFromString(urlDesc.toMap()["type"].toString()));
#endif
                detailsToDelete << detail;
                result.saveDetail(detail);
            }
        } else if (field == "birthday") {
            QContactBirthday *detail = new QContactBirthday;
            detail->setDateTime(QDateTime::fromString(params[field].toString(), "yyyy-MM-ddThh:mm:ss.zzzZ"));
            detailsToDelete << detail;
            result.saveDetail(detail);
        } else if (field == "organizations") {

            if (params[field].type() != QVariant::List)
                continue;
            QVariantList organizationsList = params[field].toList();
            foreach (const QVariant &organizationDesc, organizationsList) {
                if (organizationDesc.type() != QVariant::Map)
                    continue;
                QContactOrganization *detail = new QContactOrganization;
                detail->setName(organizationDesc.toMap()["name"].toString());
                detail->setDepartment(QStringList() << organizationDesc.toMap()["department"].toString());
                detail->setRole(organizationDesc.toMap()["title"].toString());
                detailsToDelete << detail;
                result.saveDetail(detail);
            }

        } else if (field == "name") {
            QContactName *detail = new QContactName;
            QVariantMap nameMap = params[field].toMap();
            detail->setLastName(nameMap["familyName"].toString());
            detail->setFirstName(nameMap["givenName"].toString());
            detail->setMiddleName(nameMap["middleName"].toString());
            detail->setPrefix(nameMap["honorificPrefix"].toString());
            detail->setSuffix(nameMap["honorificSuffix"].toString());
            detailsToDelete << detail;
            result.saveDetail(detail);
        }

    }
#if QT_VERSION < 0x050000
    result = m_manager->compatibleContact(result);
#endif
    if (!m_manager->saveContact(&result)) {
        switch (m_manager->error()) {
        case QContactManager::DoesNotExistError:
        case QContactManager::AlreadyExistsError:
        case QContactManager::InvalidDetailError:
        case QContactManager::InvalidRelationshipError:
        case QContactManager::BadArgumentError:
        case QContactManager::InvalidContactTypeError:
            callback(ecId, "ContactError.INVALID_ARGUMENT_ERROR");
            break;
        case QContactManager::DetailAccessError:
        case QContactManager::PermissionsError:
            callback(ecId, "ContactError.PERMISSION_DENIED_ERROR");
            break;
        case QContactManager::NotSupportedError:
            callback(ecId, "ContactError.NOT_SUPPORTED_ERROR");
            break;
        case QContactManager::TimeoutError:
            callback(ecId, "ContactError.TIMEOUT_ERROR");
            break;
        case QContactManager::UnspecifiedError:
        case QContactManager::LockedError:
        case QContactManager::OutOfMemoryError:
        case QContactManager::VersionMismatchError:
        case QContactManager::LimitReachedError:
        case QContactManager::NoError:
        default:
            callback(ecId, "ContactError.UNKNOWN_ERROR");
            break;
        }
    } else {
        callback(scId, "");
    }
    qDeleteAll(detailsToDelete);
}

void Contacts::removeContact(int scId, int ecId, const QString &localId)
{
#if QT_VERSION < 0x050000
    quint32 id = localId.toUInt();
#else
    QContactId id = QContactId::fromString(localId);
#endif

    if (!m_manager->removeContact(id)) {
        switch (m_manager->error()) {
        case QContactManager::DoesNotExistError:
        case QContactManager::AlreadyExistsError:
        case QContactManager::InvalidDetailError:
        case QContactManager::InvalidRelationshipError:
        case QContactManager::BadArgumentError:
        case QContactManager::InvalidContactTypeError:
            callback(ecId, "ContactError.INVALID_ARGUMENT_ERROR");
            break;
        case QContactManager::DetailAccessError:
        case QContactManager::PermissionsError:
            callback(ecId, "ContactError.PERMISSION_DENIED_ERROR");
            break;
        case QContactManager::NotSupportedError:
            callback(ecId, "ContactError.NOT_SUPPORTED_ERROR");
            break;
        case QContactManager::TimeoutError:
            callback(ecId, "ContactError.TIMEOUT_ERROR");
            break;
        case QContactManager::UnspecifiedError:
        case QContactManager::LockedError:
        case QContactManager::OutOfMemoryError:
        case QContactManager::VersionMismatchError:
        case QContactManager::LimitReachedError:
        case QContactManager::NoError:
        default:
            callback(ecId, "ContactError.UNKNOWN_ERROR");
            break;
        }

    } else {
        callback(scId, "");
    }
}

void Contacts::findContacts(int scId, int ecId, const QStringList &fields, const QString &filter, bool multiple)
{
    qDebug() << Q_FUNC_INFO << filter << fields << multiple;
//    Q_UNUSED(ecId);
    if(fields.length() <= 0){
        callback(ecId, "ContactError.INVALID_ARGUMENT_ERROR");
    }

    QContactUnionFilter unionFilter;

#if QT_VERSION < 0x050000
    QMap<QString, QStringList> fieldNames;
    fieldNames[QContactDisplayLabel::DefinitionName] << QContactDisplayLabel::FieldLabel;
    fieldNames[QContactName::DefinitionName] << QContactName::FieldFirstName << QContactName::FieldLastName << QContactName::FieldMiddleName << QContactName::FieldPrefix << QContactName::FieldSuffix;
    fieldNames[QContactNickname::DefinitionName] << QContactNickname::FieldNickname;
    fieldNames[QContactPhoneNumber::DefinitionName] << QContactPhoneNumber::FieldNumber;
    fieldNames[QContactEmailAddress::DefinitionName] << QContactEmailAddress::FieldEmailAddress;
    fieldNames[QContactAddress::DefinitionName] << QContactAddress::FieldCountry << QContactAddress::FieldLocality << QContactAddress::FieldPostcode << QContactAddress::FieldPostOfficeBox << QContactAddress::FieldRegion << QContactAddress::FieldStreet;
    fieldNames[QContactOnlineAccount::DefinitionName] << QContactOnlineAccount::FieldAccountUri;
    fieldNames[QContactOrganization::DefinitionName] << QContactOrganization::FieldAssistantName << QContactOrganization::FieldDepartment << QContactOrganization::FieldLocation << QContactOrganization::FieldName << QContactOrganization::FieldRole << QContactOrganization::FieldTitle;
    fieldNames[QContactBirthday::DefinitionName] << QContactBirthday::FieldBirthday;
    fieldNames[QContactNote::DefinitionName] << QContactNote::FieldNote;
    fieldNames[QContactUrl::DefinitionName] << QContactUrl::FieldUrl;

    foreach (const QString &defName, fieldNames.keys()) {
        foreach(const QString &fieldName, fieldNames[defName]) {
            QContactDetailFilter subFilter;
            subFilter.setDetailDefinitionName(defName, fieldName);
            subFilter.setValue(filter);
            subFilter.setMatchFlags(QContactFilter::MatchContains);
            unionFilter.append(subFilter);
        }
    }
#else
    QMap<QContactDetail::DetailType, QList<int> > fieldNames;
    fieldNames[QContactDetail::TypeDisplayLabel] << QContactDisplayLabel::FieldLabel;
    fieldNames[QContactDetail::TypeName] << QContactName::FieldFirstName << QContactName::FieldLastName << QContactName::FieldMiddleName << QContactName::FieldPrefix << QContactName::FieldSuffix;
    fieldNames[QContactDetail::TypeNickname] << QContactNickname::FieldNickname;
    fieldNames[QContactDetail::TypePhoneNumber] << QContactPhoneNumber::FieldNumber;
    fieldNames[QContactDetail::TypeEmailAddress] << QContactEmailAddress::FieldEmailAddress;
    fieldNames[QContactDetail::TypeAddress] << QContactAddress::FieldCountry << QContactAddress::FieldLocality << QContactAddress::FieldPostcode << QContactAddress::FieldPostOfficeBox << QContactAddress::FieldRegion << QContactAddress::FieldStreet;
    fieldNames[QContactDetail::TypeOnlineAccount] << QContactOnlineAccount::FieldAccountUri;
    fieldNames[QContactDetail::TypeOrganization] << QContactOrganization::FieldAssistantName << QContactOrganization::FieldDepartment << QContactOrganization::FieldLocation << QContactOrganization::FieldName << QContactOrganization::FieldRole << QContactOrganization::FieldTitle;
    fieldNames[QContactDetail::TypeBirthday] << QContactBirthday::FieldBirthday;
    fieldNames[QContactDetail::TypeNote] << QContactNote::FieldNote;
    fieldNames[QContactDetail::TypeUrl] << QContactUrl::FieldUrl;

    foreach (const QContactDetail::DetailType &defName, fieldNames.keys()) {
        foreach(int fieldName, fieldNames[defName]) {
            QContactDetailFilter subFilter;
            subFilter.setDetailType(defName, fieldName);
            subFilter.setValue(filter);
            subFilter.setMatchFlags(QContactFilter::MatchContains);
            unionFilter.append(subFilter);
        }
    }
#endif


    QList<QContact> contacts = m_manager->contacts(unionFilter);
    if (contacts.empty()) {
        callback(scId, "[]");
    } else {
        QStringList stringifiedContacts;
        qDebug() << "stringifiedContacts:\n";
        foreach (const QContact &contact, contacts) {
            stringifiedContacts << jsonedContact(contact, fields);
            qDebug() << jsonedContact(contact, fields) << "\n";
            if (!multiple)
                break;
        }
        callback(scId, QString("[%1]").arg(stringifiedContacts.join(", ")));
    }
}

#if QT_VERSION < 0x050000
QString Contacts::cordovaFieldNameToQtDefinition(const QString &cordovaFieldName) const
#else
QContactDetail::DetailType Contacts::cordovaFieldNameToQtDefinition(const QString &cordovaFieldName) const
#endif
{
    if (m_fieldNamePairs.contains(cordovaFieldName))
        return m_fieldNamePairs[cordovaFieldName];
#if QT_VERSION < 0x050000
    return "";
#else
    return QContactDetail::TypeUndefined;
#endif
}

#if QT_VERSION >= 0x050000
int Contacts::subTypePhoneFromString(const QString &cordovaSubType) const
{
    QString preparedSubType = cordovaSubType.toLower();
    if (preparedSubType == "mobile")
        return QContactPhoneNumber::SubTypeMobile;
    else if (preparedSubType == "fax")
        return QContactPhoneNumber::SubTypeFax;
    else if (preparedSubType == "pager")
        return QContactPhoneNumber::SubTypePager;
    else if (preparedSubType == "voice")
        return QContactPhoneNumber::SubTypeVoice;
    else if (preparedSubType == "modem")
        return QContactPhoneNumber::SubTypeModem;
    else if (preparedSubType == "video")
        return QContactPhoneNumber::SubTypeVideo;
    else if (preparedSubType == "car")
        return QContactPhoneNumber::SubTypeCar;
    else if (preparedSubType == "assistant")
        return QContactPhoneNumber::SubTypeAssistant;
    return QContactPhoneNumber::SubTypeLandline;
}

int Contacts::subTypeOnlineAccountFromString(const QString &cordovaSubType) const
{
    QString preparedSubType = cordovaSubType.toLower();
    if (preparedSubType == "aim")
        return QContactOnlineAccount::ProtocolAim;
    else if (preparedSubType == "icq")
        return QContactOnlineAccount::ProtocolIcq;
    else if (preparedSubType == "irc")
        return QContactOnlineAccount::ProtocolIrc;
    else if (preparedSubType == "jabber")
        return QContactOnlineAccount::ProtocolJabber;
    else if (preparedSubType == "msn")
        return QContactOnlineAccount::ProtocolMsn;
    else if (preparedSubType == "qq")
        return QContactOnlineAccount::ProtocolQq;
    else if (preparedSubType == "skype")
        return QContactOnlineAccount::ProtocolSkype;
    else if (preparedSubType == "yahoo")
        return QContactOnlineAccount::ProtocolYahoo;
    return QContactOnlineAccount::ProtocolUnknown;
}

int Contacts::subTypeUrlFromString(const QString &cordovaSubType) const
{
    QString preparedSubType = cordovaSubType.toLower();
    if (preparedSubType == "blog")
        return QContactUrl::SubTypeBlog;
    else if (preparedSubType == "favourite")
        return QContactUrl::SubTypeFavourite;
    return QContactUrl::SubTypeHomePage;
}

QString Contacts::subTypePhoneToString(int qtSubType) const
{
    if (qtSubType == QContactPhoneNumber::SubTypeMobile)
        return "mobile";
    else if (qtSubType == QContactPhoneNumber::SubTypeFax)
        return "fax";
    else if (qtSubType == QContactPhoneNumber::SubTypePager)
        return "pager";
    else if (qtSubType == QContactPhoneNumber::SubTypeVoice)
        return "voice";
    else if (qtSubType == QContactPhoneNumber::SubTypeModem)
        return "modem";
    else if (qtSubType == QContactPhoneNumber::SubTypeVideo)
        return "video";
    else if (qtSubType == QContactPhoneNumber::SubTypeCar)
        return "car";
    else if (qtSubType == QContactPhoneNumber::SubTypeAssistant)
        return "assistant";
    return "home";
}

QString Contacts::subTypeOnlineAccountToString(int qtSubType) const
{
    if (qtSubType == QContactOnlineAccount::ProtocolAim)
        return "aim";
    else if (qtSubType == QContactOnlineAccount::ProtocolIcq)
        return "icq";
    else if (qtSubType == QContactOnlineAccount::ProtocolIrc)
        return "irc";
    else if (qtSubType == QContactOnlineAccount::ProtocolJabber)
        return "jabber";
    else if (qtSubType == QContactOnlineAccount::ProtocolMsn)
        return "msn";
    else if (qtSubType == QContactOnlineAccount::ProtocolQq)
        return "qq";
    else if (qtSubType == QContactOnlineAccount::ProtocolSkype)
        return "skype";
    else if (qtSubType == QContactOnlineAccount::ProtocolYahoo)
        return "yahoo";
    return "unknown";
}

QString Contacts::subTypeUrlToString(int qtSubType) const
{
    if (qtSubType == QContactUrl::SubTypeBlog)
        return "blog";
    else if (qtSubType == QContactUrl::SubTypeFavourite)
        return "favourite";
    return "homepage";
}
#endif


QString Contacts::jsonedContact(const QContact &contact, const QStringList &fields) const
{
    QStringList resultingFields = fields;
    if (resultingFields.empty())
        resultingFields.append(m_fieldNamePairs.keys());
    if (!resultingFields.contains("id"))
        resultingFields << "id";
    QStringList fieldValuesList;
    foreach (const QString &field, resultingFields) {
#if QT_VERSION < 0x050000
        QString qtDefinitionName = cordovaFieldNameToQtDefinition(field);
#else
        QContactDetail::DetailType qtDefinitionName = cordovaFieldNameToQtDefinition(field);
#endif
        if (field == "id") {
#if QT_VERSION < 0x050000
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(contact.localId());
#else
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(contact.id().toString());
#endif
        } else if (field == "displayName") {
            QContactDisplayLabel detail = contact.detail(qtDefinitionName);
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(detail.label());
        } else if (field == "nickname") {
            QContactNickname detail = contact.detail(qtDefinitionName);
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(detail.nickname());
        } else if (field == "note") {
            QContactNote detail = contact.detail(qtDefinitionName);
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(detail.note());
        } else if (field == "phoneNumbers") {
            QStringList fieldValues;
            QList<QContactDetail> details = contact.details(qtDefinitionName);
            foreach (const QContactDetail &detail, details) {
                QContactPhoneNumber castedDetail = detail;
#if QT_VERSION < 0x050000
                QStringList subTypes = castedDetail.subTypes();
#else
                QStringList subTypes;
                foreach (int subType, castedDetail.subTypes())
                    subTypes << subTypePhoneToString(subType);
#endif

                if (subTypes.isEmpty())
                    subTypes << "phone";
                foreach(const QString &subType, subTypes) {
                    fieldValues << QString("{type: \"%1\", value: \"%2\", pref: %3}")
                                   .arg(subType)
                                   .arg(castedDetail.number())
                                   .arg("false");
                }
            }
            fieldValuesList << QString("%1: [%2]")
                               .arg(field)
                               .arg(fieldValues.join(", "));
        } else if (field == "emails") {
            QStringList fieldValues;
            QList<QContactDetail> details = contact.details(qtDefinitionName);
            foreach (const QContactDetail &detail, details) {
                QContactEmailAddress castedDetail = detail;
                fieldValues << QString("{type: \"%1\", value: \"%2\", pref: %3}")
                               .arg("email")
                               .arg(castedDetail.emailAddress())
                               .arg("false");
            }
            fieldValuesList << QString("%1: [%2]")
                               .arg(field)
                               .arg(fieldValues.join(", "));
        } else if (field == "ims") {
            QStringList fieldValues;
            QList<QContactDetail> details = contact.details(qtDefinitionName);
            foreach (const QContactDetail &detail, details) {
                QContactOnlineAccount castedDetail = detail;
#if QT_VERSION < 0x050000
                QStringList subTypes = castedDetail.subTypes();
#else
                QStringList subTypes;
                foreach (int subType, castedDetail.subTypes())
                    subTypes << subTypeOnlineAccountToString(subType);
#endif
                if (subTypes.isEmpty())
                    subTypes << "IM";
                foreach(const QString &subType, subTypes) {
                    fieldValues << QString("{type: \"%1\", value: \"%2\", pref: %3}")
                                   .arg(subType)
                                   .arg(castedDetail.accountUri())
                                   .arg("false");
                }
            }
            fieldValuesList << QString("%1: [%2]")
                               .arg(field)
                               .arg(fieldValues.join(", "));
        } else if (field == "photos") {
            QStringList fieldValues;
            QList<QContactDetail> details = contact.details(qtDefinitionName);
            foreach (const QContactDetail &detail, details) {
                QContactAvatar castedDetail = detail;
                fieldValues << QString("{type: \"%1\", value: \"%2\", pref: %3}")
                               .arg("url")
                               .arg(castedDetail.imageUrl().toString())
                               .arg("false");
            }
            fieldValuesList << QString("%1: [%2]")
                               .arg(field)
                               .arg(fieldValues.join(", "));
        } else if (field == "urls") {
            QStringList fieldValues;
            QList<QContactDetail> details = contact.details(qtDefinitionName);

            foreach (const QContactDetail &detail, details) {
                QContactUrl castedDetail = detail;
#if QT_VERSION < 0x050000
                QString subType = castedDetail.subType();
#else
                QString subType = subTypeUrlToString(castedDetail.subType());
#endif
                fieldValues << QString("{type: \"%1\", value: \"%2\", pref: %3}")
                               .arg(subType)
                               .arg(castedDetail.url())
                               .arg("false");
            }
            fieldValuesList << QString("%1: [%2]")
                               .arg(field)
                               .arg(fieldValues.join(", "));
        } else if (field == "birthday") {
            QContactBirthday detail = contact.detail(qtDefinitionName);
            fieldValuesList << QString("%1: new Date(%2)")
                               .arg(field)
                               .arg(detail.dateTime().toTime_t()*1000);
        } else if (field == "organizations") {
            QStringList fieldValues;
            QList<QContactDetail> details = contact.details(qtDefinitionName);
            foreach (const QContactDetail &detail, details) {
                QContactOrganization castedDetail = detail;
                fieldValues << QString("{type: \"%1\", name: \"%2\", department: \"%3\", title: \"%4\", pref: %5}")
                               .arg("organization")
                               .arg(castedDetail.name())
                               .arg(castedDetail.department().join(" "))
                               .arg(castedDetail.role())
                               .arg("false");
            }
            fieldValuesList << QString("%1: [%2]")
                               .arg(field)
                               .arg(fieldValues.join(", "));
        } else if (field == "name") {
            QContactName detail = contact.detail(qtDefinitionName);
            fieldValuesList <<  QString("%1: {familyName: \"%2\", givenName: \"%3\", middleName: \"%4\", honorificPrefix: \"%5\", honorificSuffix: \"%6\"}")
                                .arg(field)
                                .arg(detail.lastName())
                                .arg(detail.firstName())
                                .arg(detail.middleName())
                                .arg(detail.prefix())
                                .arg(detail.suffix());
        }


    }
    QString result = QString("Contact.create({%1})").arg(fieldValuesList.join(", "));
    return result;
}
