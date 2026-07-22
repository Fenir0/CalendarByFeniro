import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    id: dialogShare_Window
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
                    id: dialogShare_TextEdit_Username
                    leftPadding: 5
                    SequentialAnimation {
                        id: dialogShare_SeqAnim_Username
                        loops: 4
                        
                        ColorAnimation { 
                            target: dialogShare_TextEdit_Username
                            property: "color"
                            to: "red"
                            duration: 150 
                        }
                        ColorAnimation { 
                            target: dialogShare_TextEdit_Username
                            property: "color"
                            to: "black"
                            duration: 150
                        }
                    }
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
                id: dialogShare_ComboBox_AccessLevel
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
                    RequestHandler.share(documentId, dialogShare_TextEdit_Username.text, dialogShare_ComboBox_AccessLevel.currentText, function(success, msg){

                        if(success){
                            console.log("Shared successfully")
                            dialogShare_Window.close()
                        }else {
                            if(msg == "Nouser"){
                                dialogShare_SeqAnim_Username.start()
                            }
                        }
                    })
                }
                
            }
            Button{
                Layout.alignment: Qt.AlignRight
                text: "Cancel"
                onClicked:{
                    dialogShare_Window.close()
                }
            }
        }
    }
}