// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Pane {
    id: searchPane
    property alias searchTextField: theSearchTextField

    // don't use text ! won't work on Android or iOS but on OSX
    // displayText is always working
    property alias currentSearchText: theSearchTextField.displayText

    topPadding: 0
    z: 1
    Material.elevation: 12
    width: appWindow.width * 2/3
    height: 54
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottomMargin: 10
    anchors.bottom: parent.bottom
    RowLayout {
        width: parent.width - 30
        TextField {
            id: theSearchTextField
            selectByMouse: true
            Layout.fillWidth: true
            Layout.topMargin: 6
            Layout.leftMargin: 6
            placeholderText: qsTr("Search")
            // Keys.onReturnPressed: not used here
        }
    } // row
    ButtonIconActive {
        visible: theSearchTextField.displayText.length > 0
        focusPolicy: Qt.ClickFocus
        anchors.right: parent.right
        imageName: "clear.png"
        onClicked: {
            theSearchTextField.text = ""
            theSearchTextField.forceActiveFocus()
        }
    }
} // searchPane
