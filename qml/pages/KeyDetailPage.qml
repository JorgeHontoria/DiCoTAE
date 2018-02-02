// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

import Clipboard 1.0

import org.msolanogadea.data 1.0

import "../common"
import "../navigation"

Page {
    id: keyDetailPage
    focus: true
    property string name: "KeyDetailPage"

    property Key key
    property bool isDictionaryItem: false
    property int keyId: -2
    onKeyIdChanged: {
        if(keyId > 0) {
            key = dataManager.findKeyByKeyId(keyId)
            // already resolved for the list
            // dataManager.resolveOrderReferences(order)
            // customer = order.customerAsDataObject
            isDictionaryItem = key.hasDictionaryItem()
        }
    }

    Clipboard {
        id: clipboard

        onDataChanged: console.log("Clipboard data changed");
        onSelectionChanged: console.log("Clipboard selection changed");
    }

    Flickable {
        id: flickable
        property string name: "keyDetail"
        // need some extra space if scrolling to bottom
        // and nothing covered by the FAB
        contentHeight: root.implicitHeight + 60
        anchors.fill: parent

        Pane {
            id: root
            anchors.fill: parent

            ColumnLayout {
                Layout.fillWidth: true
                anchors.right: parent.right
                anchors.left: parent.left
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16 + 32
                    Layout.bottomMargin: 4
                    LabelTitle {
                        text: key.title
                        wrapMode: Text.WordWrap
                    }
                }
                /*LabelSubheading {
                    visible: key.subtitle.length
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    text: key.subtitle
                    wrapMode: Text.WordWrap
                }*/
                LabelSubheading {
                    id: descriptionText
                    visible: key.description.length
                    Layout.topMargin: 12
                    Layout.bottomMargin: 12
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    text: key.description
                    wrapMode: Text.WordWrap

                    /*Layout.fillWidth: true
                    font.pixelSize: fontSizeSubheading
                    opacity: opacitySubheading
                    readOnly: true
                    selectByMouse: true*/
                }
                IconActive {
                    visible: !isDictionaryItem
                    imageSize: 36
                    imageName: "stars.png"
                    opacity: key.isFavorite? opacityToggleActive : opacityToggleInactive
                    anchors.right: parent.right
                    anchors.top: parent.top
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // Favorito
                            key.isFavorite = !key.isFavorite
                            if(key.isFavorite) {
                                appWindow.showToast(qsTr("Added to Personal Dictionary"))
                            } else {
                                appWindow.showToast(qsTr("Removed from Personal Dictionary"))
                            }
                            if(appWindow.myDictionaryActive) {
                                dataUtil.refreshMyDictionary()
                            }
                        }
                    }
                } // favoritesIcon

                HorizontalDivider {
                    id: h1Div
                    IconActive {
                        visible: !isDictionaryItem
                        imageSize: 36
                        imageName: "form.png"
                        opacity: opacityToggleInactive
                        anchors.right: h1Div.right
                        anchors.top: h1Div.bottom
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                // Preparamos el texto a enviar al portapapeles
                                var strText = key.title + ": " + key.description + "\n";
                                strText += "\n";
                                if (key.keyTagsPropertyList[0] !== undefined) {
                                    strText += qsTr("Tags") + ": \n";
                                    var i = 0;
                                    for (var keyTag in key.keyTagsPropertyList) {
                                        strText += "» " + key.keyTagsPropertyList[i].name + "\n";
                                        i++;
                                    }
                                }
                                if (key.keyContentBlocksPropertyList[0] !== undefined) {
                                    strText += qsTr("ContentBlocks") + ": \n";
                                    i = 0;
                                    for (var keyContentBlock in key.keyContentBlocksPropertyList) {
                                        strText += "» " + key.keyContentBlocksPropertyList[i].name + "\n";
                                        i++;
                                    }
                                }
                                if (key.keyContentStandardTypesPropertyList[0] !== undefined) {
                                    strText += qsTr("ContentStandardTypes") + ": \n";
                                    i = 0;
                                    for (var keyContentStandardTypes in key.keyContentStandardTypesPropertyList) {
                                        strText += "» " + key.keyContentStandardTypesPropertyList[i].name + "\n";
                                        i++;
                                    }
                                }
                                if (key.contentStandardName.length > 0)
                                    strText += qsTr("contentStandardName") + ": \n» " + key.contentStandardName + "\n";
                                if (key.contentSituacion1.length > 0)
                                    strText += qsTr("contentSituacion1") + ": \n» " + key.contentSituacion1 + "\n";
                                if (key.contentLink.length > 0)
                                    strText += qsTr("contentLink") + ": \n» " + key.contentLink + "\n";

                                // Enviamos al portapapeles
                                clipboard.text = strText;

                                // Notificamos
                                appWindow.showToast(qsTr("Text has been copied to clipboard"))
                            }
                        }
                    } // clipboardIcon
                }

                /*RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    ButtonOneCharUncolored {
                        visible: !isDictionaryItem
                        anchors.verticalCenter: parent.verticalCenter
                        text: dataUtil.sectionForButton(key)
                    } // button one char
                    IconActive {
                        visible: isDictionaryItem
                        anchors.verticalCenter: parent.verticalCenter
                        //transform: Translate { x: -36 }
                        imageSize: 24
                        imageName: isDictionaryItem? dataUtil.dictionaryItemImageForKey(key) : ""
                    } // dictionaryItemImage
                    LabelSubheading {
                        Layout.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        text: dataUtil.textForKeyType(key)
                        wrapMode: Text.WordWrap
                    }
                    IconActive {
                        visible: !isDictionaryItem
                        imageSize: 36
                        imageName: "stars.png"
                        opacity: key.isFavorite? opacityToggleActive : opacityToggleInactive
                        anchors.right: parent.right
                        anchors.top: parent.top
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                key.isFavorite = !key.isFavorite
                                if(key.isFavorite) {
                                    appWindow.showToast(qsTr("Added to Personal Dictionary"))
                                } else {
                                    appWindow.showToast(qsTr("Removed from Personal Dictionary"))
                                }
                                if(appWindow.myDictionaryActive) {
                                    dataUtil.refreshMyDictionary()
                                }
                            }
                        }
                    } // favoritesIcon
                }*/

                // TAG
                RowLayout {
                    visible: (key.keyTagsPropertyList[0] !== undefined)
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive {
                        imageSize: 24
                        imageName: "tag.png"
                    }
                    LabelSubheading {
                        leftPadding: 16
                        rightPadding: 16
                        text: qsTr("Tags") + ":"
                        color: accentColor
                        wrapMode: Text.WordWrap
                    }
                }

                // TAG   REPEATER
                Repeater {
                    model: key.keyTagsPropertyList
                    RowLayout {
                        visible: tagLabel.text.length > 0
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        Rectangle {
                            Layout.leftMargin: 24+32
                            width: 16
                            height: 16
                            color: dataUtil.tagColor(model.modelData.tagId)
                            radius: width / 2
                        }
                        LabelSubheading {
                            id: tagLabel
                            rightPadding: 16
                            text: dataUtil.textForKeyTag(model.modelData)
                            wrapMode: Text.WordWrap
                        }
                    } // tag row
                } // tag repeater


                // CONTENTBLOCK
                RowLayout {
                    visible: (key.keyContentBlocksPropertyList[0] !== undefined)
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive {
                        imageSize: 24
                        imageName: "folder.png"
                    }
                    LabelSubheading {
                        leftPadding: 16
                        rightPadding: 16
                        text: qsTr("Block") + ":"
                        color: accentColor
                        wrapMode: Text.WordWrap
                    }
                }

                // CONTENTBLOCK   REPEATER
                Repeater {
                    id: keyContentBlocksRepeater
                    model: key.keyContentBlocksPropertyList
                    RowLayout {
                        visible: contentBlockLabel.text.length > 0
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        LabelSubheading {
                            id: contentBlockLabel
                            leftPadding: 24+32
                            rightPadding: 16
                            text: dataUtil.textForKeyContentBlock(model.modelData)
                            wrapMode: Text.WordWrap
                        }
                    } // contentBlock row
                } // contentBlock repeater

                // CONTENTSTANDARDTYPE
                RowLayout {
                    visible: (key.keyContentStandardTypesPropertyList[0] !== undefined)
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive {
                        imageSize: 24
                        imageName: "folder_fav.png"
                    }
                    LabelSubheading {
                        leftPadding: 16
                        rightPadding: 16
                        text: qsTr("Standard Type") + ":"
                        color: accentColor
                        wrapMode: Text.WordWrap
                    }
                }

                // CONTENTSTANDARDTYPE   REPEATER
                Repeater {
                    model: key.keyContentStandardTypesPropertyList
                    RowLayout {
                        visible: contentStandardTypeLabel.text.length > 0
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16

                        LabelSubheading {
                            id: contentStandardTypeLabel
                            leftPadding: 24+32
                            rightPadding: 16
                            text: dataUtil.textForKeyContentStandardType(model.modelData)
                            wrapMode: Text.WordWrap
                        }
                    } // contentStandardType row
                } // contentStandardType repeater


                // CONTENTSTANDARDNAME
                RowLayout {
                    visible: (key.contentStandardName.length > 0)
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive {
                        imageSize: 24
                        imageName: "code.png"
                    }
                    LabelSubheading {
                        leftPadding: 16
                        rightPadding: 16
                        text: qsTr("Standard Name") + ":"
                        color: accentColor
                        wrapMode: Text.WordWrap
                    }
                }

                RowLayout {
                    visible: (key.contentStandardName.length > 0)
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    LabelSubheading {
                        leftPadding: 24+32
                        rightPadding: 16
                        text: key.contentStandardName + (key.contentArticulo1.length > 0 ? " " + key.contentArticulo1 : "")
                    }
                }

                // CONTENTSITUACION1
                RowLayout {
                    visible: (key.contentSituacion1.length > 0)
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "history.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: key.contentSituacion1
                    }
                }

                // CONTENTLINK
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "info.png"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                var strText = "";
                                if (key.contentLink.length > 0)
                                    strText += key.contentLink;

                                // Enviamos al portapapeles
                                clipboard.text = strText;

                                // Notificamos
                                appWindow.showToast(qsTr("Link has been copied to clipboard"))
                            }
                        }
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: "<a href=\"" + key.contentLink + "\">" + qsTr("View content source") + "...</a>"
                        onLinkActivated: Qt.openUrlExternally(key.contentLink)
                    }
                }

                /*RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "calendar.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: key.keyDayAsDataObject.repositoryDay.toLocaleDateString()
                    }
                }*/

                /*RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    IconActive{
                        imageSize: 24
                        imageName: "time.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: key.startTime.toLocaleTimeString("HH:mm") + " - " + key.endTime.toLocaleTimeString("HH:mm")
                    }
                }*/

                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.bottomMargin: 12

                    /*//transform: Translate{y: -10}
                    IconActive{
                        imageSize: 24
                        imageName: "directions.png"
                    }
                    LabelSubheading {
                        Layout.leftMargin: 16
                        text: key.sectionAsDataObject.sectionName
                    }
                    FloatingActionMiniButton {
                        z: 1
                        visible: key.sectionAsDataObject.inAssets
                        transform: Translate{y: -6}
                        showShadow: true
                        imageSource: "qrc:/images/"+iconOnAccentFolder+"/directions.png"
                        backgroundColor: accentColor
                        anchors.right: parent.right
                        onClicked: {
                            navPane.pushSectionDetail(key.sectionAsDataObject.sectionId)
                        }
                    } // favoritesIcon*/
                }

                /*LabelBodySecondary {
                    text: "id "+key.keyId
                    font.italic: true
                    transform: Translate{y: -8}
                }*/
                HorizontalDivider{
                    height: 3
                    transform: Translate{y: -8}
                }
                /*RowLayout {
                    visible: key.creatorPropertyList.length
                    Layout.leftMargin: 16
                    IconActive {
                        anchors.verticalCenter: parent.verticalCenter
                        //transform: Translate { x: -36 }
                        imageSize: 36
                        imageName: "creator.png"
                    } // creator image
                    LabelHeadline {
                        leftPadding: 10
                        text: qsTr("Creator")
                        color: primaryColor
                    }
                }
                LabelBodySecondary {
                    visible: key.creatorPropertyList.length
                    leftPadding: 16
                    font.italic: true
                    text: qsTr("Tap on the Creator Data to get the Details.")
                    wrapMode: Text.WordWrap
                }
                HorizontalListDivider{
                    visible: key.creatorPropertyList.length
                }*/

                // C R E A T O R    Repeater
                /*Repeater {
                    model: key.creatorPropertyList

                    Pane {
                        padding: 0
                        Layout.fillWidth: true

                        ColumnLayout {
                            id: creatorRow
                            // without this divider not over total width
                            implicitWidth: appWindow.width
                            RowLayout {
                                spacing: 20
                                Layout.leftMargin: 16
                                Layout.rightMargin: 6
                                CreatorImageItem {
                                    creator: model.modelData
                                }
                                ColumnLayout {
                                    Layout.fillWidth: true
                                    // without setting a maximum width, word wrap not working
                                    Layout.maximumWidth: appWindow.width-120
                                    spacing: 0
                                    LabelSubheading {
                                        rightPadding: 12
                                        text: model.modelData.name.length? model.modelData.name : qsTr("Unnamed Creator")
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                    } // label
                                    LabelBody {
                                        text: dataUtil.keyInfoForCreator(model.modelData)
                                        rightPadding: 12
                                        wrapMode: Label.WordWrap
                                        maximumLineCount: 3
                                        elide: Label.ElideRight
                                    }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        navPane.pushCreatorDetail(model.modelData.creatorId)
                                    }
                                } // mouse
                            } // end Row Layout
                            HorizontalListDivider{}
                        } // end Col Layout creator row
                    }// creator Pane
                } // creator repeater*/
            } // main col layout
        }// root pane
        ScrollIndicator.vertical: ScrollIndicator { }
    } // flickable

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from KeyDetailPage")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from KeyDetailPage")
    }

} // page
