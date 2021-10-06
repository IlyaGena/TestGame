import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import my.app.backend 1.1

Rectangle {
    id: gameSpace
    color: '#b9faf0'
    width: height

    BackEnd{
        id: backEnd
    }

    Component{
        id: cellDelegate
        Item {
            required property string display
//                readonly property real size: Math.round(tableView.width/9-2)
            readonly property real size: 44

            implicitWidth: size
            implicitHeight: size

            Rectangle{
                anchors.centerIn: parent
                color: display

                width: size
                height: size

                border.color: '#009999'
            }
            MouseArea {
                anchors.fill: parent

                onDoubleClicked: {
                    console.log(row)
                    console.log(column)
                    backEnd.click(backEnd.index(0, 0), backEnd.index(row, column))
                }
            }
        }
    }

    TableView {
        id: tableView
        model: backEnd
        delegate: cellDelegate

        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    anchors.top: labelScore.bottom
    anchors.topMargin: 20
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    anchors.horizontalCenter: parent.horizontalCenter
}

