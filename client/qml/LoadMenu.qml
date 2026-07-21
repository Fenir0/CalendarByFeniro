import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    color: backColor
    id: loadMenu_Window
    width: 400  
    height: 400


    property string backColor: '#e4fbfd'
    property string editColor: '#f0f0f0'
    property string loadMenu_Button_LogInColor: '#ffc174'
    property string loggedInButtonColor: '#a84343'
    property string editForbiddenColor: '#a1aaaa'
    property string buttonColor: '#6deeee'
    property string buttonUnavailableColor: '#6273e0'

    RowLayout{
        id: loadMenu_RowLayout_TopButtons
        width: parent.width
        height:parent.height/7
        anchors.top: parent.top
        Button{
            background: Rectangle{
                color: {
                    if(loadMenu_Column_User.visible) return buttonColor
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
                loadMenu_Column_User.visible = true
                loadMenu_Column_CurrentDocument.visible = false
                loadMenu_Column_DocumentsOverview.visible = false
            }
        }
        Button{
            background: Rectangle{
                color: {
                    if(loadMenu_Column_CurrentDocument.visible) return buttonColor
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
                loadMenu_Column_User.visible = false
                loadMenu_Column_DocumentsOverview.visible = false
                loadMenu_Column_CurrentDocument.visible = true
                }
                loadMenu_ListModel_User.clear()
                if(AppState.documentId != 0){
                    RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                        if(success){
                            var jsArray = JSON.parse(documentQString);
                            for(var i = 0; i < jsArray.length; i++){
                                loadMenu_ListModel_User.append({
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
                    if(loadMenu_Column_DocumentsOverview.visible) return buttonColor
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
                    loadMenu_Column_User.visible = false
                    loadMenu_Column_DocumentsOverview.visible = true
                    loadMenu_Column_CurrentDocument.visible = false
                
                    loadMenu_ListModel_Document.clear()

                    RequestHandler.loadList(function(success, documentQString){
                        if(success){
                            var jsArray = JSON.parse(documentQString);

                            for(var i = 0; i < jsArray.length; i++){
                                loadMenu_ListModel_Document.append({
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
        id: loadMenu_Rectangle_Spacer
        anchors.top: loadMenu_RowLayout_TopButtons.bottom
        color: "transparent"
        width: parent.width
        height: parent.height/10
    }
// =========================
// USER
// =========================
Column{
        id: loadMenu_Column_User
        
        property bool loginInProgress: false

        anchors.top:loadMenu_Rectangle_Spacer.bottom
        width: parent.width
        height: parent.height * 6/7 - loadMenu_Rectangle_Spacer.height
        visible: true
        spacing: 20
        Text{
            id: loadColumn_Text_IfLoggedIn
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
                id: loadMenu_Text_User
                text: "User: "
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: parent.height
                verticalAlignment: Text.AlignVCenter
                SequentialAnimation {
                    id: loadMenu_SeqAnim_BlinkUser
                    loops: 4
                    
                    ColorAnimation { 
                        target: loadMenu_Text_User
                        property: "color"
                        to: "red"
                        duration: 150 
                    }
                    ColorAnimation { 
                        target: loadMenu_Text_User
                        property: "color"
                        to: "black"
                        duration: 150
                    }
                }
            }
            Rectangle{
                Layout.preferredWidth: parent.width * 2/5
                Layout.preferredHeight: parent.height
                Layout.alignment: Qt.AlignRight
                color: editColor
                id: loadMenu_Rectangle_TextEdit
                border.color: "black"

                TextInput {
                    id: loadMenu_TextInput_Username
                    anchors.fill: parent
                    verticalAlignment: TextInput.AlignVCenter
                    selectByMouse: true
                    focus: true
                    wrapMode: TextInput.NoWrap 
                    clip: true
                    text: AppState.loggedIn ? AppState.username : ""
                    readOnly: AppState.loggedIn 
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            height: parent.height/8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                id: loadColumn_Text_Password
                text: "Password"
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: parent.height
                verticalAlignment: Text.AlignVCenter
                SequentialAnimation {
                    id: loadMenu_SeqAnim_pwdBlink
                    loops: 4
                    
                    ColorAnimation { 
                        target: loadColumn_Text_Password
                        property: "color"
                        to: "red"
                        duration: 150 
                    }
                    ColorAnimation { 
                        target: loadColumn_Text_Password
                        property: "color"
                        to: "black"
                        duration: 150
                    }
                }
            }
            Rectangle{
                Layout.preferredWidth: parent.width * 2/5
                Layout.preferredHeight: parent.height
                Layout.alignment: Qt.AlignRight
                color: editColor
                id: loadMenu_Rectangle_Password
                border.color: "black"
                TextInput {
                    id: loadMenu_TextInput_Password
                    anchors.fill: parent
                    verticalAlignment: TextInput.AlignVCenter
                    selectByMouse: true
                    focus: true
                    wrapMode: TextInput.NoWrap 
                    clip: true
                    text: AppState.loggedIn ? "***" : ""
                    readOnly: AppState.loggedIn 
                }
            }
        }
        RowLayout{
            width:parent.width * 4/5
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                id: loadMenu_Button_LogIn
                text: "Log in"
                Layout.alignment: Qt.AlignLeft
                background: Rectangle{
                    color:{ 
                    if(AppState.loggedIn) return loggedInButtonColor
                    else return loadMenu_Button_LogInColor
                    }
                    radius: 10
                }    
                onClicked:{
                    RequestHandler.login(loadMenu_TextInput_Username.text, loadMenu_TextInput_Password.text, function(success, msg){

                        if(success){
                            loadMenu_Rectangle_TextEdit.color = editForbiddenColor
                            loadMenu_Rectangle_Password.color = editForbiddenColor
                            console.log("Log in successful")
                            AppState.loggedIn = true
                        }else{
                            if(msg == "pwd"){
                                loadMenu_SeqAnim_pwdBlink.restart()
                            }
                            else if(msg == "user"){
                                loadMenu_SeqAnim_BlinkUser.restart()
                            }
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
                    else return loadMenu_Button_LogInColor
                    }
                    radius: 10
                }     
                onClicked:{
                    RequestHandler.signup(loadMenu_TextInput_Username.text, loadMenu_TextInput_Password.text, function(success, msg){
                        if(success){
                            loadMenu_Rectangle_TextEdit.color = editForbiddenColor
                            loadMenu_Rectangle_Password.color = editForbiddenColor
                            console.log("Sign up successful")
                            AppState.loggedIn = true
                        }else{
                            if(msg == "user"){
                                console.log(123)
                                loadMenu_SeqAnim_BlinkUser.restart()
                            }
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
                        else return loadMenu_Button_LogInColor
                    }
                    radius: 10
                }                
                onClicked:{
                    RequestHandler.logout(function(success, msg){
                        loadColumn_Text_IfLoggedIn.text(msg)
                        if(success){
                            loadMenu_Rectangle_TextEdit.color = editColor
                            loadMenu_Rectangle_Password.color = editColor
                            console.log("You are logged out")
                            AppState.loggedIn = false
                        }else{
                            console.log("Log out failed:", msg)
                        }
                    })
                }
            }
        }
    }
// =========================
// CURRENT DOCUMENT
// =========================
ListModel{
    id: loadMenu_ListModel_User
}
Column{
    id: loadMenu_Column_CurrentDocument
    visible: false

    anchors.top:loadMenu_Rectangle_Spacer.bottom
    height: parent.height * 6/7 - loadMenu_Rectangle_Spacer.height
    width: parent.width
    spacing: 10

    Text{
        anchors.horizontalCenter: parent.horizontalCenter
        text:{
            if(AppState.accessLevel == 2) return "You can only view"
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
                id:loadMenu_TextEdit_DocumentName
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
                    RequestHandler.create(loadMenu_TextEdit_DocumentName.text, DayDataHandler.getDataMapAsJSON(), function(success, msg){
                        loadColumn_Text_IfLoggedIn.text(msg)
                        if(success){
                            console.log("Document saved")
                        }else{
                            console.log("Failed to save:", msg)
                        }
                    })
                }
                else{
                    RequestHandler.rename(loadMenu_TextEdit_DocumentName.text, AppState.documentId, function(success, msg){
                        loadColumn_Text_IfLoggedIn.text(msg)
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
        width: parent.width * 4/5
        height: parent.height * 2/5
        clip: true
        anchors.horizontalCenter: parent.horizontalCenter
        model: loadMenu_ListModel_User
        delegate: UserListModel{
                userNameModel: model.userName
                userAccessLevelModel: model.accessLevel
                isCurrentUserOwner: model.isOwner
                userId: model.userId
        }
    }
    Connections {
        target: RequestHandler
        function onPeopleListChanged() {
            console.log("Reloading list of users")
            loadMenu_ListModel_User.clear()
            if(AppState.documentId != 0){
                RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                    if(success){
                        var jsArray = JSON.parse(documentQString);
                        for(var i = 0; i < jsArray.length; i++){
                            loadMenu_ListModel_User.append({
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
    Connections {
        target: WebSocket
        function pnDocumentsStateChanged() {
            console.log("Reloading list of users")
            loadMenu_ListModel_User.clear()
            if(AppState.documentId != 0){
                RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                    if(success){
                        var jsArray = JSON.parse(documentQString);
                        for(var i = 0; i < jsArray.length; i++){
                            loadMenu_ListModel_User.append({
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
}
// =========================
// LIST OF DOCUMENTS
// =========================
    ListModel{
        id: loadMenu_ListModel_Document
    }
    Column{
        id: loadMenu_Column_DocumentsOverview
        visible: false
            
        anchors.top:loadMenu_Rectangle_Spacer.bottom
        width: parent.width
        height: parent.height * 6/7 - loadMenu_Rectangle_Spacer.height
        spacing: 20
        ListView{
            clip: true
            width: parent.width * 4/5
            height: parent.height * 4/5

            anchors.horizontalCenter: parent.horizontalCenter
            model: loadMenu_ListModel_Document
            delegate: DocumentListModel{
                documentName: model.documentName
                documentId: model.documentId
            }
        }
    }
    Connections {
        target: RequestHandler
        function documentListChanged() {
            console.log("Reloading docs")
            loadMenu_ListModel_Document.clear()

            RequestHandler.loadList(function(success, documentQString){
                if(success){
                    var jsArray = JSON.parse(documentQString);

                    for(var i = 0; i < jsArray.length; i++){
                        loadMenu_ListModel_Document.append({
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
    Connections {
        target: WebSocket
        function onDocumentsStateChanged() {
            console.log("Reloading docs")
            loadMenu_ListModel_Document.clear()

            RequestHandler.loadList(function(success, documentQString){
                if(success){
                    var jsArray = JSON.parse(documentQString);

                    for(var i = 0; i < jsArray.length; i++){
                        loadMenu_ListModel_Document.append({
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

