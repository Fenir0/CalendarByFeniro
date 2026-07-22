import QtQuick
import Calendar

Rectangle{
    property string itemImageSource: ""

    Image{
        anchors.centerIn: parent
        source: itemImageSource
        fillMode: Image.PreserveAspectCrop
        width: 146
        height: 146

    }

    id: day_base
    implicitWidth: 150
    implicitHeight: 150

    radius: 8
    color: "#549ee2"
    border.color: "black"
    border.width: 2

    property string itemText: ""
    property color  itemColor: "blue"

    property int dayOfWeek:  1
    property int dayOfMonth: 1

    property int d_day:   1
    property int d_month: 1
    property int d_year:  1
    property int y_m_d:   1

    visible: true
    Column{
        padding: 10
        Text {text: dayOfMonth; color: '#000000'}

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
            color: '#0f5e25'
            font.bold: true

        }
        Text{
            text:itemText 
        }
    }
}