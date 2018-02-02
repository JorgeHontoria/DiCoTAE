// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0

import "../common"
import "../navigation"

Page {
    id: creatorDetailPage
    focus: true
    property string name: "CreatorDetailPage"

    property Creator creator
    property int creatorId: -2
    onCreatorIdChanged: {
        if(creatorId > 0) {
            creator = dataManager.findCreatorByCreatorId(creatorId)
            creatorImage.creator = creator
            // already resolved for the list
            // dataManager.resolveOrderReferences(order)
            // customer = order.customerAsDataObject
        }
    }

    Flickable {
        id: flickable
        property string name: "creatorDetail"
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
                    Layout.rightMargin: 6
                    Layout.bottomMargin: 12
                    CreatorImageItem {
                        id: creatorImage
                        anchors.top: parent.top
                    }
                    ColumnLayout {
                        LabelHeadline {
                            leftPadding: 10+6
                            text: creator.name.length? creator.name : qsTr("Unnamed Creator")
                            color: accentColor
                        }

                        LabelSubheading {
                            leftPadding: 10+6
                            rightPadding: 10
                            wrapMode: Text.WordWrap
                            text: creator.bio
                        }
                    }
                } // row
                /*LabelBodySecondary {
                    text: "id "+creator.creatorId
                    font.italic: true
                    transform: Translate{y: -12}
                }*/
                HorizontalDivider{
                    height: 3
                    transform: Translate{y: -8}
                }
                /*RowLayout {
                    visible: creator.keysPropertyList.length
                    Layout.leftMargin: 16
                    IconActive {
                        anchors.verticalCenter: parent.verticalCenter
                        //transform: Translate { x: -36 }
                        imageSize: 36
                        imageName: "dictionary.png"
                    } // dictionaryItemImage
                    LabelHeadline {
                        leftPadding: 10
                        text: qsTr("Keys")
                        color: primaryColor
                    }
                }
                LabelBodySecondary {
                    visible: creator.keysPropertyList.length
                    leftPadding: 16
                    font.italic: true
                    text: qsTr("Tap on the Talk Icon or Menu Button to get the Details.\nTap on the Star Icon to add to / remove from your personal dictionary.")
                    wrapMode: Text.WordWrap
                }
                HorizontalListDivider{
                    visible: creator.keysPropertyList.length
                }
                // S E S S I O N    Repeater
                Repeater {
                    id: keyRepeater
                    model: creator.keysPropertyList
                    property int keyRepeaterIndex: index
                    Pane {
                        id: creatorKeyPane
                        property Key theCreatorKey: model.modelData
                        topPadding: 4
                        leftPadding: 0
                        rightPadding: 0
                        Layout.fillWidth: true

                        // dirty hack with some Buttons to get click events
                        // without loosing click from Menu or Favorites
                        ColumnLayout {
                            id: keyRow
                            Layout.fillWidth: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            RowLayout {
                                // base row
                                Layout.leftMargin: 16
                                Layout.rightMargin: 6
                                Layout.bottomMargin: 2
                                ColumnLayout {
                                    // repeater left column
                                    Layout.maximumWidth: creatorImage.width
                                    Layout.minimumWidth: creatorImage.width
                                    Layout.rightMargin: 6
                                    anchors.top: parent.top
                                    CharCircle {
                                        Layout.leftMargin: 14
                                        size: 36
                                        text: dataUtil.sectionForButton(modelData)
                                    }
                                    LabelBody {
                                        text: modelData.minutes + qsTr(" Minutes")
                                    }
                                    ListRowButton {
                                        onClicked: {
                                            navPane.pushKeyDetail(modelData.keyId)
                                        }
                                    }
                                } // repeater left column

                                ColumnLayout {
                                    // repeater right column
                                    Layout.fillWidth: true
                                    Layout.leftMargin: 10
                                    Layout.rightMargin: 10
                                    RowLayout {
                                        // repeater date row
                                        IconActive{
                                            imageSize: 18
                                            imageName: "calendar.png"
                                        }
                                        LabelBody {
                                            text: modelData.keyDayAsDataObject.repositoryDay.toLocaleDateString()
                                            wrapMode: Text.WordWrap
                                        }
                                        IconActive {
                                            //id: menuIcon
                                            imageSize: 24
                                            imageName: "more_vert.png"
                                            anchors.right: parent.right
                                            // anchors.top: parent.top
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    optionsMenu.open()
                                                }
                                            } // mouse area
                                            Menu {
                                                id: optionsMenu
                                                modal:true
                                                dim: false
                                                closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
                                                x: parent.width - width
                                                transformOrigin: Menu.TopRight
                                                MenuItem {
                                                    text: qsTr("Key Details")
                                                    onTriggered: {
                                                        navPane.pushKeyDetail(modelData.keyId)
                                                    }
                                                }
                                                MenuItem {
                                                    text: qsTr("Section Info")
                                                    visible: modelData.sectionAsDataObject.inAssets
                                                    onTriggered: {
                                                        navPane.pushSectionDetail(modelData.sectionAsDataObject.sectionId)
                                                    }
                                                }
                                                onAboutToShow: {
                                                    appWindow.modalMenuOpen = true
                                                }
                                                onAboutToHide: {
                                                    appWindow.modalMenuOpen = false
                                                    appWindow.resetFocus()
                                                }
                                            } // end optionsMenu
                                        } // menuIcon
                                    } // // repeater date row
                                    RowLayout {
                                        // repeater time and section besides favorite button
                                        ColumnLayout {
                                            // time section column
                                            Layout.fillWidth: true
                                            RowLayout {
                                                // repeater time row
                                                IconActive{
                                                    imageSize: 18
                                                    imageName: "time.png"
                                                }
                                                LabelBody {
                                                    text: modelData.startTime.toLocaleTimeString("HH:mm") + " - " + modelData.endTime.toLocaleTimeString("HH:mm")
                                                }
                                            } // repeater time row
                                            RowLayout {
                                                // repeater section row
                                                IconActive{
                                                    imageSize: 18
                                                    imageName: "directions.png"
                                                }
                                                LabelBody {
                                                    text: modelData.sectionAsDataObject.sectionName
                                                }
                                            } // repeater section row
                                        } // // time section column
                                        IconActive {
                                            transform: Translate { x: 6; y: 8 }
                                            imageSize: 36
                                            imageName: "stars.png"
                                            opacity: modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                                            anchors.right: parent.right
                                            anchors.top: parent.top
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    modelData.isFavorite = !modelData.isFavorite
                                                    if(modelData.isFavorite) {
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
                                    } // repeater time and section besides favorite button

                                    // INNER REPEATER for Tags
                                    Repeater {
                                        id: innerTagRepeater
                                        model: creatorKeyPane.theCreatorKey.keyTagsPropertyList
                                        RowLayout {
                                            // repeater tag row
                                            //visible: tagLabel.text.length
                                            IconActive{
                                                visible: index == 0
                                                imageSize: 18
                                                imageName: "tag.png"
                                            }
                                            Item {
                                                visible: index > 0
                                                width: 18
                                            }
                                            Rectangle {
                                                width: 16
                                                height: 16
                                                color: dataUtil.tagColor(modelData.tagId)
                                                radius: width / 2
                                            }
                                            LabelBody {
                                                id: tagLabel
                                                text: dataUtil.textForKeyTag(modelData)
                                                rightPadding: 16
                                                wrapMode: Text.WordWrap
                                            }
                                        } // repeater tag row

                                    } // innerTagRepeater

                                    LabelSubheading {
                                        text: modelData.title
                                        font.bold: true
                                        wrapMode: Label.WordWrap
                                        ListRowButton {
                                            onClicked: {
                                                navPane.pushKeyDetail(modelData.keyId)
                                            }
                                        }
                                    } // title
                                    LabelBody {
                                        visible: modelData.subtitle.length
                                        text: modelData.subtitle
                                        wrapMode: Label.WordWrap
                                        ListRowButton {
                                            onClicked: {
                                                navPane.pushKeyDetail(modelData.keyId)
                                            }
                                        }
                                    } // subtitle
                                    RowLayout {
                                        id: creatorRow
                                        visible: modelData.creatorPropertyList.length > 1
                                        property var creator: modelData.creatorPropertyList
                                        // creators row
                                        IconActive{
                                            Layout.alignment: Qt.AlignTop
                                            imageSize: 18
                                            imageName: "creator.png"
                                        }
                                        ColumnLayout {
                                            Repeater {
                                                id: creatorRepeater
                                                model: creatorRow.creator
                                                LabelBody {
                                                    text: modelData.name
                                                }
                                            } // creatorRepeater
                                        } // creator Column
                                    } // repeater tag row

                                } // // repeater right column
                            } // repeater base row
                            HorizontalListDivider{}
                        } // repeater keyRow
                    } // key pane
                } // key repeater*/
            } // col layout
        } // root

        ScrollIndicator.vertical: ScrollIndicator { }
    } // flickable
    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from CreatorDetailPage")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from CreatorDetailPage")
    }

} // creatorDetailPage
