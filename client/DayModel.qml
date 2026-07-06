import QtQuick
import Calendar

Rectangle{
    id: day_base
    implicitWidth: 150
    implicitHeight: 150

    color: itemColor
    radius: 8
    border.color: "#ccaacc"

    property string itemText: ""
    property color  itemColor: "blue"
// SET ELSEWHERE
    property int dayOfWeek:  1
    property int dayOfMonth: 1

    property int d_day:   1
    property int d_month: 1
    property int d_year:  1
    property int y_m_d:   1

    visible: true
    Column{
    Text{
        text: dayOfMonth
    }
    Text{
        text: {
            switch(dayOfWeek){
                case 1:return "mon";
                case 2:return "tue";
                case 3:return "wed";
                case 4:return "thu";
                case 5:return "fri";
                case 6:return "sat";
                case 7:return "sun";
                default: return "___";
            }
        }
    }
    Text{
        text:itemText 
    }
    }
}