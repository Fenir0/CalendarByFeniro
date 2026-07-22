import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar
Window{
    color: backColor
    id: menu_Window
    width: 400  
    height: 400


    property string backColor: '#e4fbfd'
    property string editColor: '#f0f0f0'
    property string menu_Button_LogInColor: '#ffc174'
    property string loggedInButtonColor: '#a84343'
    property string editForbiddenColor: '#a1aaaa'
    property string buttonColor: '#6deeee'
    property string buttonUnavailableColor: '#6273e0'

    RowLayout{
        id: menu_RowLayout_TopButtons
        width: parent.width
        height:parent.height/7
        anchors.top: parent.top
        Button{
            background: Rectangle{
                color: {
                    if(menu_Column_User.visible) return buttonColor
                    else buttonUnavailableColor
                }
                border.color: "black"
                bottomLeftRadius:  10
                bottomRightRadius: 10
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
                menu_Column_User.visible = true
                menu_Column_CurrentDocument.visible = false
                menu_Column_DocumentsOverview.visible = false
            }
        }
        Button{
            background: Rectangle{
                color: {
                    if(menu_Column_CurrentDocument.visible) return buttonColor
                    else buttonUnavailableColor
                }
                border.color: "black"
                bottomLeftRadius:  10
                bottomRightRadius: 10
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
                menu_Column_User.visible = false
                menu_Column_DocumentsOverview.visible = false
                menu_Column_CurrentDocument.visible = true
                }
                menu_ListModel_User.clear()
                if(AppState.documentId != 0){
                    RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                        if(success){
                            var jsArray = JSON.parse(documentQString);
                            for(var i = 0; i < jsArray.length; i++){
                                menu_ListModel_User.append({
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
            id: menu_Button_LoadDocs
            background: Rectangle{
                color: {
                    if(menu_Column_DocumentsOverview.visible) return buttonColor
                    else buttonUnavailableColor
                }
                border.color: "black"
                bottomLeftRadius:  10
                bottomRightRadius: 10
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
                    menu_Column_User.visible = false
                    menu_Column_DocumentsOverview.visible = true
                    menu_Column_CurrentDocument.visible = false
                
                    menu_ListModel_Document.clear()

                    RequestHandler.loadList(function(success, documentQString){
                        if(success){
                            var jsArray = JSON.parse(documentQString);

                            for(var i = 0; i < jsArray.length; i++){
                                menu_ListModel_Document.append({
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
        id: menu_Rectangle_Spacer
        anchors.top: menu_RowLayout_TopButtons.bottom
        color: "transparent"
        width: parent.width
        height: parent.height/10
    }
// =========================
// USER
// =========================
Column{
        id: menu_Column_User
        
        property bool loginInProgress: false

        anchors.top:menu_Rectangle_Spacer.bottom
        width: parent.width
        height: parent.height * 6/7 - menu_Rectangle_Spacer.height
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
                id: menu_Text_User
                text: "User: "
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: parent.height
                verticalAlignment: Text.AlignVCenter
                SequentialAnimation {
                    id: menu_SeqAnim_BlinkUser
                    loops: 4
                    
                    ColorAnimation { 
                        target: menu_Text_User
                        property: "color"
                        to: "red"
                        duration: 150 
                    }
                    ColorAnimation { 
                        target: menu_Text_User
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
                id: menu_Rectangle_TextEdit
                border.color: "black"

                TextInput {
                    id: menu_TextInput_Username
                    anchors.fill: parent
                    leftPadding: 5
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
                    id: menu_SeqAnim_pwdBlink
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
                id: menu_Rectangle_Password
                border.color: "black"
                TextInput {
                    leftPadding: 5
                    id: menu_TextInput_Password
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
                id: menu_Button_LogIn
                text: "Log in"
                Layout.alignment: Qt.AlignLeft
                background: Rectangle{
                    color:{ 
                    if(AppState.loggedIn) return loggedInButtonColor
                    else return menu_Button_LogInColor
                    }
                    radius: 10
                }    
                onClicked:{
                    RequestHandler.login(menu_TextInput_Username.text, menu_TextInput_Password.text, function(success, msg){

                        if(success){
                            menu_Rectangle_TextEdit.color = editForbiddenColor
                            menu_Rectangle_Password.color = editForbiddenColor
                            console.log("Log in successful")
                            AppState.loggedIn = true
                        }else{
                            if(msg == "pwd"){
                                menu_SeqAnim_pwdBlink.restart()
                            }
                            else if(msg == "user"){
                                menu_SeqAnim_BlinkUser.restart()
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
                    else return menu_Button_LogInColor
                    }
                    radius: 10
                }     
                onClicked:{
                    RequestHandler.signup(menu_TextInput_Username.text, menu_TextInput_Password.text, function(success, msg){
                        if(success){
                            menu_Rectangle_TextEdit.color = editForbiddenColor
                            menu_Rectangle_Password.color = editForbiddenColor
                            console.log("Sign up successful")
                            AppState.loggedIn = true
                        }else{
                            if(msg == "user"){
                                console.log(123)
                                menu_SeqAnim_BlinkUser.restart()
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
                        else return menu_Button_LogInColor
                    }
                    radius: 10
                }                
                onClicked:{
                    RequestHandler.logout(function(success, msg){
                        loadColumn_Text_IfLoggedIn.text(msg)
                        if(success){
                            menu_Rectangle_TextEdit.color = editColor
                            menu_Rectangle_Password.color = editColor
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
    id: menu_ListModel_User
}
Column{
    id: menu_Column_CurrentDocument
    visible: false

    anchors.top:menu_Rectangle_Spacer.bottom
    height: parent.height * 6/7 - menu_Rectangle_Spacer.height
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
                leftPadding: 5
                verticalAlignment: Text.AlignVCenter
                id:menu_TextEdit_DocumentName
                anchors.fill: parent
                anchors.horizontalCenter: parent.horizontalCenter
                selectByMouse: true
                text:  AppState.documentName
                SequentialAnimation {
                id: menu_SeqAnim_DocNameBlink
                loops: 4
                
                ColorAnimation { 
                    target: menu_TextEdit_DocumentName
                    property: "color"
                    to: "red"
                    duration: 150 
                }
                ColorAnimation { 
                    target: menu_TextEdit_DocumentName
                    property: "color"
                    to: "black"
                    duration: 150
                }
            }
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
                    RequestHandler.create(menu_TextEdit_DocumentName.text, DayDataHandler.getDataMapAsJSON(), function(success, msg){
                        loadColumn_Text_IfLoggedIn.text(msg)
                        if(success){
                            console.log("Document saved")
                        }else{
                            if(msg == "name") menu_SeqAnim_DocNameBlink.restart()
                        }
                    })
                }
                else{
                    RequestHandler.rename(menu_TextEdit_DocumentName.text, AppState.documentId, function(success, msg){
                        loadColumn_Text_IfLoggedIn.text(msg)
                        if(success){
                            console.log("Document renamed")
                        }
                        else{
                            if(msg == "name") menu_SeqAnim_DocNameBlink.restart()
                        }
                    })
                }
            }
        }
        Button{
            text: "Create new"
            visible: AppState.documentId != 0
            Layout.alignment: Qt.AlignCenter
            onClicked:{
                RequestHandler.create(menu_TextEdit_DocumentName.text, DayDataHandler.getEmptyJSON(), function(success, msg){
                    loadColumn_Text_IfLoggedIn.text(msg)
                    if(success){
                        console.log("Document saved")
                        DayDataHandler.setClearJSON();
                    }else{
                        if(msg == "name") menu_SeqAnim_DocNameBlink.restart()
                    }
                })
            }
        }
        Button{
            text: "Close"
            visible: AppState.documentId != 0
            Layout.alignment: Qt.AlignRight
            onClicked:{
                AppState.documentId = 0
                AppState.documentName = ""
            }
        }
    }

    ListView{ 
        width: parent.width * 4/5
        height: parent.height * 2/5
        clip: true
        anchors.horizontalCenter: parent.horizontalCenter
        model: menu_ListModel_User
        delegate: UserListModel{
                userNameModel: model.userName
                userAccessLevelModel: model.accessLevel
                isCurrentUserOwner: model.isOwner
                userId: model.userId
        }
    }
    Connections {
        target: RequestHandler
        function onUserListChanged() {
            console.log("Reloading list of users")
            menu_ListModel_User.clear()
            if(AppState.documentId != 0){
                RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                    if(success){
                        var jsArray = JSON.parse(documentQString);
                        for(var i = 0; i < jsArray.length; i++){
                            menu_ListModel_User.append({
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
            menu_ListModel_User.clear()
            if(AppState.documentId != 0){
                RequestHandler.loadListOfPeople(AppState.documentId,  function(success, documentQString){
                    if(success){
                        var jsArray = JSON.parse(documentQString);
                        for(var i = 0; i < jsArray.length; i++){
                            menu_ListModel_User.append({
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
        id: menu_ListModel_Document
    }
    Column{
        id: menu_Column_DocumentsOverview
        visible: false
            
        anchors.top:menu_Rectangle_Spacer.bottom
        width: parent.width
        height: parent.height * 6/7 - menu_Rectangle_Spacer.height
        spacing: 20
        ListView{
            clip: true
            width: parent.width * 4/5
            height: parent.height * 4/5

            anchors.horizontalCenter: parent.horizontalCenter
            model: menu_ListModel_Document
            delegate: DocumentListModel{
                documentName: model.documentName
                documentId: model.documentId
            }
        }
    }
    Connections {
        target: RequestHandler
        function onDocumentListChanged() {
            console.log("Reloading docs")
            menu_Button_LoadDocs.clicked()
        }
    }
    Connections {
        target: WebSocket
        function onDocumentsStateChanged() {
            console.log("Reloading docs")
            menu_ListModel_Document.clear()

            RequestHandler.loadList(function(success, documentQString){
                if(success){
                    var jsArray = JSON.parse(documentQString);

                    for(var i = 0; i < jsArray.length; i++){
                        menu_ListModel_Document.append({
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

