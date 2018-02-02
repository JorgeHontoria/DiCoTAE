// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0

import "../common"

Item {
    property Creator creator
    property string assetsSource: "qrc:/data-assets/repository/creatorImages/creator_"
    property string dataSource: dataUtil.repositoryDataPath4QML()+"creatorImages/creator_"
    id: imageItem
    height: 64
    width: 64
    Image {
        visible: creator && creator.hasCreatorImage()
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.Pad
        source: creator.hasCreatorImage()? creator.creatorImageAsDataObject.inAssets? assetsSource+creator.creatorId+"."+creator.creatorImageAsDataObject.suffix
                                           : creator.creatorImageAsDataObject.inData? dataSource+creator.creatorId+"."+creator.creatorImageAsDataObject.suffix : "" : ""
    } // image
    IconInactive {
        visible: creator && !creator.hasCreatorImage()
        imageSize: 48
        imageName: "person.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.Pad
    }
} // icon item
