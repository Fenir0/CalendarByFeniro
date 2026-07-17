import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar

Rectangle{
    property string documentName: ""
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
                icon.source: "img/documentLoadButton.png"
                icon.color: "transparent"
                onClicked:{

                }
            }
            RoundButton{
                id: shareDocumentButton
                
                background: Rectangle{
                    radius: 10
                    color: "transparent"
                    border.color: "black"
                }
                icon.source: "img/documentShareButton.png"
                icon.color: "transparent"
                onClicked:{

                }
            }
            RoundButton{
                id: deleteDocumentButton
                
                background: Rectangle{
                    radius: 10
                    color: "transparent"
                    border.color: "black"
                }
                icon.source: "img/documentTrashButton.png"
                icon.color: "transparent"
                onClicked:{

                }
            }

        }
    }
}