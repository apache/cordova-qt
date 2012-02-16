import QtQuick 2.0
import QtWebKit 3.0
import QtWebKit.experimental 3.0
import "cordova_wrapper.js" as CordovaWrapper

WebView {
    id: webView
    width: 854
    height: 480


    experimental.preferences.navigatorQtObjectEnabled: true
    experimental.onMessageReceived: {
        console.log("WebView received Message: " + message.data)
        CordovaWrapper.messageHandler(message)
    }
    //Uncomment when it will be available
    //experimental.setFlickableViewportEnabled: false
    experimental.useTraditionalDesktopBehaviour: true


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
