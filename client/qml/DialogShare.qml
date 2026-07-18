import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    id: shareWindow
    height: 260
    width: 230
    property string documentName: ""
    property int documentId: 0

    color: '#e4fbfd'
    property string editColor: '#a1aaaa'
    property string buttonColor: '#6deeee'
    property string buttonUnavailableColor: '#6273e0'

    Column{
        spacing: 10
        anchors.margins: 10
        anchors.fill: parent
        Text{
            text: "Share a document:<br><b>%1</b>".arg(documentName)
            textFormat: Text.StyledText
        }
        RowLayout{
            height: parent.height / 5
            width: parent.width
            Text{ 
                Layout.alignment: Qt.AlignLeft
                text: "User name: "
            }
            Rectangle{
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.width * 1/2
                color: editColor
                Layout.alignment: Qt.AlignRight
                TextEdit{
                    verticalAlignment: Qt.AlignVCenter
                    anchors.fill: parent
                    id: usernameShareEdit
                }
            }
        }
        RowLayout{
            height: parent.height / 5
            width: parent.width
            Text{ 
                Layout.alignment: Qt.AlignLeft
                text: "Access: "
            }
            ComboBox{
                Layout.alignment: Qt.AlignRight
                id: accessCombo
                model: ["Editor", "Reader"]
            }
        }
        RowLayout{
            height: parent.height / 4
            width: parent.width
            Button{ 
                Layout.alignment: Qt.AlignLeft
                text: "Share"
                onClicked:{
                    RequestHandler.share(documentId, usernameShareEdit.text, accessCombo.currentText, function(success, documentQString){
                        if(success){
                            console.log("Shared successfully")
                        }  else{
                            console.log("Failed to fetch")
                        }         
                    })
                    shareWindow.close()
                }
                
            }
            Button{
                Layout.alignment: Qt.AlignRight
                text: "Cancel"
                onClicked:{
                    shareWindow.close()
                }
            }
        }
    }
}