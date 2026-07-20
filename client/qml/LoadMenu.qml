import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    color: backColor
    id: loadMenuWindow
    width: 400  
    height: 400


    property string backColor: '#e4fbfd'
    property string editColor: '#f0f0f0'
    property string loginButtonColor: '#ffc174'
    property string loggedInButtonColor: '#a84343'
    property string editForbiddenColor: '#a1aaaa'
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
                radius: 10
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
                radius: 10
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
                userModel.clear()
                if(AppState.documentId != 0){
                    RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                        if(success){
                            var jsArray = JSON.parse(documentQString);
                            for(var i = 0; i < jsArray.length; i++){
                                userModel.append({
                                    "userName": jsArray[i].name,
                                    "accessLevel": jsArray[i].accessLevel,
                                    "isOwner": jsArray[i].isOwner,
                                    "userId": jsArray[i].id
                                })
                            }
                            console.log("Loaded people")
                        }  else{
                            console.log("Failed to fetch")
                        }         

                    })
                }
            }
        }
        Button{
            background: Rectangle{
                color: {
                    if(documentsOverviewColumn.visible) return buttonColor
                    else buttonUnavailableColor
                }
                radius: 10
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
                
                    documentModel.clear()

                    RequestHandler.loadList(function(success, documentQString){
                        if(success){
                            var jsArray = JSON.parse(documentQString);

                            for(var i = 0; i < jsArray.length; i++){
                                documentModel.append({
                                    "documentName": jsArray[i].name,
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
    }
    Rectangle{
        id:spacer
        anchors.top: topButtonsLayout.bottom
        color: "transparent"
        width: parent.width
        height: parent.height/10
    }
// =========================
// USER
// =========================
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
                id: usernameEditRectangle
                border.color: "black"
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
                id: passwordEditRectangle
                border.color: "black"
                TextEdit{
                    id: passwordEdit
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                    selectByMouse: true
                    text: {
                        if (AppState.loggedIn) return  "***"
                        else return ""
                    }
                    readOnly: {return (AppState.loggedIn)?  true:false}
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
                background: Rectangle{
                    color:{ 
                    if(AppState.loggedIn) return loggedInButtonColor
                    else return loginButtonColor
                    }
                    radius: 10
                }    
                onClicked:{
                  //  loginInProgress = true
                    RequestHandler.login(usernameEdit.text, passwordEdit.text, function(success, msg){

                        if(success){
                            usernameEditRectangle.color = editForbiddenColor
                            passwordEditRectangle.color = editForbiddenColor
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
                background: Rectangle{
                    color:{ 
                    if(AppState.loggedIn) return loggedInButtonColor
                    else return loginButtonColor
                    }
                    radius: 10
                }     
                onClicked:{
                    RequestHandler.signup(usernameEdit.text, passwordEdit.text, function(success, msg){
                        textResult.text(msg)
                        if(success){
                            usernameEditRectangle.color = editForbiddenColor
                            passwordEditRectangle.color = editForbiddenColor
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
                background: Rectangle{
                    color:{ 
                        if(!AppState.loggedIn) return loggedInButtonColor
                        else return loginButtonColor
                    }
                    radius: 10
                }                
                onClicked:{
                    RequestHandler.logout(function(success, msg){
                        textResult.text(msg)
                        if(success){
                            usernameEditRectangle.color = editColor
                            passwordEditRectangle.color = editColor
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
// =========================
// CURRENT DOCUMENT
// =========================
    ListModel{
        id: userModel
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
                if(AppState.documentId == 0) return "No document loaded from server.\nSave this as new, or load from server"
                else {
                    return "Changes are saved"
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
            Button{
                text: {
                    if (AppState.documentId != 0) return "Rename"
                    else return "Save"
                }
                Layout.alignment: Qt.AlignLeft
                onClicked:{
                    if(AppState.documentId == 0) {
                        RequestHandler.create(documentNameEdit.text, DayDataHandler.getDataMapAsJSON(), function(success, msg){
                            textResult.text(msg)
                            if(success){
                                console.log("Document saved")
                            }else{
                                console.log("Failed to save:", msg)
                            }
                        })
                    }
                    else{
                        RequestHandler.rename(documentNameEdit.text, AppState.documentId, function(success, msg){
                            textResult.text(msg)
                            if(success){
                                console.log("Document renamed")
                            }
                            else{
                                console.log("Failed to rename:", msg)
                            }
                        })
                    }
                }
            }
        }

        ListView{ 
            id: userListView
            width: parent.width * 4/5
            height: parent.height * 2/5

            anchors.horizontalCenter: parent.horizontalCenter
            model: userModel
            delegate: UserListModel{
                    userNameModel: model.userName
                    userAccessLevelModel: model.accessLevel
                    isCurrentUserOwner: model.isOwner
                    userId: model.userId
            }
        }
        Button{
            text: "Change access rules"
            width: parent.width * 3/5
            height: parent.height * 1/6
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle{
                color: loggedInButtonColor
                radius: 10
            }
        }
    }
    // - - - - - 
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

