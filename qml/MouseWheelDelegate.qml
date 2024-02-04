import QtQuick
import QtQuick.Controls
AbstractDelegate{
    name: "Scroller"
    Text{
        text:"Scroll direction:"
    }
    Row{
        RadioButton{
            checked: classHolder.direction
            text: "up"
            onClicked: {
                if(checked){
                    classHolder.direction = true
                }
            }
        }
        RadioButton{
            checked: !classHolder.direction
            text: "down"
            onClicked: {
                if(checked){
                    classHolder.direction = false
                }
            }
        }
    }

}
