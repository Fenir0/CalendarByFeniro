import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    color: "#9db8d1"
    id: loadMenuWindow
    width: 400  
    height: 400

    RowLayout{
        id: topButtonsLayout
        width: parent.width
        height:parent.height/7
        anchors.top: parent.top
        Button{
            Layout.preferredWidth: parent.width/4
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignLeft
            Text{
                text: "User"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                userSettingsColumn.visible = true
                documentSettingsColumn.visible = false
            }
        }
        Button{
            Layout.preferredWidth:parent.width/3
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignCenter
            Text{
                text: "This\ndocument"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
            if(AppState.loggedIn) {
                userSettingsColumn.visible = false
                documentSettingsColumn.visible = true
                }
            }
        }
        Button{
            Layout.preferredWidth:parent.width/3
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignRight
            Text{
                text: "Your\ndocuments"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter 
            }
            onClicked: {if(AppState.loggedIn) documentsOverviewColumn.visible = !documentsOverviewColumn.visible}}
    }
    Rectangle{
        id:spacer
        anchors.top: topButtonsLayout.bottom
        color: "transparent"
        width: parent.width
        height: parent.height/10
    }
    // - - - - - - -
    Column{
        id: userSettingsColumn
        
        property bool loginInProgress: false

        anchors.top:spacer.bottom
        width: parent.width
        height: parent.height * 6/7 - spacer.height
        visible: true
        spacing: 20
        Text{
            id: textResult
            anchors.horizontalCenter: parent.horizontalCenter
            text:{
                if(AppState.loggedIn) return "Logged in:"
                else return "Not logged in"
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                text: "User: "
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                Layout.preferredWidth: parent.width * 2/5
                Layout.preferredHeight: parent.height
                Layout.alignment: Qt.AlignRight
                color: "gray"
                TextEdit{
                    id:usernameEdit
                    height: parent.height
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    selectByMouse: true
                    focus:true
                    text: {
                        if(AppState.loggedIn) return AppState.username
                        else return ""
                    }
                    readOnly: {return (AppState.loggedIn)?  true:false}
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                text: "Password"
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                Layout.preferredWidth: parent.width * 2/5
                Layout.preferredHeight: parent.height
                Layout.alignment: Qt.AlignRight
                color: "gray"
                TextEdit{
                    id: passwordEdit
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                    selectByMouse: true
                    text: {if (AppState.loggedIn) return  "***"
                    else return ""}
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                id: loginButton
                text: "Log in"
                Layout.alignment: Qt.AlignLeft
                onClicked:{
                  //  loginInProgress = true
                    RequestHandler.login(usernameEdit.text, passwordEdit.text, function(success, msg){

                        if(success){
                            console.log("Log in successful")
                            AppState.loggedIn = true
                        }else{
                            console.log("Log in failed:", msg)
                        }
                    })
                }
            }
            Button{
                text: "Sign up"
                Layout.alignment: Qt.AlignCenter
                onClicked:{
                    RequestHandler.signup(usernameEdit.text, passwordEdit.text, function(success, msg){
                        textResult.text(msg)
                        if(success){
                            console.log("Sign up successful")
                            AppState.loggedIn = true
                        }else{
                            console.log("Sign up failed:", msg)
                        }
                    })
                }
            }
            Button{
                text:"Log out"
                Layout.alignment: Qt.AlignRight
                onClicked:{
                    RequestHandler.logout(function(success, msg){
                        textResult.text(msg)
                        if(success){
                            console.log("You are logged out")
                            AppState.loggedIn = false
                        }else{
                            console.log("Log out failed:", msg)
                        }
                    })
                }
            }
        }
        Text{
            id: userEditResult
            
        }
    }
    Column{
        id: documentSettingsColumn
        visible: false

        anchors.top:spacer.bottom
        height: parent.height * 6/7 - spacer.height
        width: parent.width
        spacing: 10
        Text{
            id: textDocument
            anchors.horizontalCenter: parent.horizontalCenter
            text:{
                if(AppState.saved) return "Changes are saved"
                else return "Changes are NOT saved!"
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                text: "Document name: "
                height: parent.height/5
                Layout.alignment: Qt.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                Layout.preferredWidth: parent.width * 2/5
                Layout.preferredHeight: parent.height
                Layout.alignment: Qt.AlignRight
                color: "gray"
                TextEdit{
                    verticalAlignment: Text.AlignVCenter
                    id:documentNameEdit
                    anchors.fill: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    selectByMouse: true
                    text: {
                        if(AppState.loggedIn) return AppState.documentName
                        else return ""
                    }
                    readOnly: {return (AppState.loggedIn)?  true:false}
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                text: "Save"
                Layout.alignment: Qt.AlignLeft
                onClicked:{
                    RequestHandler.logout(function(success, msg){
                        textResult.text(msg)
                        if(success){
                            console.log("You are logged out")
                            AppState.loggedIn = false
                        }else{
                            console.log("Log out failed:", msg)
                        }
                    })
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                text: "Share with: "
                Layout.alignment: Qt.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                Layout.preferredWidth: parent.width * 2/5
                Layout.preferredHeight: parent.height
                Layout.alignment: Qt.AlignRight
                color: "gray"
                TextEdit{
                    id:shareWithEdit
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    selectByMouse: true
                    focus:true
                    text: ""
                    readOnly: {return (AppState.loggedIn)?  true:false}
                }
            }
        }

    }
    Column{
        id: documentsOverviewColumn
        visible: false
        anchors.top:spacer.bottom
        width: parent.width
        spacing: 10

    }
}

/*

onClicked:{
                if (WebSocket.isConnected) {
                    WebSocket.disconnectFromServer();
                } else {
                    WebSocket.connectToServer();
                }
            }
*/