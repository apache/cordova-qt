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

#include "phonegap.h"
#include "pluginregistry.h"
#include "pgwebpage.h"

#include <QDebug>
#include <QXmlStreamReader>

PhoneGap::PhoneGap(QWebView *webView) : QObject(webView) {
    m_webView = webView;
    // Configure web view
    m_webView->settings()->enablePersistentStorage();
    m_webView->settings()->setAttribute( QWebSettings::LocalStorageEnabled, true );
    m_webView->settings()->setAttribute( QWebSettings::OfflineStorageDatabaseEnabled, true );
    m_webView->settings()->setAttribute( QWebSettings::LocalContentCanAccessRemoteUrls, true );

    // Listen to load finished signal
    QObject::connect( m_webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)) );

    // Set our own WebPage class
    m_webView->setPage( new PGWebPage() );

    // Determine index file path
    m_workingDir = QDir::current();
    QDir wwwDir( m_workingDir );
    wwwDir.cd( "www" );

    // Load the correct startup file
    m_webView->load( QUrl::fromUserInput(wwwDir.absoluteFilePath("index.html")) );
    //m_webView->load( QUrl::fromUserInput("http://html5test.com/") );
}

/**
 * Called when the webview finished loading a new page
 */
void PhoneGap::loadFinished( bool ok ) {
    Q_UNUSED(ok)

    // Change into the xml-directory
    QDir xmlDir( m_workingDir );
    xmlDir.cd( "xml" );

    // Try to open the plugins configuration
    QFile pluginsXml( xmlDir.filePath("plugins.xml") );
    if( !pluginsXml.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        qDebug() << "Error loading plugins config!";
        return;
    }

    // Start reading the file as a stream
    QXmlStreamReader plugins;
    plugins.setDevice( &pluginsXml );

    // Get a reference to the current main-frame
    QWebFrame *webFrame = m_webView->page()->mainFrame();

    // Iterate over plugins-configuration and load all according plugins
    while(!plugins.atEnd()) {
        if( plugins.readNext() == QXmlStreamReader::StartElement ) {
            // Check if we have a plugin element
            if( plugins.name() == "plugin" ) {
                QXmlStreamAttributes attribs = plugins.attributes();
                // Check for name & value attributes
                if( attribs.hasAttribute("name") && attribs.hasAttribute("value") ) {
                    // Construct object & attribute names
                    QString attribName = attribs.value( "name" ).toString();
                    QString attribValue = attribs.value( "value" ).toString();
                    QString objectName = attribName + "_native";

                    qDebug() << "Adding Plugin " << attribName << " with " << attribValue;
                    // Check for such a plugin
                    PGPlugin *currPlugin = PluginRegistry::getRegistry()->getPlugin( attribValue );
                    if( currPlugin != NULL ) {
                        currPlugin->setWebFrame( webFrame );
                        webFrame->addToJavaScriptWindowObject( objectName, currPlugin );

                        webFrame->evaluateJavaScript( "PhoneGap.Qt.registerObject( '" + attribValue + "', " + objectName + " )" );
                        webFrame->evaluateJavaScript( "PhoneGap.enablePlugin( '" + attribValue + "' )" );
                    }
                    else {
                        qDebug() << "Unknown Plugin " << attribName;
                    }
                }
            }
        }
    }

    // Device is now ready to rumble
    webFrame->evaluateJavaScript( "PhoneGap.deviceready();" );
}
