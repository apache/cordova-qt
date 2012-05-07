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
                settings.offlineWebApplicationCacheEnabled : true
                settings.pluginsEnabled : true
                settings.localStorageDatabaseEnabled: true
                settings.offlineStorageDatabaseEnabled: true
                settings.localContentCanAccessRemoteUrls: true
                javaScriptWindowObjects: [QtObject{
                        WebView.windowObjectName: "qmlWrapper"

                        function callPluginFunction(pluginName, functionName, parameters) {
                            parameters = eval("("+parameters+")")
                            CordovaWrapper.execMethodOld(pluginName, functionName, parameters)
                        }
                        function callConfirm(message){
                            comfirmText.text = message;
                            myConfirm.open();
                        }
                    }]
                onUrlChanged: { console.log("url changing..")}



                onLoadFinished: {
                    cordova.loadFinished(true)
                }
                onLoadFailed: cordova.loadFinished(false)
                onAlert: {
                    alertText.text = message
                    myalert.open()
                }

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

                Dialog {
                    id: myalert
                    content:Item {
                        height: 50
                        width: parent.width
                        Text {
                            id: alertText
                            font.pixelSize: 22
                            anchors.centerIn: parent
                            color: "white"
                            text: "Hello"
                        }
                    }

                    buttons: ButtonRow {
                        style: ButtonStyle { }
                        anchors.horizontalCenter: parent.horizontalCenter
                        Button {text: "OK"; onClicked: myalert.accept()}
                    }
                }

                Dialog {
                    id: myConfirm
                    property string js: "Notification.Callback"
                    content:Item {
                        height: 50
                        width: parent.width
                        Text {
                            id: comfirmText
                            font.pixelSize: 22
                            anchors.centerIn: parent
                            color: "white"
                            text: "confirm"
                        }
                    }

                    buttons: ButtonRow {
                        style: ButtonStyle { }
                        anchors.horizontalCenter: parent.horizontalCenter
                        Button {text: "OK"; onClicked:myConfirm.accept()}
                        Button {text: "Cancel"; onClicked:myConfirm.reject()}
                    }
                    onAccepted: cordova.execJS(js + "(1)")
                    onRejected: cordova.execJS(js + "(2)")
                }
            }

        }
    }

}
