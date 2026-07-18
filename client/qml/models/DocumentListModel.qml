import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar

Rectangle{
    property string documentName: ""
    property int documentId: 0
    color: '#dfdf7a'
    width: ListView.view ? ListView.view.width : 300 
    height: 100
    border.color: "black"

    RowLayout{
        anchors.fill: parent
        anchors.margins: 5
        Text{
            text: documentName
            leftPadding: 30
            verticalAlignment: Text.AlignVCenter
        }
        Column{
            Layout.alignment: Qt.AlignRight
            RoundButton{
                id: downloadDocumentButton
                background: Rectangle{
                    radius: 10
                    color: "transparent"
                    border.color: "black"
                }
                icon.source: "../../img/documentLoadButton.png"
                icon.color: "transparent"
                onClicked:{
                    RequestHandler.loadFromServer(documentId, documentName, function(success, response){
                        if(success){
                            console.log("File loaded")
                        }
                        else {
                            console.log("Oops")
                        }
                    })
                    loadMenuWindow.close();
                }
            }
            RoundButton{
                id: shareDocumentButton
                
                background: Rectangle{
                    radius: 10
                    color: "transparent"
                    border.color: "black"
                }
                icon.source: "../../img/documentShareButton.png"
                icon.color: "transparent"
                onClicked:{
                    var component = Qt.createComponent("../DialogShare.qml")

                    if (component.status === Component.Ready) {
                        var window = component.createObject(null, {
                            "documentName": documentName,
                            "documentId": documentId
                        })
                        if (window) {
                            window.show()
                        } else {
                            console.error("Error: createObject returned null even though component was Ready.")
                        }
                        
                    } else if (component.status === Component.Error) {
                        console.error("Error loading DialogShare.qml:", component.errorString())
                    }
                }
            }
            RoundButton{
                id: deleteDocumentButton

                background: Rectangle{
                    radius: 10
                    color: "transparent"
                    border.color: "black"
                }
                icon.source: "../../img/documentTrashButton.png"
                icon.color: "transparent"
                onClicked:{
                    deletionDialog.open()
                }
            }

        }
    }
    Dialog{
        id: deletionDialog
        title: "Confirm"
        modal: true

        property string documentNameDialog: ""
        property int documentIdDialog: 0

        standardButtons: Dialog.Ok | Dialog.Cancel

        Label{
            text: "Are you sure?\nThis action can't be undone"
            anchors.centerIn: parent
        }

        onAccepted: {
            console.log("Deletion confirmed")
        }
        onRejected: {
            console.log("Deletion confirmed")
        }
    }
    
}