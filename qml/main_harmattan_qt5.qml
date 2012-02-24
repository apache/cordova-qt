import QtQuick 2.0
import com.nokia.meego 2.0
import QtWebKit 3.0
import QtWebKit.experimental 1.0
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
                width: appWindow.width
                height: appWindow.height


                experimental.preferences.navigatorQtObjectEnabled: true
                experimental.onMessageReceived: {
                    console.log("WebView received Message: " + message.data)
                    CordovaWrapper.messageHandler(message)
                }
                //Uncomment when it will be available
                //experimental.setFlickableViewportEnabled: false


                Component.onCompleted: {
                    webView.load(cordova.mainUrl)
                }

                onLoadSucceeded: cordova.loadFinished(true)
                onLoadFailed: cordova.loadFinished(false)

                Connections {
                    target: cordova
                    onJavaScriptExecNeeded: {
                        webView.experimental.postMessage(JSON.stringify({messageType: "evalJS", jsData: js}))
                    }

                    onPluginWantsToBeAdded: {
                        CordovaWrapper.addPlugin(pluginName, pluginObject)
                    }
                }
            }
        }

    }

}
