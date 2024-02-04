import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Rectangle{
    property int index : _index
    property var classHolder : _class
    property bool amountEnabled: true;
    property bool workState : _class.isWorking
    property var autoclicker : _autoclicker
    property string name
    default property alias contents: row.data
    border.color: "darkgray"
    color: _color
    id:root
    height:layout.height
    width:parent.width
    onWorkStateChanged:{
        if(workState){
            this.color="darkgreen"
        }
        else{
            this.color=_color
        }
    }
    ColumnLayout{
        enabled: !autoclicker.working;
        id:layout
        width:parent.width
        Layout.alignment: Qt.AlignVCenter
        RowLayout{

            Text{
                id:label
                text:index + ")" + name
            }
            Item {
                Layout.fillWidth: true
            }
            Button{
                id:b
                text:"remove"
                onClicked: autoclicker.deleteItem(index);
            }


        }
        RowLayout{
            id:row
            Text{
                visible: amountEnabled
                text:"Count"
            }
            TextField{
                visible: amountEnabled
                text: classHolder.amount
                validator: IntValidator {bottom: 1;}
                onEditingFinished: classHolder.amount = this.text
            }
            Text{
                text:"Delay"
            }
            TextField{
                text: classHolder.delay
                validator: IntValidator {bottom: 1; }
                onEditingFinished: classHolder.delay = this.text
            }
        }
    }
}
