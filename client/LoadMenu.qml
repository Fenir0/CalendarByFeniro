import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    color: '#e4fbfd'
    id: loadMenuWindow
    width: 400  
    height: 400

    property string editColor: '#a1aaaa'
    property string buttonColor: '#6deeee'
    property string buttonUnavailableColor: '#6273e0'

    RowLayout{
        id: topButtonsLayout
        width: parent.width
        height:parent.height/7
        anchors.top: parent.top
        Button{
            background: Rectangle{
                color: {
                    if(userSettingsColumn.visible) return buttonColor
                    else buttonUnavailableColor
                }
            }

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
                documentsOverviewColumn.visible = false

            }
        }
        Button{
            background: Rectangle{
                color: {
                    if(documentSettingsColumn.visible) return buttonColor
                    else buttonUnavailableColor
                }
            }
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
                documentsOverviewColumn.visible = false
                documentSettingsColumn.visible = true
                }
            }
        }
        Button{
            background: Rectangle{
                color: {
                    if(documentsOverviewColumn.visible) return buttonColor
                    else buttonUnavailableColor
                }
            }
            Layout.preferredWidth:parent.width/3
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignRight
            Text{
                text: "Your\ndocuments"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter 
            }
            onClicked: {
                if(AppState.loggedIn) {
                    userSettingsColumn.visible = false
                    documentsOverviewColumn.visible = true
                    documentSettingsColumn.visible = false
                }
                documentModel.clear()

                RequestHandler.loadList(function(success, documentQString){
                    if(success){
                        var jsArray = JSON.parse(documentQString);

                        for(var i = 0; i < jsArray.length; i++){
                            documentModel.append({
                                "documentName": jsArray[i].name
                                "documentId": jsArray[i].id
                            })
                        }
                        console.log("Loaded")
                    }  else{
                        console.log("Failed to fetch")
                    }         

                })
            }
        }
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
                color: editColor
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
                color: editColor
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
    // - - - - - - -
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
                if(AppState.documentName == "") return "No document loaded from server.\npSave this as new, or load from server"
                else {
                    if(AppState.saved) return "Changes are saved"
                    else return "Changes are NOT saved!"
                }
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
                color: editColor
                TextEdit{
                    verticalAlignment: Text.AlignVCenter
                    id:documentNameEdit
                    anchors.fill: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    selectByMouse: true
                    text:  AppState.documentName
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: {
                if(AppState.documentId > -1) return true
                else return false
            }
            Button{
                text: "Save"
                Layout.alignment: Qt.AlignLeft
                onClicked:{
                    RequestHandler.create(documentNameEdit.text, DayDataHandler.getDataMapAsJSON(), function(success, msg){
                        textResult.text(msg)
                        if(success){
                            console.log("Document saved")
                            // CHANGE VIEW
                        }else{
                            console.log("Failed to save:", msg)
                        }
                    })
                }
            }
            Button{

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
                color: editColor
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
    ListModel{
        id: documentModel
    }
    Column{
        id: documentsOverviewColumn
        visible: false
            
        anchors.top:spacer.bottom
        width: parent.width
        height: parent.height * 6/7 - spacer.height
        spacing: 20
        ListView{
            id: documentListView
            width: parent.width * 4/5
            height: parent.height * 4/5

            anchors.horizontalCenter: parent.horizontalCenter
            model: documentModel
            delegate: DocumentListModel{
                documentName: model.documentName
                documentId: model.documentId
            }
        }
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