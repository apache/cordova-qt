function ContactAddress() {
}

ContactAddress.create = function(obj) {
            var result = new ContactAddress()
            result.pref = obj.pref
            result.type = obj.type
            result.formatted = obj.formatted
            result.streetAddress = obj.streetAddress
            result.locality = obj.streetLocality
            result.region = obj.region
            result.postalCode = obj.postalCode
            result.country = obj.country
            return result
        }

ContactAddress.prototype.pref = false
ContactAddress.prototype.type = ""
ContactAddress.prototype.formatted = ""
ContactAddress.prototype.streetAddress = ""
ContactAddress.prototype.locality = ""
ContactAddress.prototype.region = ""
ContactAddress.prototype.postalCode = ""
ContactAddress.prototype.country = ""


function ContactField() {
}

ContactField.create = function(obj) {
            var result = new ContactField()
            result.type = obj.type
            result.value = obj.value
            result.pref = obj.pref
            return result
        }

ContactField.prototype.type = ""
ContactField.prototype.value = ""
ContactField.prototype.pref = false


function ContactFindOptions() {
}

ContactFindOptions.create = function(obj) {
            var result = new ContactFindOptions()
            result.filter = obj.filter
            result.multiple = obj.multiple
            return result
        }

ContactFindOptions.prototype.filter = ""
ContactFindOptions.prototype.multiple = false


function ContactName() {
}

ContactName.create = function(obj) {
            var result = new ContactName()
            result.familyName = obj.familyName
            result.givenName = obj.givenName
            result.middleName = obj.middleName
            result.honorificPrefix = obj.honorificPrefix
            result.honorificSuffix = obj.honorificSuffix
            var formattedArr = []
            if (typeof result.honorificPrefix == 'undefined')
                result.honorificPrefix = ""
            else if (result.honorificPrefix != "")
                formattedArr.push(result.honorificPrefix)
            if (typeof result.givenName == 'undefined')
                result.givenName = ""
            else if (result.givenName != "")
                formattedArr.push(result.givenName)
            if (typeof result.middleName == 'undefined')
                result.middleName = ""
            else if (result.middleName != "")
                formattedArr.push(result.middleName)
            if (typeof result.familyName == 'undefined')
                result.familyName = ""
            else if (result.familyName != "")
                formattedArr.push(result.familyName)
            if (typeof result.honorificSuffix == 'undefined')
                result.honorificSuffix = ""
            else if (result.honorificSuffix != "")
                formattedArr.push(result.honorificSuffix)

            result.formatted = formattedArr.join(" ")

            return result
        }

ContactName.prototype.formatted = ""
ContactName.prototype.familyName = ""
ContactName.prototype.givenName = ""
ContactName.prototype.middleName = ""
ContactName.prototype.honorificPrefix = ""
ContactName.prototype.honorificSuffix = ""


function ContactOrganization() {
}

ContactOrganization.create = function(obj) {
            var result = new ContactOrganization()
            result.pref = obj.pref
            result.type = obj.type
            result.name = obj.name
            result.department = obj.department
            result.title = obj.title
            return result
        }

ContactOrganization.prototype.pref = false
ContactOrganization.prototype.type = ""
ContactOrganization.prototype.name = ""
ContactOrganization.prototype.department = ""
ContactOrganization.prototype.title = ""


function ContactError() {
}

ContactError.UNKNOWN_ERROR = 0
ContactError.INVALID_ARGUMENT_ERROR = 1
ContactError.TIMEOUT_ERROR = 2
ContactError.PENDING_OPERATION_ERROR = 3
ContactError.IO_ERROR = 4
ContactError.NOT_SUPPORTED_ERROR = 5
ContactError.PERMISSION_DENIED_ERROR = 6

ContactError.prototype.code = ContactError.UNKNOWN_ERROR


function Contact() {
    this.name = new ContactName()
    this.phoneNumbers = []
    this.emails = []
    this.addresses = []
    this.ims = []
    this.organizations = []
    this.photos = []
    this.categories = []
    this.urls = []
}

Contact.create = function(obj) {
            var result = new Contact()
            result.id = obj.id
            result.displayName = obj.displayName
            result.name = ContactName.create(obj.name)
            result.nickname = obj.nickname
            var subObj
            for (subObj in obj.phoneNumbers)
                result.phoneNumbers.push(ContactField.create(obj.phoneNumbers[subObj]))
            for (subObj in obj.emails)
                result.emails.push(ContactField.create(obj.emails[subObj]))
            for (subObj in obj.addresses)
                result.addresses.push(ContactAddress.create(obj.addresses[subObj]))
            for (subObj in obj.ims)
                result.ims.push(ContactField.create(obj.ims[subObj]))
            for (subObj in obj.organizations)
                result.organizations.push(ContactOrganization.create(obj.organizations[subObj]))
            result.birthday = obj.birthday
            result.note = obj.note
            for (subObj in obj.photos)
                result.photos.push(ContactField.create(obj.photos[subObj]))
            for (subObj in obj.categories)
                result.categories.push(ContactField.create(obj.categories[subObj]))
            for (subObj in obj.urls)
                result.urls.push(ContactField.create(obj.urls[subObj]))
            return result
        }

Contact.prototype.id = null
Contact.prototype.displayName = ""
Contact.prototype.name = new ContactName()
Contact.prototype.nickname = ""
Contact.prototype.phoneNumbers = []
Contact.prototype.emails = []
Contact.prototype.addresses = []
Contact.prototype.ims = []
Contact.prototype.organizations = []
Contact.prototype.birthday = new Date()
Contact.prototype.note = ""
Contact.prototype.photos = []
Contact.prototype.categories = []
Contact.prototype.urls = []

Contact.prototype.clone = function() {
            var newContact = Contact.create(this)
            newContact.id = null
            return newContact
        }

Contact.prototype.remove = function(contactSuccess,contactError) {
            console.log("Contact.remove 1")
            if( typeof contactSuccess !== "function" ) contactSuccess = function() {}
            if( typeof contactError !== "function" ) contactError = function() {}

            //TODO: call onSaveError here
            if (this.id == null || this.id == "")
                return
            console.log("Contact.remove 2")

            Cordova.exec( contactSuccess, contactError, "com.cordova.Contacts", "removeContact", [ this.id ] )
            console.log("Contact.remove 3")
}

Contact.prototype.save = function(contactSuccess,contactError) {
            console.log("Contact.save 1")
            if( typeof contactSuccess !== "function" ) contactSuccess = function() {}
            if( typeof contactError !== "function" ) contactError = function() {}

            console.log("Contact.save 2")

            Cordova.exec( contactSuccess, contactError, "com.cordova.Contacts", "saveContact", [ this ] )
            console.log("Contact.save 3")

        }



function ContactsManager() {

}

ContactsManager.prototype.create = function(properties) {
            return Contact.create(properties)
        }

ContactsManager.prototype.find = function(contactFields, contactSuccess, contactError, contactFindOptions) {
            // Check the callbacks
            if( typeof contactSuccess !== "function" ) return
            if( typeof contactError !== "function" ) contactError = function() {}

            Cordova.exec( contactSuccess, contactError, "com.cordova.Contacts", "findContacts", [ contactFields, contactFindOptions.filter, contactFindOptions.multiple ] )
        }

/**
 * Add the ContactsManager object to the navigator
 */
Cordova.addConstructor( "com.cordova.Contacts", function () {
                            navigator.contacts = new ContactsManager()
                        } );
