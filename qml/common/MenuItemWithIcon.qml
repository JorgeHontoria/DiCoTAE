// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

MenuItem {
    id: menuItem
    property alias imageName: theIcon.imageName
    property alias itemText: theLabel.text
    focusPolicy: Qt.NoFocus
    contentItem:
        Row {
        topPadding: 0
        bottomPadding: 0
        spacing: 12
        IconInactive {
            id: theIcon
        }
        LabelSubheading {
            id: theLabel
            text: ""
            anchors.verticalCenter: parent.verticalCenter
        }
    } // row
}
