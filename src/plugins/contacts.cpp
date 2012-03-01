#include "contacts.h"
#include "../pluginregistry.h"

#include <QMap>
#include <QContactGuid>
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
    m_fieldNamePairs["id"] = (QLatin1String)QContactGuid::DefinitionName;
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
    Q_UNUSED(scId);
    Q_UNUSED(ecId);

    QContact *result = new QContact();
    QList<QContactDetail *> detailsToDelete;
    foreach (const QString& paramName, params.keys()) {
        QString mobilityDetailName = cordovaFieldNameToQtDefinitionName(paramName);
        if (mobilityDetailName.isEmpty())
            continue;
        QContactDetail *detail = new QContactDetail(mobilityDetailName);
//        detail->setValue(params[paramName]);
        detailsToDelete << detail;
        result->saveDetail(detail);
    }
    m_manager->saveContact(result);
    delete result;
    qDeleteAll(detailsToDelete);
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
    QStringList fieldValuesList;
    foreach (const QString &field, resultingFields) {
        QString qtDefinitionName = cordovaFieldNameToQtDefinitionName(field);
        if (field == "id") {
            QContactGuid detail = contact.detail(qtDefinitionName);
            fieldValuesList << QString("%1: \"%2\"")
                               .arg(field)
                               .arg(detail.guid());
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
