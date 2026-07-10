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

    property int dayWeekStart:      AppState.weekDayStartOfMonth
    property int dayAmountPrevious: AppState.dayAmountPrevious
    property int dayAmountCurrent : AppState.dayAmountCurrent

    Button{
        width: 100
        height: 30
        id: monthButton
        text: AppState.visibleMonth

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        onClicked:monthSelection.visible = !monthSelection.visible
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

            model: DayItemModel{}
            columnWidthProvider: function(column) { return 150; }
            rowHeightProvider: function(row) { return 150; }
            delegate: DayModel {
                required property int row
                required property int column
                property bool currentMonth
                property int index: row*7+column
                dayOfMonth: {
                    if(index + 1 - dayWeekStart < 0){
                        // PREVIOUS
                        return index-dayWeekStart+dayAmountPrevious+2;
                    }
                    if((index-dayWeekStart+1)>=dayAmountCurrent){
                        // NEXT
                        return  index-dayWeekStart-dayAmountCurrent+2;
                    }
                    // CURRENT
                    return index-dayWeekStart+ 2;
                }
                d_day:   dayOfMonth
                d_month:{
                    if (index + 1 - dayWeekStart < 0) return AppState.previousMonth;
                    if ((index - dayWeekStart + 1) >= dayAmountCurrent) return AppState.nextMonth;
                    return AppState.visibleMonth;
                }
                d_year: {
                    if (index + 1 - dayWeekStart < 0) return AppState.previousYear;
                    if ((index - dayWeekStart + 1) >= dayAmountCurrent) return AppState.nextYear;
                    return AppState.visibleYear;
                }
                dayOfWeek: model.day 
                currentMonth: (index + 1 - dayWeekStart >= 0) && ((index - dayWeekStart + 1) < dayAmountCurrent)
                y_m_d: d_year * 1e4 + d_month * 1e2 + d_day

                itemImageSource: {
                    if (y_m_d == AppState.highlightedDay) {
                        return "img/monthBackgroundChosen.png"
                    } else if (currentMonth) {
                        return "img/monthBackgroundCurrent.png"
                    } else {
                        return "img/monthBackground.png"
                    }
                }
                
                itemText : {return DayDataHandler.getContentByYMD(y_m_d);}
            
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