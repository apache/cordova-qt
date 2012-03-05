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

    m_notSupportedFields.clear();
    m_notSupportedFields << "categories";
    m_manager = new QContactManager();
}

void Contacts::saveContact(int scId, int ecId, const QVariantMap &params)
{
    QContact result;
    QList<QContactDetail *> detailsToDelete;
    foreach (const QString& field, params.keys()) {
        QString qtDefinitionName = cordovaFieldNameToQtDefinitionName(field);
        if (qtDefinitionName.isEmpty())
            continue;

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
                if (!phoneDesc.toMap()["type"].toString().isEmpty() && phoneDesc.toMap()["type"].toString() != "phone")
                    detail->setSubTypes(phoneDesc.toMap()["type"].toString());
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
                    detail->setSubTypes(imDesc.toMap()["type"].toString());
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
                    detail->setSubType(urlDesc.toMap()["type"].toString());
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
    result = m_manager->compatibleContact(result);
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
    if (!m_manager->removeContact(localId.toUInt())) {
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
    Q_UNUSED(ecId);

    QContactUnionFilter unionFilter;

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

    QList<QContact> contacts = m_manager->contacts(unionFilter);
    if (contacts.empty()) {
        callback(scId, "[]");
    } else {
        QStringList stringifiedContacts;
        foreach (const QContact &contact, contacts) {
            stringifiedContacts << jsonedContact(contact, fields);
            if (!multiple)
                break;
        }
        callback(scId, QString("[%1]").arg(stringifiedContacts.join(", ")));
    }
}

QString Contacts::cordovaFieldNameToQtDefinitionName(const QString &cordovaFieldName) const
{
    if (m_fieldNamePairs.contains(cordovaFieldName))
        return m_fieldNamePairs[cordovaFieldName];
    return "";
}

QString Contacts::jsonedContact(const QContact &contact, const QStringList &fields) const
{
    QStringList resultingFields = fields;
    if (resultingFields.empty())
        resultingFields.append(m_fieldNamePairs.keys());
    if (!resultingFields.contains("id"))
        resultingFields << "id";
    QStringList fieldValuesList;
    foreach (const QString &field, resultingFields) {
        QString qtDefinitionName = cordovaFieldNameToQtDefinitionName(field);
        if (field == "id") {
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(contact.localId());
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
                QStringList subTypes = castedDetail.subTypes();
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
                QStringList subTypes = castedDetail.subTypes();
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
                fieldValues << QString("{type: \"%1\", value: \"%2\", pref: %3}")
                               .arg(castedDetail.subType())
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
