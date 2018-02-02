// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0

import "../common"

Page {
    id: sectionDetailPage
    focus: true
    property string name: "SectionDetailPage"

    property Section section
    property int sectionId: -2
    onSectionIdChanged: {
        if(sectionId > 0) {
            section = dataManager.findSectionBySectionId(sectionId)
            // dont forget to resolved if access keys or category/dictionary
        }
    }

    Flickable {
        id: flickable
        property string name: "sectionDetail"
        // need some extra space if scrolling to bottom
        // and nothing covered by the FAB
        contentHeight: sectionImage.sourceSize.height * sectionImage.scale + 60
        contentWidth: sectionImage.sourceSize.width * sectionImage.scale
        anchors.fill: parent

            Image {
                id: sectionImage
                anchors.top: parent.top
                anchors.left: parent.left
                width: sourceSize.width
                height: sourceSize.height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/data-assets/repository/categoryplan/section_"+section.sectionId+".png"
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop
                transformOrigin: Item.TopLeft
                transform: Translate { y: 6; x:6 }
            } // image


    } // flickable

    FloatingActionMiniButton {
        visible: sectionImage.scale >= 0.2
        property string imageName: "/remove.png"
        z: 1
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        showShadow: true
        onClicked: {
            sectionImage.scale = sectionImage.scale - 0.2
        }
    } // FAB
    FloatingActionMiniButton {
        property string imageName: "/aspect_ratio.png"
        z: 1
        anchors.leftMargin: 80
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        showShadow: true
        onClicked: {
            fitIntoWindow()
        }
    } // FAB
    FloatingActionMiniButton {
        visible: sectionImage.scale <= 1.0
        property string imageName: "/add.png"
        z: 1
        anchors.leftMargin: 140
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        showShadow: true
        onClicked: {
            sectionImage.scale = sectionImage.scale + 0.2
        }
    } // FAB

    function fitIntoWindow() {
        var widthScale = (appWindow.width-20) / sectionImage.sourceSize.width
        var heightScale = (appWindow.height-20) / sectionImage.sourceSize.height
        sectionImage.scale = Math.min(widthScale, heightScale)
        flickable.contentX = 0
        flickable.contentY = 0
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        fitIntoWindow()
        console.log("Init done from SectionDetailPage")
        appWindow.setDefaultTitleBarInLandscape()
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from SectionDetailPage")
        appWindow.resetDefaultTitleBarInLandscape()
    }

} // sectionDetailPage
