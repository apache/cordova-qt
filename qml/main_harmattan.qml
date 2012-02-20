import QtQuick 1.1
import com.nokia.meego 1.1
import QtWebKit 1.0
import "cordova_wrapper.js" as CordovaWrapper



PageStackWindow {
    id: appWindow
    initialPage: mainPage
    showToolBar: false

    Page {
        id: mainPage

        WebView {

            id: webView
            anchors.fill: parent

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
