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

#include "cordova.h"
#include "pluginregistry.h"

#include <qplatformdefs.h>

#include <QDebug>
#include <QXmlStreamReader>
#include <QApplication>
#include <QUrl>

Cordova *Cordova::m_instance = 0;

Cordova::Cordova(QObject *parent) : QObject(parent) {
    m_topLevelEventsReceiver = 0;
    // Determine index file path
    m_workingDir = QApplication::applicationDirPath();
#ifdef MEEGO_EDITION_HARMATTAN
    m_workingDir.cdUp();
#endif
    qDebug() << "Using" << m_workingDir.absolutePath() << "as working dir";
    QDir wwwDir( m_workingDir );
    wwwDir.cd( "www" );

#if QT_VERSION < 0x050000
    m_mainUrl = QUrl::fromUserInput(wwwDir.absoluteFilePath("index.html")).toString();
#else
    m_mainUrl = QUrl::fromUserInput(wwwDir.absoluteFilePath("index_qt5.html")).toString();
#endif
}

Cordova *Cordova::instance()
{
    if (!m_instance)
        m_instance = new Cordova;
    return m_instance;
}

/**
 * Called when the webview finished loading a new page
 */
void Cordova::loadFinished( bool ok ) {
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

                    qDebug() << "Adding Plugin " << attribName << " with " << attribValue;
                    // Check for such a plugin
                    CPlugin *currPlugin = PluginRegistry::getRegistry()->getPlugin( attribValue );
                    if(currPlugin) {
                        currPlugin->init();
                        emit pluginWantsToBeAdded(attribValue, currPlugin, attribName);
                        execJS( "Cordova.enablePlugin( '" + attribValue + "' )" );
                    }
                    else {
                        qDebug() << "Unknown Plugin " << attribName;
                    }
                }
            }
        }
    }

    // Device is now ready to rumble
    execJS( "Cordova.deviceready();" );
}


QString Cordova::workingDir() const
{
    return m_workingDir.absolutePath();
}

void Cordova::execJS(const QString &js)
{
    emit javaScriptExecNeeded(js);
}

QString Cordova::mainUrl() const
{
    return m_mainUrl;
}

void Cordova::setTopLevelEventsReceiver(QObject *obj)
{
    m_topLevelEventsReceiver = obj;
}

QObject *Cordova::topLevelEventsReceiver()
{
    return m_topLevelEventsReceiver;
}
