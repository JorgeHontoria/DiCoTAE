// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

Item {
    id: charCircle
    property alias text: theLabel.text
    property alias color: theLabel.color
    property alias colorCircle: charRectangle.color

    property int size: 24
    width: size
    height: size
    Rectangle {
        id: charRectangle
        width: charCircle.size
        height: charCircle.size
        color: isDarkTheme? "lightgrey" : "darkgrey"
        radius: width / 2
        Label {
            id: theLabel
            anchors.centerIn: charRectangle
            text: "?"
            font.pixelSize: charCircle.size*3/4
            color: isDarkTheme? "black" : "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}


