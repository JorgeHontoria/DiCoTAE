// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import "../common"

Popup {
    id: popup
    closePolicy: Popup.CloseOnPressOutside
    property alias text: popupLabel.text
    property alias buttonText: okButton.text
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    implicitHeight: columnContent.height + 30
    implicitWidth: parent.width * .9
    ColumnLayout {
        id: columnContent
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: 10
        RowLayout {
            LabelSubheading {
                id: popupLabel
                topPadding: 20
                leftPadding: 8
                rightPadding: 8
                text: ""
                color: popupTextColor
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            } // popupLabel
        } // row label
        RowLayout {
            ButtonFlat {
                id: okButton
                text: ""
                textColor: accentColor
                onClicked: {
                    popup.close()
                }
            } // okButton
        } // row button
    } // col layout

    onAboutToHide: {
        stopTimer()
    }
    onAboutToShow: {
        closeTimer.start()
    }

    Timer {
        id: closeTimer
        interval: 4000
        repeat: false
        onTriggered: {
            popup.close()
        }
    }
    function stopTimer() {
        closeTimer.stop()
    }
} // popup
