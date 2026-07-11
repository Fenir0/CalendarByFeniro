import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    color: "#9db8d1"
    id: loadMenuWindow
    width: 350  
    height: 350

    Column{
        width: parent.width
        height: parent.height
        Column{
            width: 200
            anchors.horizontalCenter: parent.horizontalCenter
            height: 100
            Text{
                height: 30
                text: "File name"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Rectangle{
                width: 200
                height: 30
                color:"gray"
                TextEdit{
                    id: fileNameEdit
                    selectByMouse: true
                    focus: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    clip:true
                    width: parent.width
                    height: parent.height
                    
                }
            }
        }
        
        Column{
            width: 200
            anchors.horizontalCenter: parent.horizontalCenter
            height: 100
            Text{
                height: 30
                text: "Connect as"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Rectangle{
                width: 200
                height: 30
                color:"gray"
                TextEdit{
                    id: usernameEdit
                    selectByMouse: true
                    focus: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    clip:true
                    width: parent.width
                    height: parent.height
                    
                }
            }
        }
        
        Column{
            width: 200
            anchors.horizontalCenter: parent.horizontalCenter
            height: 100
            Text{
                height: 30
                text: "Password"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Rectangle{
                width: 200
                height: 30
                color:"gray"
                TextEdit{
                    id: passwordEdit
                    selectByMouse: true
                    focus: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    clip:true
                    width: parent.width
                    height: parent.height
                    
                }
            }
        }
        Button{
            text: "Connect"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked:{
                if (WebSocket.isConnected) {
                    WebSocket.disconnectFromServer();
                } else {
                    WebSocket.connectToServer();
                }
            }
        }
    }
}