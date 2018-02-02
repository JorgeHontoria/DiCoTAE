// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import "../common"

Popup {
    id: popup
    closePolicy: Popup.CloseOnPressOutside
    property string titleText: "Select Section"
    property bool isOK: false
    property string selectedSection: "Servicio"
    Material.elevation: 8

    x: appWindow.width - width
    width: Math.min(120, appWindow.height) / 3 * 2
    height: appWindow.height - 64
    transformOrigin: Popup.TopRight

    // Hint: as of Qt 5.9.x Beta because of bug in dark theme,
    // background is too dark
    // https://bugreports.qt.io/browse/QTBUG-53266

    ListView {
        id: listView
        // Attention: clip should be used carefully,
        // but using a ListView inside a Popup
        // you must set it to true
        // otherwise content will appear outside while scrolling
        // don't clip at Paopup: will cut the elevation shadow
        clip: true

        currentIndex: -1
        anchors.fill: parent
        implicitHeight: popup.height
        delegate: Item {
            id: itemDelegate
            width: parent.width
            implicitHeight: 40
            Row {
                spacing: 0
                LabelTitle {
                    anchors.verticalCenter: parent.verticalCenter
                    leftPadding: 24
                    text: model.section
                    font.bold: true
                    color: primaryColor
                }
            } // end Row
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    popup.selectedSection = model.section
                    popup.isOK = true
                    popup.close()
                }
            } // mouse
        } // end delegateItem

        model: ListModel {
            ListElement { section: "A" }
            ListElement { section: "B" }
            ListElement { section: "C" }
            ListElement { section: "D" }
            ListElement { section: "E" }
            ListElement { section: "F" }
            ListElement { section: "G" }
            ListElement { section: "H" }
            ListElement { section: "I" }
            ListElement { section: "J" }
            ListElement { section: "K" }
            ListElement { section: "L" }
            ListElement { section: "M" }
            ListElement { section: "N" }
            ListElement { section: "O" }
            ListElement { section: "P" }
            ListElement { section: "Q" }
            ListElement { section: "R" }
            ListElement { section: "S" }
            ListElement { section: "T" }
            ListElement { section: "U" }
            ListElement { section: "V" }
            ListElement { section: "W" }
            ListElement { section: "X" }
            ListElement { section: "Y" }
            ListElement { section: "Z" }
        }
        ScrollIndicator.vertical: ScrollIndicator { }
    } // end listView
} // end popup
