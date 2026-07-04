import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import calendar.DayModel
 
Window {
    width: mainGrid.width
    height: mainGrid.height
    visible: true
    title: qsTr("CalendarByFeniro")
    Column{
        id: mainColumn

        Button{
            width: 80
            anchors.centerIn: parent
            height: 30
            text: "Cl"
        }

        ListModel{
            id: day_model
            ListElement{name: "1"; bgColor : "lightcoral"}
            ListElement{name: "2"; bgColor : "lightcoral"}
            ListElement{name: "3"; bgColor : "lightcoral"}
            ListElement{name: "4"; bgColor : "lightcoral"}
            ListElement{name: "5"; bgColor : "lightcoral"}
            ListElement{name: "6"; bgColor : "lightcoral"}
            ListElement{name: "7"; bgColor : "lightcoral"}
            ListElement{name: "1"; bgColor : "lightcoral"}
            ListElement{name: "2"; bgColor : "lightcoral"}
            ListElement{name: "3"; bgColor : "lightcoral"}
            ListElement{name: "4"; bgColor : "lightcoral"}
            ListElement{name: "5"; bgColor : "lightcoral"}
            ListElement{name: "6"; bgColor : "lightcoral"}
            ListElement{name: "7"; bgColor : "lightcoral"}
            ListElement{name: "1"; bgColor : "lightcoral"}
            ListElement{name: "2"; bgColor : "lightcoral"}
            ListElement{name: "3"; bgColor : "lightcoral"}
            ListElement{name: "4"; bgColor : "lightcoral"}
            ListElement{name: "5"; bgColor : "lightcoral"}
            ListElement{name: "6"; bgColor : "lightcoral"}
            ListElement{name: "7"; bgColor : "lightcoral"}            
            ListElement{name: "1"; bgColor : "lightcoral"}
            ListElement{name: "2"; bgColor : "lightcoral"}
            ListElement{name: "3"; bgColor : "lightcoral"}
            ListElement{name: "4"; bgColor : "lightcoral"}
            ListElement{name: "5"; bgColor : "lightcoral"}
            ListElement{name: "6"; bgColor : "lightcoral"}
            ListElement{name: "7"; bgColor : "lightcoral"}
            ListElement{name: "1"; bgColor : "lightcoral"}
            ListElement{name: "2"; bgColor : "lightcoral"}
            ListElement{name: "3"; bgColor : "lightcoral"}
            ListElement{name: "4"; bgColor : "lightcoral"}
            ListElement{name: "5"; bgColor : "lightcoral"}
            ListElement{name: "6"; bgColor : "lightcoral"}
            ListElement{name: "7"; bgColor : "lightcoral"}
        }
        
        GridLayout{
            id: mainGrid

            columns: 7
            rows: 5
            property int dayWeekStart: 1
            property int dayAmountPrevious: 1
            property int dayAmountCurrent: 1

            property int day:   1
            property int month: 1
            property int year:  1
            Repeater{
                model: DayModel
                DayModel {
                    dayOfMonth: ((index + 1 - dayWeekStart < 0)? 
                        index-dayWeekStart+dayAmountPrevious+1
                        : (index-dayWeekStart+1)>=dayAmountCurrent? 
                                (index-dayWeekStart-dayAmountCurrent+1):
                                index-dayWeekStart+1) + 1
                    dayOfWeek: model.name
                    itemColor: model.bgColor

                    d_day: day
                    d_month: month
                    d_year:  year

                    y_m_d: year * 10e4 + month * 10e2 + day
                }
            }
 
        }
    }
}