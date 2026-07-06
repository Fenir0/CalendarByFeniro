import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

import Calendar

Window {
    width: 1050  
    height: 800  
    visible: true
    title: qsTr("CalendarByFeniro")

    property int dayWeekStart:      AppState.weekDayStartOfMonth
    property int dayAmountPrevious: AppState.dayAmountPrevious
    property int dayAmountCurrent : AppState.dayAmountCurrent

    Column{
        id: mainColumn
        anchors.fill: parent 
        
        TableView{
            id: mainGrid
            width: parent.width
            height: parent.height - 40

            model: DayItemModel{}
            columnWidthProvider: function(column) { return 150; }
            rowHeightProvider: function(row) { return 150; }
            delegate: DayModel {
                required property int row
                required property int column

                dayOfMonth: {
                    var index = row*7+column
                    if(index + 1 - dayWeekStart < 0){
                        // PREVIOUS
                        d_month = AppState.previousMonth;
                        d_year =  AppState.previousYear;
                        return index-dayWeekStart+dayAmountPrevious+2;
                    }
                    if((index-dayWeekStart+1)>=dayAmountCurrent){
                        // NEXT
                        d_month = AppState.nextMonth;
                        d_year =  AppState.nextYear;
                        return  index-dayWeekStart-dayAmountCurrent+2;
                    }
                    // CURRENT
                    d_month = AppState.visibleMonth;
                    d_year =  AppState.visibleYear;
                    return index-dayWeekStart+ 2;
                }
                d_day:   dayOfMonth

                dayOfWeek: model.day
                itemText : model.content
                itemColor:{
                    if(AppState.highlightedDay == y_m_d) return "red"
                    else return "blue"
                }


                y_m_d: d_year * 1e4 + d_month * 1e2 + d_day
            
            MouseArea{
                anchors.fill: parent
                
                onClicked:{ AppState.highlightedDay = y_m_d}
                
                }
            }
        }
        RoundButton{
            id: editButton
            width: 60
            anchors.top: mainGrid
            anchors.horizontalCenter: parent.horizontalCenter

            height: 40
            background: Rectangle{
                radius: editButton.radius
                color: "transparent"
                border.color: "black"
            }
            icon.source: "img/editButton.png"
            icon.color: "transparent"

        }
    }
}