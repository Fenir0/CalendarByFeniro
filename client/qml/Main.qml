import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import Qt.labs.platform

import Calendar


Window {
    id: main_Window_Root
    width: 1050  
    height: 818  
    visible: true
    title: qsTr("CalendarByFeniro")
    color: '#2127d2'

    property var dayEditWindow: null
    
    MouseArea{
        anchors.fill: parent
        onClicked:{
            monthSelection.visible = false
        }
    }

    Button{
        width: 100
        height: 40
        id: mainWindow_Button_MonthSelect
        text: AppState.visibleMonthString
        background: Rectangle{
            color:'#b5eef5'
            border.color: "black"
            radius: 5
        } 

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        onClicked: {monthSelection.visible = !monthSelection.visible}
    } 
    ComboBox{
        anchors.left: mainWindow_Button_MonthSelect.right
        anchors.verticalCenter: mainWindow_Button_MonthSelect.verticalCenter
        anchors.leftMargin: 10
        id: mainWindow_ComboBox_YearSelect
        width: 70
        background: Rectangle{
            color: "#dfdf7a"
        }
        model: ListModel{
            id: mainWindow_ComboBox_yearModel
            Component.onCompleted: {
            for (var year = 2020; year <= 2030; year++) {
                mainWindow_ComboBox_yearModel.append({ text: year.toString() })
            }
        }
        }
        currentIndex: AppState.visibleYear - 2020
        onActivated: {
            AppState.visibleYear = currentText
        }
    }

    RoundButton{
        height:40
        width: 40
        id: mainWindow_RoundButton_Network
        background: Rectangle{
            radius: mainWindow_RoundButton_Network.radius
            color: {
                if(!AppState.loggedIn) return "#e4fbfd"
                else return '#41f034'
            }
            border.color: "black"
        }
        
        icon.source: "../img/networkButton.png"
        icon.color: "transparent"
        onClicked:{
            monthSelection.visible = false

            if (WebSocket.isConnected) {
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
            else{
                var component = Qt.createComponent("ServerIpEnter.qml")

                if (component.status === Component.Ready) {
                    var window = component.createObject(null, {})
                    if (window) {
                        window.show()
                    } else {
                        console.error("Error")
                    } 
                    
                } else if (component.status === Component.Error) {
                    console.error("Error loading ServerIpEnter.qml:", component.errorString())
                }
            }
        }
    }
    DayItemModel{
        id: mainWindow_DayItemModel
    }
    Connections {
        target: AppState
        function onParameterChanged() {
            console.log("Reloading: ", AppState.visibleYear, AppState.visibleMonth)
            mainWindow_DayItemModel.loadMonth(AppState.visibleYear, AppState.visibleMonth)
        }
    }
    Connections {
        target: DayDataHandler
        function onNewDataSet() {
            console.log("Reloading: ", AppState.visibleYear, AppState.visibleMonth)
            mainWindow_DayItemModel.loadMonth(AppState.visibleYear, AppState.visibleMonth)
        }
    }
    Component.onCompleted: {
        mainWindow_DayItemModel.loadMonth(AppState.visibleYear, AppState.visibleMonth)
    }

    Column{
        id: mainWindow_Column
        height: main_Window_Root.height - mainWindow_RoundButton_Network.height
        width: parent.width 
        anchors.top: mainWindow_Button_MonthSelect.bottom
        anchors.bottom: parent.bottom
        property string monthName: AppState.visibleMonth
        z: 0
        TableView{
            focus: true
            id: mainWindow_TableView_Month
            width: parent.width
            height: 750

            model: mainWindow_DayItemModel
            columnWidthProvider: function(column) { return mainWindow_TableView_Month.width/7; }
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
                        return "../../img/monthBackgroundChosen.png"
                    } else if (model.isCurrent) {
                        if(DayDataHandler.isDaySaved(y_m_d)){
                            return "../../img/monthBackgroundSaved.png"
                        }
                        return "../../img/monthBackgroundCurrent.png"
                    } else {
                        return "../../img/monthBackground.png"
                    }
                }
                
                itemText: model.content
            
                MouseArea{
                    anchors.fill: parent

                    Timer {
                        id: mainWindow_Timer_Clicker
                        interval: 250
                    }
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked:{ 
                        mainWindow_Timer_Clicker.start() 
                        monthSelection.visible = false
                        
                        if (mouse.button == Qt.RightButton && AppState.highlightedDay == y_m_d) {
                            AppState.highlightedDay = -1;
                        }
                        else if (mouse.button === Qt.LeftButton) {
                            AppState.highlightedDay = y_m_d;
                        }
                    }
                    
                    onDoubleClicked:{
                        mainWindow_Timer_Clicker.stop()
                        var component = Qt.createComponent("DayEdit.qml")

                        if (component.status === Component.Ready) {
                            console.log(AppState.highlightedDay)
                            console.log(DayDataHandler.getContentByYMD(AppState.highlightedDay))
                            dayEditWindow = component.createObject(main_Window_Root, {
                                "editDayOfMonth": AppState.highlightedDay,
                                "editContent": DayDataHandler.getContentByYMD(AppState.highlightedDay)
                            })
                            if (dayEditWindow) {
                                dayEditWindow.show()
                            } else {
                                console.error("Error: createObject returned null even though component was Ready.")
                            }
                            
                        } else if (component.status === Component.Error) {
                            console.error("Error loading DayEdit.qml:", component.errorString())
                        }
                    }
                }
            }
            // Keys.onUpPressed: {
            //     if(AppState.highlightedDay == -1) return
            //     AppState.highlightedDay -= 7
            // }
            // Keys.onDownPressed: {
            //     if(AppState.highlightedDay == -1) return
            //     AppState.highlightedDay += 7
            // }
            // Keys.onLeftPressed: {
            //     if(AppState.highlightedDay == -1) return
            //     if(AppState.getWeekDay(AppState.highlightedDay) != 0) AppState.highlightedDay -= 1
            // }
            // Keys.onRightPressed:{
            //     if(AppState.highlightedDay == -1) return
            //     if(AppState.getWeekDay(AppState.highlightedDay) != 6) AppState.highlightedDay += 1
            // }
            Keys.onReturnPressed: {
                if(AppState.highlightedDay == -1) return
                editButton.clicked()
            }
        }
        RowLayout{
            width: parent.width - 40
            height: parent.height - mainWindow_TableView_Month.height - 4
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle{
                id:mainWindow_Rectanle_Previous
                radius: 10
                color: "#e4fbfd"
                border.color: "black"
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: 90

                Text{
                    text: "<"
                    color:"#2127d2"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                }
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: mainWindow_Rectanle_Previous.color = '#3fa6f9'
                    onExited: mainWindow_Rectanle_Previous.color = "#e4fbfd"
                    onClicked:{
                        AppState.visibleMonth = AppState.visibleMonth - 1;
                    }
                }
            }
            RoundButton{
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                id: mainWindow_RoundButton_Edit
                Layout.preferredWidth: 90
                //anchors.horizontalCenter: parent.horizontalCenter

                background: Rectangle{
                    radius: mainWindow_RoundButton_Edit.radius
                    color:  {
                        if(AppState.highlightedDay == -1) return "transparent"
                        else return '#41f034'
                    }
                    border.color: "black"
                }
                icon.source: "../img/editButton.png"
                icon.color: {
                    if(AppState.accessLevel < 2) return "transparent"
                    else return "#a84343"    
                }
                enabled: AppState.accessLevel < 2
                onClicked: {
                    if(AppState.accessLevel == 2 || AppState.highlightedDay == -1) return
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
            Rectangle{
                id:mainWindow_Rectangle_ButtonNext
                radius: 10
                color: "#e4fbfd"
                border.color: "black"
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: 90

                Text{
                    text: ">"
                    color:"#2127d2"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                }
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: mainWindow_Rectangle_ButtonNext.color = '#3fa6f9'
                    onExited: mainWindow_Rectangle_ButtonNext.color = "#e4fbfd"
                    onClicked:{
                        AppState.visibleMonth = AppState.visibleMonth + 1;
                    }
                }
            }
        }
    }

    ListModel{
            id: mainWindow_ListModel_Month
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
            anchors.top: mainWindow_Button_MonthSelect.bottom
                
            id: monthSelection
            visible: false
            color: "gray"
            Grid{
                rows: 4
                columns: 3
                spacing: 5
                anchors.centerIn: parent
                Repeater {
                    model: mainWindow_ListModel_Month
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