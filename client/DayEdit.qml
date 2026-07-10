import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window


import Calendar

Window{
    id: editDayWindow
    width: 350  
    height: 350

    color: '#9db8d1'

    property int editDayOfMonth: -1
    property string editContent: ""


    Text{
        anchors.top: parent.top
        height: 30
        width: 170
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        color:"#1700ff"
        id: dateText
        text: Math.floor(editDayOfMonth/10000) + ' ' + Math.floor(editDayOfMonth/100)%100 + ' ' + editDayOfMonth%100 
        anchors.horizontalCenter: parent.horizontalCenter
    }  
    TextEdit{
        id: editTextContent
        text: editContent
        selectByMouse: true
        focus: true
        height: 285
        width: 300
        anchors.top:  dateText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        clip:true
        onTextChanged:{

            var pos_h = editTextContent.positionAt(1, editTextContent.height + 1);
            if(editTextContent.length >= pos_h){
                editTextContent.remove(pos_h, editTextContent.length)
            }
        }
    }
    Button{
        text: "Finish edit"
        anchors.top: editTextContent.bottom
        anchors.horizontalCenter:parent.horizontalCenter
        onClicked:{
            DayDataHandler.setContentByYMD(editDayOfMonth, editTextContent.getText(0, editTextContent.length))
            editDayWindow.close();
        }
    }

}