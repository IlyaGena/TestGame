import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import my.app.backend 1.1

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("The test Game")

    TableSpace{
        id: tableSpace
    }
    Connections {
        target: tableSpace
        function onChangeScore(score) {
            countScore.text = score
        }
    }

    Label {
        id: labelScore
        text: qsTr("Счет:")
        font.pointSize: 15
        font.family: "Helvetica"
        font.italic: true

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: button_new_game.verticalCenter
    }

    Text {
        id: countScore
        text: tableSpace.getScore()
        font.pointSize: 15
        font.italic: true

        anchors.left: labelScore.right
        anchors.leftMargin: 3
        anchors.top: labelScore.top
    }

    Button {
        id: button_new_game
        width: label_btn.width + 20
        height: label_btn.height + 20

        contentItem: Rectangle {
            color: "#00000000"
            Label {
                id: label_btn

                text: qsTr("New Game")
                font.family: "Helvetica"
                font.pointSize: 15
                font.bold: true

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        background: Rectangle {
            color: "#c9c7b5"
            border.color: "#ffffff"
            radius: 10
        }

        onClicked: {
            tableSpace.newGame()
            tableSpace.step()
        }

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
    }

    GameSpace{
    anchors.top: button_new_game.bottom
    anchors.topMargin: 20
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    anchors.horizontalCenter: parent.horizontalCenter
    }
}
