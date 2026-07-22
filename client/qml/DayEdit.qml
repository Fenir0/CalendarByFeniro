import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window


import Calendar

Window{
    id: editDay_Window
    width: 350
    height: 280

    color: '#9db8d1'

    property int editDayOfMonth: -1
    property string editContent: ""

    
    Column{
        anchors.margins: 5
        anchors.fill: parent
        spacing: 10
        Rectangle{
            height: 30
            width: 170
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#dfdf7a"
            radius: 20
            border.color: "black"
            Text{
                anchors.fill: parent
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color:"#1700ff"
                text: Math.floor(editDayOfMonth/10000) + ' ' + Math.floor(editDayOfMonth/100)%100 + ' ' + editDayOfMonth%100 
            }  
        }
        Rectangle{
            id: dayEdit_EditText_Rectangle
            height: parent.height * 2/3
            width: parent.width * 6/7
            color: '#a1aaaa'
            border.color: "black"
            anchors.horizontalCenter: parent.horizontalCenter
            TextEdit{
                leftPadding: 5
                id: dayEdit_EditText_content
                text: editContent
                selectByMouse: true
                anchors.fill: parent
                clip:true
                property bool isUpdating: false
                focus: true
                onTextChanged:{
                    if(isUpdating) return

                    if (dayEdit_EditText_content.height === 0 || dayEdit_EditText_content.width === 0) {
                                return;
                    }

                    let lines = text.split("\n");

                    let modified = false;
                    var pos_ver = dayEdit_EditText_content.positionAt(1, dayEdit_EditText_content.height + 1);
                    var pos_hor = 16;
                    
                    for (let i = 0; i < lines.length; i++) {
                        if (lines[i].length > pos_hor) {
                            lines[i] = lines[i].substring(0, pos_hor);
                            modified = true;
                        }
                    }
                    var lineHeight = dayEdit_EditText_content.font.pixelSize * 1.2; // Approximate line height
                    var maxLines = 7;
                    
                    if (lines.length > maxLines) {
                        lines = lines.slice(0, maxLines);
                        modified = true;
                    }

                    if (modified) {
                        isUpdating = true
                        let cursor = cursorPosition; 
                        text = lines.join("\n");
                        cursorPosition = Math.min(cursor, text.length);
                        isUpdating = false
                    }
                }
                Keys.onEscapePressed: {
                    AppState.highlightedDay = -1
                    editDay_Window.close()
                    }
                Keys.onReturnPressed: dayEdit_Button_Ok.clicked()
            }
        }
        RoundButton{
            id: dayEdit_Button_Ok
            focus: true
            height: 40
            width: 40
            background: Rectangle{
                color: '#6deeee'
                border.color: "black"
                radius: 10   
            }
            icon.source: "../img/okButton.png"
            icon.color: "transparent"
            anchors.horizontalCenter:parent.horizontalCenter
            
            onClicked:{
                DayDataHandler.setContentByYMD(editDayOfMonth, dayEdit_EditText_content.getText(0, dayEdit_EditText_content.length))
                if(AppState.loggedIn){
                    RequestHandler.update(editDayOfMonth, dayEdit_EditText_content.text, 
                        function(success, msg){
                            if(success){
                                AppState.saved = true;
                                console.log(msg)
                            }
                            else console.log("Failed to update on server")
                        })
                }
                editDay_Window.close();
                AppState.highlightedDay = -1;
            }
        }
    }
}
