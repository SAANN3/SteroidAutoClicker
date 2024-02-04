import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Autoclicker
ApplicationWindow {
    minimumWidth: 880
    width: 880
    height: 480
    visible: true
    id:root
    title: qsTr("AutoClicker")
    AutoClicker{
        id:autoclicker
    }
    FileDialog{
        property int type
        id: fileDialog
        onAccepted: autoclicker.workWithFile(selectedFile,type)


    }

    Connections {
        target : Application
        function onAboutToQuit(){
            autoclicker.guiExited();
        }
    }

    Column{
        anchors.fill: parent
        height:root.height
        RowLayout{
            id:buttonRow
            width:root.width
            Button{
                enabled: !autoclicker.working;
                text: "start"
                onClicked: autoclicker.startWork();
            }
            Button{
                enabled: autoclicker.working;
                text:"stop"
                onClicked: autoclicker.stopWork();
            }
            RowLayout{
                enabled:!autoclicker.working;
                Item {
                    Layout.fillWidth: true
                }
                Button{
                    enabled: !autoclicker.working;
                    text:"import"
                    onClicked:{
                        fileDialog.fileMode = FileDialog.OpenFile
                        fileDialog.type = 1
                        fileDialog.open();
                    }
                }
                Button{
                    enabled: !autoclicker.working;
                    text:"export"
                    onClicked:{
                        fileDialog.fileMode = FileDialog.SaveFile
                        fileDialog.type = 2
                        fileDialog.open();
                    }
                }
                Text{
                    text:"Count:"
                }

                TextField{
                    text: autoclicker.repeatAmount
                    validator: IntValidator {bottom: 1;}
                    onEditingFinished:autoclicker.repeatAmount = this.text
                }

                Text{
                    lineHeight: 1.0
                    text:autoclicker.globalPos
                }

                Button{
                    text:"Add item"
                    onClicked:autoclicker.addItem(choice.currentIndex);
                }
                ComboBox{
                    id:choice
                    model: ["Timer","Clicker","Scroller"]
                }

            }


        }

        ListView{
            clip: true
            height:root.height - buttonRow.height
            width:root.width
            model:autoclicker.jobs
            delegate: Component{

                id: delegate
                Loader{
                    width:root.width
                    source: {
                        if(autoclicker.jobs[index].type == 0)
                            return "SleepDelegate.qml"
                        if(autoclicker.jobs[index].type == 1)
                            return "ClickDelegate.qml"
                        if(autoclicker.jobs[index].type == 2)
                            return "MouseWheelDelegate.qml"

                    }

                    property int _index:index
                    property var _class:autoclicker.jobs[index]
                    property var _autoclicker:autoclicker
                    property var _color:root.color

                }
            }

        }
    }

}
