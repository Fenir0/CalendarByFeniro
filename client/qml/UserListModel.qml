import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar

Rectangle{
    property string userNameModel: ""
    property int userAccessLevelModel: 0
    property bool isCurrentUserOwner: false
    property int userId: 0
    color: '#dfdf7a'
    width: ListView.view ? ListView.view.width : 300 
    height: 30
    border.color: "black"
    RowLayout{
        anchors.fill: parent
        anchors.margins: 5
        Text{
            Layout.preferredHeight: parent.height
            text: userNameModel
            Layout.preferredWidth: 150
            clip:true
            leftPadding: 30
            verticalAlignment: Text.AlignVCenter
        }
        Text{
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignCenter
            text: {
                if(userAccessLevelModel == 0) return "Owner"
                if(userAccessLevelModel == 1) return "Editor"
                if(userAccessLevelModel == 2) return "Reader"
            }
            visible: AppState.accessLevel != 0
        }
        ComboBox{
            id: userlistmode_combobox_level
            Layout.preferredHeight: parent.height
            visible: AppState.accessLevel == 0 && userNameModel != AppState.username
            Layout.alignment: Qt.AlignCenter
            model: ["Editor", "Reader"]
            currentIndex: userAccessLevelModel - 1
            onActivated: {
                RequestHandler.share(AppState.documentId, userNameModel, userlistmode_combobox_level.currentText, function(success, msg){
                    if(success){
                        console.log("changed successfully")
                        dialogShare_Window.close()
                    }else {
                        if(msg == "Nouser"){
                            dialogShare_SeqAnim_Username.start()
                        }
                    }
                })
            }
        }
        RoundButton{
            visible: AppState.accessLevel == 0 && userNameModel != AppState.username
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            //anchors.verticalCenter: parent.verticalCenter
            id: documentListModel_RoundButton_Download
            background: Rectangle{
                radius: 10
                color: "transparent"
                border.color: "black"
            }
            icon.source: "../img/documentTrashButton.png"
            icon.color: "transparent"
            onClicked:{
                RequestHandler.share(AppState.documentId, userNameModel, "exclude", function(success, msg){
                    if(success){
                        console.log("Deleted successfully")
                    }
                })
            }
        }
        
    }
    
}