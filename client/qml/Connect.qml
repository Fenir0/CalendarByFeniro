import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar

Window{
    id: serverIpEnter_Window
    height: 180
    width: 200
    property string backColor: '#e4fbfd'
    property string editColor: '#a1aaaa'
    property string buttonColor: '#6deeee'
    property string buttonUnavailableColor: '#6273e0'

    color: backColor
    Column{
        spacing: 30
        width: parent.width - 5
        height: parent.height - 15
        anchors.centerIn: parent

        Text{
            height: 20
            width: 150
            text: "Enter server IP"
            horizontalAlignment: Text.AlignHCenter 
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Rectangle{
            height: 40
            width: parent.width * 4/5
            anchors.horizontalCenter: parent.horizontalCenter
            color: editColor
            TextField {
                id: serverIpEnter_TextField_Ip
                width: parent.width
                placeholderText: "e.g. 192.168.1.1"
                focus: true
                inputMethodHints: Qt.ImhFormattedNumbersOnly

                validator: RegularExpressionValidator {
                    regularExpression: /^(?:(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){0,3}(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])?\.?$/
                }

                color: acceptableInput ? "black" : "red"
                SequentialAnimation {
                    id: serverIpEnter_SeqAnim_IpBlink
                    loops: 4
                    
                    ColorAnimation { 
                        target: serverIpEnter_TextField_Ip
                        property: "color"
                        to: "red"
                        duration: 150 
                    }
                    ColorAnimation { 
                        target: serverIpEnter_TextField_Ip
                        property: "color"
                        to: "black"
                        duration: 150
                    }
                }
                Keys.onReturnPressed: serverIpEnter_Button_Confirm.clicked()
                Keys.onEscapePressed: serverIpEnter_Window.close()
            }
        }
        Button{
            id: serverIpEnter_Button_Confirm
            text: "confirm"
            width: parent.width * 3/5
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle{
                color: buttonColor
                radius: 10
                border.color: "black"
            }
            onClicked:{ 
               WebSocket.connectToServer(serverIpEnter_TextField_Ip.text)
               serverIpEnter_SeqAnim_IpBlink.start()
            }
        }
    }
Connections {
        target: WebSocket

        function onConnectionSucceeded() {
            serverIpEnter_Window.close();
            mainWindow_RoundButton_Network.clicked()
            console.log("Connected successfully!")
        }
    }
}