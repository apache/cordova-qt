import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0
import "cordova_wrapper.js" as CordovaWrapper



PageStackWindow {
    id: appWindow
    initialPage: mainPage
    showToolBar: false

    Page {
        id: mainPage

        Flickable {
            id: webFlickable

            anchors.fill: parent

            contentHeight: webView.height
            contentWidth: webView.width

            boundsBehavior: "StopAtBounds"
            clip: true

            WebView {

                id: webView
                preferredWidth: mainPage.width
                preferredHeight: mainPage.height

                url: cordova.mainUrl
                settings.javascriptEnabled: true
                settings.localStorageDatabaseEnabled: true
                settings.offlineStorageDatabaseEnabled: true
                settings.localContentCanAccessRemoteUrls: true
                javaScriptWindowObjects: [QtObject{
                    WebView.windowObjectName: "qmlWrapper"

                    function callPluginFunction(pluginName, functionName, parameters) {
                        parameters = eval("("+parameters+")")
                        CordovaWrapper.execMethodOld(pluginName, functionName, parameters)
                    }
                }]

                onLoadFinished: cordova.loadFinished(true)
                onLoadFailed: cordova.loadFinished(false)

                Connections {
                    target: cordova
                    onJavaScriptExecNeeded: {
                        console.log("onJavaScriptExecNeeded: " + js)
                        webView.evaluateJavaScript(js)
                    }

                    onPluginWantsToBeAdded: {
                        console.log("onPluginWantsToBeAdded: " + pluginName)
                        CordovaWrapper.addPlugin(pluginName, pluginObject)
                    }
                }
            }
        }

    }

}
