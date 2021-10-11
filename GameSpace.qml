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

    Connections {
        target: tableSpace

        function onGameEnd() {
            dialogRec.visible = true
        }
        function onNewGameStart(){
            dialogRec.visible = false
        }
    }

//    Rectangle{
//        id: tmp
//        width: 10
//        height: 10
//        color: "red"
//        opacity: 0
//    }

    Component{
        id: cellDelegate
        Item {
            id: itemCell
            required property string display
            readonly property real size: 42.9
            property bool visibleBall: true
            property double opacityBall: 0

            implicitWidth: size
            implicitHeight: size

            Rectangle {
                id: cell
                anchors.centerIn: parent

                width: size
                height: size

                color: "#facaf5"

                border.color: '#009999'
                Rectangle{
                    id: ball
                    visible: itemCell.visibleBall
                    opacity: opacityBall
                    height: parent.height-5
                    width: height
                    color: display
                    border.width: display ? 0 : 10
                    border.color: '#009999'

                    radius: width
                    anchors.centerIn: parent

                    states: [
                        State {
                            name: "changeColor"
                            when: (ball.onColorChanged &&
                                   ball.color != "#ffffff" &&
                                   itemCell != firstBall &&
                                   itemCell != secondBall)
                        },
                        State {
                            name: "colorClear"
                            when: (ball.onColorChanged &&
                                   ball.color == "#ffffff" )
                        }
                    ]

                    transitions: [
                        Transition {
                            from: "*"
                            to: "changeColor"
                            OpacityAnimator{
                                target: ball;
                                from: 0.1;
                                to: 1;
                                duration: 1000
                                running: true
                            }
                        },
                        Transition {
                            from: "*"
                            to: "colorClear"
                            OpacityAnimator{
                                target: ball;
                                from: 1;
                                to: 0;
                                duration: 1000
                                running: true
                            }
                        }
                    ]
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (firstBall == null)
                    {
                        firstBall = itemCell
                        firstX = firstBall.x
                        firstY = firstBall.y
                        firstCol = column
                        firstRow = row

                        if (ball.color == "#ffffff")
                        {
                            clearValueFirst()
                            return
                        }

                        tmpBall.x = firstX+2
                        tmpBall.y = firstY+2
                        tmpBall.color = ball.color
                        tmpBall.visible = ball.visible
                        tmpBall.border.color = "#000000"
                        tmpBall.border.width = 3
                        firstBall.visibleBall = false

                        console.log("First remember")
                    }
                    else
                    {
                        if (secondBall == null)
                        {
                            secondBall = itemCell
                            secondX = secondBall.x
                            secondY = secondBall.y
                            secondRow = row
                            secondCol = column

                            if (ball.color != "#ffffff")
                            {
                                clearValueSecond()
                                return
                            }

                            console.log("Run animation")
                            gameSpace.state = 'step'
                            tableSpace.setData(tableSpace.index(firstRow,firstCol), "#ffffff")
                            firstBall.visibleBall = true
                            firstBall.opacityBall = 0
                        }
                    }
                }
            }
        }
    }

    function clearValueFirst()
    {
        console.log("Clear First!")
        firstBall = null
        firstX = null
        firstY = null
        firstRow = null
        firstCol = null
        return;
    }

    function clearValueSecond()
    {
        console.log("Clear Second!")
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
        reuseItems: true

        anchors.fill: parent
    }
    Rectangle {
        id: dialogRec
        visible: false

        color: "#ffffff"
        opacity: 0.5

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
                    clearValueFirst()
                    clearValueSecond()
                    gameSpace.state = ''
                    tmpBall.visible = !tmpBall.visible
                    tableSpace.step()
                }
            }
        }
    ]
}
