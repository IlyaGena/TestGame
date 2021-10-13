import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import my.app.backend 1.1

Rectangle {
    id: gameSpace
    color: '#afb8ed'
    width: height

    // переменные необходимые для реализации анимации
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

    // обработка сигналов от модели
    Connections {
        target: tableSpace

        function onGameEnd() {
            dialogRec.visible = true
        }
        function onNewGameStart(){
            dialogRec.visible = false
        }
    }

    // обработка сигнала о изменении счета
    Connections {
        target: tableSpace
        function onChangeScore() {
            gameSpace.state = 'scoreHide'
        }
    }

    // делегат TableView
    Component{
        id: cellDelegate

        Item {

            id: itemCell

            required property string display
            property string size: tableGame.width/tableSpace.columnCount()
            property bool visibleBall: true
            property double opacityBall: 0

            implicitWidth: size
            implicitHeight: size

            // ячейка
            Rectangle {
                id: cell
                width: size
                height: size
                color: "#60bbf7"
                border.color: '#009999'

                anchors.centerIn: parent

                // шарик ячейки
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

                    // анимация появления и исчезновения шариков

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
                                from: 0;
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

            // обработка выбора первого шарика и место его перемещения
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (firstBall == null)
                    {
                        // инициализация параметров
                        firstBall = itemCell
                        firstX = firstBall.x
                        firstY = firstBall.y
                        firstCol = column
                        firstRow = row

                        // если выбран не цветной шарик - очищаем
                        if (ball.color == "#ffffff")
                        {
                            clearValueFirst()
                            return
                        }

                        // перемещение временного шарика
                        tmpBall.x = firstX+2
                        tmpBall.y = firstY+2
                        tmpBall.color = ball.color
                        tmpBall.visible = ball.visible
                        tmpBall.border.color = "#eeff00"
                        tmpBall.border.width = 3

                        // убираем первый шарик
                        firstBall.visibleBall = false
                    }
                    else
                    {
                        if (secondBall == null)
                        {
                            // инициализация параметров
                            secondBall = itemCell
                            secondX = secondBall.x
                            secondY = secondBall.y
                            secondRow = row
                            secondCol = column

                            // если выбран цветной шарик - очищаем
                            if (ball.color != "#ffffff")
                            {
                                clearValueSecond()
                                return
                            }

                            // анимация перемещения временного шарика и изменение значений первой ячейки
                            gameSpace.state = 'step'
                            tableSpace.setData(tableSpace.index(firstRow,firstCol), "#ffffff")
                            // показываем шарик
                            firstBall.visibleBall = true
                            firstBall.opacityBall = 0
                        }
                    }
                }
            }
        }
    }

    // методы очистки параметров
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

    // игровое поле
    TableView{
        id: tableGame
        model: tableSpace
        delegate: cellDelegate

        anchors.fill: parent
    }

    // квадрат окончания игры
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

    // временный шарик
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

    // анимация передвижения временного шарика и изменение счета
    states: [
        State {
            name: "step"
            PropertyChanges {
                target: tmpBall;
                x: secondX+2
                y: secondY+2
                visible: true
            }
        },
        State {
            name: "scoreHide"
        },
        State {
            name: "scoreShow"
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
                    // изменение параметра второй ячейки
                    tableSpace.setData(tableSpace.index(secondRow,secondCol), tmpBall.color)
                    // очищение параметров
                    clearValueFirst()
                    clearValueSecond()
                    gameSpace.state = ''
                    // прячем временных шарик
                    tmpBall.visible = !tmpBall.visible
                    // запускаем генерацию новых шариков
                    tableSpace.step()
                }
            }
        },
        Transition {
            from: "*"
            to: "scoreHide"
            OpacityAnimator{
                target: countScore;
                from: 1;
                to: 0;
                duration: 1000
                running: true
            }
            onRunningChanged: {
                if (!this.running) {
                    countScore.text = tableSpace.getScore()
                    gameSpace.state = 'scoreShow'
                }
            }
        },
        Transition {
            from: "*"
            to: "scoreShow"
            OpacityAnimator{
                target: countScore;
                from: 0;
                to: 1;
                duration: 1000
                running: true
            }
            onRunningChanged: {
                if (!this.running) {
                    countScore.text = tableSpace.getScore()
                    gameSpace.state = ''
                }
            }
        }
    ]

    onWidthChanged: {
        tableGame.forceLayout();
    }
}
