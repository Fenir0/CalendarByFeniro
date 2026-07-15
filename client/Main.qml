import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import Calendar


Window {
    id: root
    width: 1050  
    height: 830  
    visible: true
    title: qsTr("CalendarByFeniro")
    color: "#549ee2"


    Button{
        width: 100
        height: 30
        id: monthButton
        text: AppState.visibleMonth

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        onClicked: {monthSelection.visible = !monthSelection.visible}
    } 
    RoundButton{
        text: "L"
        radius: 25
        onClicked:{
            if (!WebSocket.isConnected) {
                WebSocket.connectToServer();
            }

            var component = Qt.createComponent("LoadMenu.qml")

            if (component.status === Component.Ready) {
                var window = component.createObject(null, {})
                if (window) {
                    window.show()
                } else {
                    console.error("Error")
                } 
                
            } else if (component.status === Component.Error) {
                console.error("Error loading DayModel.qml:", component.errorString())
            }
        }
    }
    DayItemModel{
        id: dayItemModel
    }
    Connections {
        target: AppState
        function onParameterChanged() {
            console.log("Reloading: ", AppState.visibleYear, AppState.visibleMonth)
            dayItemModel.loadMonth(AppState.visibleYear, AppState.visibleMonth)
        }
    }
    Component.onCompleted: {
        dayItemModel.loadMonth(AppState.visibleYear, AppState.visibleMonth)
    }

    Column{
        id: mainColumn
        width: parent.width 
        anchors.top: monthButton.bottom
        anchors.bottom: parent.bottom
        property string monthName: AppState.visibleMonth
        z: 0
        TableView{
            id: mainGrid
            width: parent.width
            height: 750

            model: dayItemModel
            columnWidthProvider: function(column) { return 150; }
            rowHeightProvider: function(row) { return 150; }
            delegate: DayModel {
                required property int row
                required property int column

                property bool currentMonth

                dayOfMonth: model.day
                d_day:      model.day
                d_month:    model.month
                d_year:     model.year
                dayOfWeek:  model.dayOfWeek 
                currentMonth: model.isCurrent
                y_m_d: model.year * 1e4 + model.month * 1e2 + model.day

                itemImageSource: {
                    if (y_m_d == AppState.highlightedDay) {
                        return "img/monthBackgroundChosen.png"
                    } else if (model.isCurrent) {
                        return "img/monthBackgroundCurrent.png"
                    } else {
                        return "img/monthBackground.png"
                    }
                }
                
                itemText: model.content
            
                MouseArea{
                    anchors.fill: parent
                    onClicked:{ 
                        AppState.highlightedDay = y_m_d;
                        }
                    }
                }
        }
        RowLayout{
            width: parent.width
            RoundButton{
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                id: buttonPrevious
                Layout.preferredWidth: 90
                //anchors.left: parent.left
                text: "<"
                height: 40
                background: Rectangle{
                    radius: buttonPrevious.radius
                    color: "transparent"
                    border.color: "black"
                }
                onClicked:{
                    AppState.visibleMonth = AppState.visibleMonth - 1;
                }
            }
            RoundButton{
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                id: editButton
                Layout.preferredWidth: 90
                //anchors.horizontalCenter: parent.horizontalCenter

                height: 40
                background: Rectangle{
                    radius: editButton.radius
                    color: "transparent"
                    border.color: "black"
                }
                icon.source: "img/editButton.png"
                icon.color: "transparent"
                onClicked: {
                    var component = Qt.createComponent("DayEdit.qml")

                    if (component.status === Component.Ready) {
                        var window = component.createObject(null, {
                            "editDayOfMonth": AppState.highlightedDay,
                            "editContent": DayDataHandler.getContentByYMD(AppState.highlightedDay)
                        })
                        if (window) {
                            window.show()
                        } else {
                            console.error("Error: createObject returned null even though component was Ready.")
                        }
                        
                    } else if (component.status === Component.Error) {
                        console.error("Error loading DayEdit.qml:", component.errorString())
                    }
                }
            }
            RoundButton{
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                id: buttonNext
                Layout.preferredWidth: 90
                //anchors.right: parent.right

                height: 40
                text: ">"
                background: Rectangle{
                    radius: buttonNext.radius
                    color: "transparent"
                    border.color: "black"
                }
                onClicked:{
                    AppState.visibleMonth = AppState.visibleMonth + 1;
                }
            }
        }
    }
    ListModel{
            id: monthModel
            ListElement{name: "Jan"; monthId: 1}
            ListElement{name: "Feb"; monthId: 2}
            ListElement{name: "Mar"; monthId: 3}
            ListElement{name: "Apr"; monthId: 4}
            ListElement{name: "May"; monthId: 5}
            ListElement{name: "Jun"; monthId: 6}
            ListElement{name: "Jul"; monthId: 7}
            ListElement{name: "Aug"; monthId: 8}
            ListElement{name: "Sep"; monthId: 9}
            ListElement{name: "Oct"; monthId: 10}
            ListElement{name: "Nov"; monthId: 11}
            ListElement{name: "Dec"; monthId: 11}
        }
        Rectangle{
            width: 175
            height: 155
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: monthButton.bottom
                
            id: monthSelection
            visible: false
            color: "gray"
            Grid{
                rows: 4
                columns: 3
                spacing: 5
                anchors.centerIn: parent
                Repeater {
                    model: monthModel
                    Rectangle{
                    width: 55
                    height: 35
                    color: {
                        if(AppState.visibleMonth == index + 1) return "gray"
                        else return "white"
                    }
                    border.color: "black"
                    Text{
                        text: model.name
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                        AppState.visibleMonth = index + 1;
                        }
                    }
                    }
                }
                z: 1
            }
        }
}