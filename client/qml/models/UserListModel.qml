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
            visible: isCurrentUserOwner
        }
        ComboBox{
            Layout.preferredHeight: parent.height
            visible: !isCurrentUserOwner
            Layout.alignment: Qt.AlignCenter
            model: ["Editor", "Reader"]
        }
        
    }
    
}