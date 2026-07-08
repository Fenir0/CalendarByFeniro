import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

import Calendar

Window {
    width: 1050  
    height: 830  
    visible: true
    title: qsTr("CalendarByFeniro")

    property int dayWeekStart:      AppState.weekDayStartOfMonth
    property int dayAmountPrevious: AppState.dayAmountPrevious
    property int dayAmountCurrent : AppState.dayAmountCurrent

    Column{
        id: mainColumn
        anchors.fill: parent 
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        property string monthName: AppState.visibleMonth
        Button{
            id: monthButton
            text: AppState.visibleMonth
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
        TableView{
            id: mainGrid
            width: parent.width
            height: 750
            anchors.top: monthButton.bottom

            model: DayItemModel{}
            columnWidthProvider: function(column) { return 150; }
            rowHeightProvider: function(row) { return 150; }
            delegate: DayModel {
                required property int row
                required property int column
                property bool currentMonth

                dayOfMonth: {
                    var index = row*7+column
                    if(index + 1 - dayWeekStart < 0){
                        // PREVIOUS
                        d_month = AppState.previousMonth;
                        d_year =  AppState.previousYear;
                        currentMonth = false;
                        return index-dayWeekStart+dayAmountPrevious+2;
                    }
                    if((index-dayWeekStart+1)>=dayAmountCurrent){
                        // NEXT
                        d_month = AppState.nextMonth;
                        d_year =  AppState.nextYear;
                        currentMonth = false;
                        return  index-dayWeekStart-dayAmountCurrent+2;
                    }
                    // CURRENT
                    d_month = AppState.visibleMonth;
                    d_year =  AppState.visibleYear;
                    currentMonth = true;
                    return index-dayWeekStart+ 2;
                }
                d_day:   dayOfMonth

                dayOfWeek: model.day
                itemColor:{
                    if(AppState.highlightedDay == y_m_d) return "red"
                    else if(currentMonth) return "lightblue"
                    else return "blue"
                }
                y_m_d: d_year * 1e4 + d_month * 1e2 + d_day
                
                itemText : {return DayDataHandler.getContentByYMD(y_m_d);}
            
            MouseArea{
                anchors.fill: parent
                
                onClicked:{ 
                    AppState.highlightedDay = y_m_d;

                    }
                }
            }
        }
        RoundButton{
            id: editButton
            width: 60
            anchors.top: mainGrid.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            height: 40
            background: Rectangle{
                radius: editButton.radius
                color: "transparent"
                border.color: "black"
            }
            icon.source: "img/editButton.png"
            icon.color: "transparent"
            onClicked:{
                
            }
        }
        RoundButton{
            id: buttonNext
            width: 60
            anchors.top: mainGrid.bottom
            anchors.right: parent.right

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
        RoundButton{
            id: buttonPrevious
            width: 60
            anchors.top: mainGrid.bottom
            anchors.left: parent.left
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
    }
}