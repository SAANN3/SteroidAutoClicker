import QtQuick
import QtQuick.Controls
AbstractDelegate{
    name: "Clicker"
    Text{
        text:"X:"
    }
    TextField{
        text: classHolder.x
        validator: IntValidator {bottom: -9999;}
        onEditingFinished: classHolder.x = this.text
    }
    Text{
        text:"Y:"
    }
    TextField{
        text: classHolder.y
        validator: IntValidator {bottom: -9999;}
        onEditingFinished: classHolder.y = this.text
    }
    RadioButton{
        checked: classHolder.side === -2
        text: "not clicking"
        onClicked: {
            if(checked){
                classHolder.side = -2
            }
        }
    }
    RadioButton{
        checked: classHolder.side === -1
        text: "left click"
        onClicked: {
            if(checked){
                classHolder.side = -1
            }
        }
    }
    RadioButton{
        checked: classHolder.side === 1
        text: "right click "
        onClicked: {
            if(checked){
                classHolder.side = 1
            }
        }
    }
}
