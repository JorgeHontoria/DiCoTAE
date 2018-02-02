// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import "../common"

RowLayout {
    id: helpRow
    Layout.bottomMargin: 6
    property string helpText: ""
    property alias color: labelSub.color
    property alias contentPadding: labelMargin.width

    Item {
        id: labelMargin
        anchors.top: parent.top
        width: 24 + 24
        height: 24
    } // image
    LabelSubheading {
        id: labelSub
        topPadding: 6
        leftPadding: 10
        rightPadding: 10
        wrapMode: Text.WordWrap
        text: helpRow.helpText
    }
} // helpRow
