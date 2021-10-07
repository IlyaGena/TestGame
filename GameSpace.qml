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
    property var secondBall: null
    property var secondX: null
    property var secondY: null

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

                        tmpBall.x = firstX+2
                        tmpBall.y = firstY+2
                        tmpBall.color = ball.color
                        tmpBall.visible = ball.visible
                        ball.visible = false
                    }
                    else
                    {
                        if (secondBall == null)
                        {
                            secondBall = itemCell
                            secondX = secondBall.x
                            secondY = secondBall.y
                            tableGame.state = 'step'
                            sleep(100)

                            tableSpace.setData(tableSpace.index(firstBall.cow,firstBall.column), "white")
                            tableSpace.setData(tableSpace.index(itemCell.cow,itemCell.column), tmpBall.color)

                            clearValue()
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
        secondBall = null
        secondX = null
        secondY = null
    }

    TableView{
        id: tableGame
        model: tableSpace
        delegate: cellDelegate

        anchors.fill: parent

        Component.onCompleted: {
            tableSpace.click()
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
                    console.log("Stop Animation!")
//                    tmpBall.visible = false
                }
            }
        ]
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
}




//Rectangle {
//    id: gameSpace
//    color: 'white'
//    width: height
//    property var firstBall: null
//    property var firstX: null
//    property var firstY: null
//    property var secondBall: null
//    property var secondX: null
//    property var secondY: null

//    BackEnd{
//        id: backEnd
//    }

//    Connections {
//        target: backEnd

//        function onGameEnd() {
//            dialogRec.visible = !dialogRec.visible
//        }
//    }

//    Component{
//        id: cellDelegate
//        Item {
//            required property string display
////                readonly property real size: Math.round(tableView.width/9-2)
//            readonly property real size: 44

//            implicitWidth: size
//            implicitHeight: size

//            Rectangle{
//                id: cell
//                anchors.centerIn: parent

//                width: size
//                height: size

//                border.color: '#009999'

//                Rectangle{
//                    id: ball
//                    anchors.fill: parent
//                    color: display
//                    border.width: display ? 0 : 10
//                    border.color: '#009999'

//                    radius: width
//                    anchors.centerIn: parent
//                }
//            }
//            MouseArea {
//                anchors.fill: parent

//                onClicked: {
//                    if (firstBall == null)
//                    {
//                        firstBall = cell
//                        firstX = this.parent.x
//                        firstY = this.parent.y
//                        console.log("FirstX:" + firstX)
//                        console.log("FirstY:" + firstY)
//                    }
//                    else
//                    {
//                        if (secondBall == null)
//                        {
//                            secondBall = cell
//                            secondX = this.parent.x
//                            secondY = this.parent.y
//                            console.log("SecondX:" + secondX)
//                            console.log("SecondY:" + secondY)
//                            secondBall.visible = false
//                            gameSpace.state = 'step'
//                        }
//                    }
//                }
//            }
//        }
//    }

//    TableView {
//        id: tableView
//        model: backEnd
//        delegate: cellDelegate

//        anchors.fill: parent
//        anchors.verticalCenter: parent.verticalCenter
//        anchors.horizontalCenter: parent.horizontalCenter
//        Component.onCompleted: {
//            backEnd.click()
//        }
//    }

//    Rectangle {
//        id: dialogRec
//        visible: false

//        color: "white"

//        Label {
//            text: qsTr("Игра окончена!")
//            font.pointSize: 35
//            font.family: "Helvetica"
//            font.italic: true

//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.verticalCenter: parent.verticalCenter
//        }

//        anchors.fill: parent
//    }

//    anchors.top: labelScore.bottom
//    anchors.topMargin: 20
//    anchors.bottom: parent.bottom
//    anchors.bottomMargin: 20
//    anchors.horizontalCenter: parent.horizontalCenter

//    function clearValue()
//    {
//        console.log("Clear!")
//        firstBall = null
//        firstX = null
//        firstY = null
//        secondBall = null
//        secondX = null
//        secondY = null
//    }

//    states: [
//        State {
//            name: "step"
////            PropertyChanges {
////                target: secondBall;
////                x: firstX
////                y: firstY
////            }
//            PropertyChanges {
//                target: firstBall.parent;
//                x: secondX
//                y: secondY
//            }

//        }
//    ]
//    transitions: [

//        // When transitioning to 'bottomLeft' move x,y over a duration of 2 seconds,
//        // with InOutQuad easing function.
//        Transition {
//            from: "*"
//            to: "step"
//            NumberAnimation {
//                properties: "x,y"
//                easing.type: Easing.Linear
//                duration: 2000
//            }
//        }
//    ]
//}



