import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import my.app.backend 1.1

Rectangle {
    id: gameSpace
    color: '#afb8ed'
    width: height
    property var firstBall: null
    property var firstX: null
    property var firstY: null
    property var firstRow: null
    property var firstCol: null
    property var secondBall: null
    property var secondX: null
    property var secondY: null
    property var secondRow: null
    property var secondCol: null

    TableSpace{
        id: tableSpace
    }
    Connections {
        target: tableSpace

        function onGameEnd() {
            dialogRec.visible = !dialogRec.visible
        }
    }

    Component{
        id: cellDelegate
        Item {
            id: itemCell
            required property string display
            readonly property real size: 42.9

            implicitWidth: size
            implicitHeight: size

            Rectangle {
                id: cell
                anchors.centerIn: parent

                width: size
                height: size

                border.color: '#009999'

                Rectangle{
                    id: ball
                    height: parent.height-5
                    width: height
                    color: display
                    border.width: display ? 0 : 10
                    border.color: '#009999'

                    radius: width
                    anchors.centerIn: parent
                }
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    console.log(cell)
                    if (firstBall == null)
                    {
                        firstBall = itemCell
                        firstX = firstBall.x
                        firstY = firstBall.y
                        firstCol = column
                        firstRow = row

                        tmpBall.x = firstX+2
                        tmpBall.y = firstY+2
                        tmpBall.color = ball.color
                        tmpBall.visible = ball.visible
                    }
                    else
                    {
                        if (secondBall == null)
                        {
                            console.log("Run animation")
                            secondBall = itemCell
                            secondX = secondBall.x
                            secondY = secondBall.y
                            secondRow = row
                            secondCol = column
                            gameSpace.state = 'step'

                            tableSpace.setData(tableSpace.index(firstRow,firstCol), "white")
                        }
                    }
                }
            }
//            MouseArea{
//                anchors.fill: parent
//                onDoubleClicked: {
//                    tableSpace.click()
//                }
//            }
        }
    }

    function clearValue()
    {
        console.log("Clear!")
        firstBall = null
        firstX = null
        firstY = null
        firstRow = null
        firstCol = null
        secondBall = null
        secondX = null
        secondY = null
        secondRow = null
        secondCol = null
        return;
    }

    TableView{
        id: tableGame
        model: tableSpace
        delegate: cellDelegate

        anchors.fill: parent

        Component.onCompleted: {
            tableSpace.click()
        }
    }
    Rectangle {
        id: dialogRec
        visible: false

        color: "white"

        Label {
            text: qsTr("Игра окончена!")
            font.pointSize: 35
            font.family: "Helvetica"
            font.italic: true

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        anchors.fill: parent
    }

    Rectangle{
        id: tmpBall
        width: 42.9-5
        height: width
        radius: width
        color:"black"
        x: 0
        y: 0
        visible: false
    }

    states: [
        State {
            name: "step"
            PropertyChanges {
                target: tmpBall;
                x: secondX+2
                y: secondY+2
                visible: true
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "step"
            NumberAnimation {
                properties: "x,y"
                easing.type: Easing.Linear
                duration: 1000
            }
            onRunningChanged: {
                if (!this.running) {
                    console.log("Stop Animation!")
                    tableSpace.setData(tableSpace.index(secondRow,secondCol), tmpBall.color)
                    tmpBall.visible = false
                    clearValue()
                    gameSpace.state = ''
                }
            }
        }
    ]
}
