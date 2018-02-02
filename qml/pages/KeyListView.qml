// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

ListView {
    id: listView
    focus: true
    clip: true
    // highlight: Rectangle {color: Material.listHighlightColor }
    currentIndex: -1
    anchors.fill: parent
    // setting the margin to be able to scroll the list above the FAB to use the Switch on last row
    bottomMargin: 40
    // QList<Key*>
    // model: dataManager.keyPropertyList
    // or
    // keyLists.dictionarydKeysPropertyList

    // SECTION HEADER
    Component {
        id: sectionHeading
        ColumnLayout {
            width: parent.width
            LabelTitle {
                topPadding: 6
                bottomPadding: 6
                leftPadding: 16
                text: section
                anchors.verticalCenter: parent.verticalCenter
                color: primaryColor
                font.bold: true
            }
            HorizontalListDivider{}
        } // col layout
    }

    delegate:
        Loader {
        id: keyLoader
        // define Components inside Loader to enable direct access to ListView functions and modelData
        sourceComponent: hasDictionaryItem()? dictionaryRowComponent : keyRowComponent

        // LIST ROW DELEGATES
        Component {
            id: dictionaryRowComponent

            ColumnLayout {
                id: dictionaryRow
                // without this divider not over total width
                implicitWidth: appWindow.width
                RowLayout {
                    spacing: 20
                    Layout.leftMargin: 20
                    Layout.rightMargin: 6
                    Layout.topMargin: 6
                    ColumnLayout {
                        IconActive {
                            //transform: Translate { x: -36 }
                            imageSize: 36
                            imageName: dataUtil.dictionaryItemImageForKey(model.modelData)
                            // opacity: model.modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                        } // dictionaryItemImage
                    } // left column
                    ColumnLayout {
                        Layout.fillWidth: true
                        // without setting a maximum width, word wrap not working
                        Layout.maximumWidth: appWindow.width-132
                        Layout.minimumWidth: appWindow.width-132
                        spacing: 0
                        LabelTitle {
                            rightPadding: 12
                            text: model.modelData.title
                            //font.bold: true
                            wrapMode: Label.WordWrap
                            maximumLineCount: 2
                            elide: Label.ElideRight
                        } // label
                        LabelSubheading {
                            rightPadding: 12
                            bottomPadding: 6
                            font.italic: true
                            text: model.modelData.startTime.toLocaleTimeString("HH:mm") + " - " + model.modelData.endTime.toLocaleTimeString("HH:mm")
                        }
                    } // middle column
                    ListRowButton {
                        onClicked: {
                            if (model.modelData.isRefView === true) {
                                // Tipo Ver
                                navPane.pushKeyDetail(model.modelData.keyIdRefView)
                            } else {
                                // Tipo Termino
                                navPane.pushKeyDetail(model.modelData.keyId)
                            }
                        }
                    }
                }
            } // dictionaryRow

        } // dictionaryRowComponent

        Component {
            id: keyRowComponent
            ItemDelegate {
                id: theItem
                height: keyRow.height
                implicitWidth: appWindow.width
                Rectangle {
                    anchors.top: theItem.top
                    height: keyRow.height-2
                    width: 8
                    color: model.modelData.isBoolean3? "#B2DFDB" : dataUtil.tagColorFirstTag(model.modelData)
                }
                onClicked: {
                    if (model.modelData.isRefView === true) {
                        // Tipo Ver
                        navPane.pushKeyDetail(model.modelData.keyIdRefView)
                    } else {
                        // Tipo Termino
                        navPane.pushKeyDetail(model.modelData.keyId)
                    }
                }

                ColumnLayout {
                    id: keyRow
                    // without this divider not over total width
                    implicitWidth: appWindow.width
                    RowLayout {
                        spacing: 20
                        Layout.leftMargin: (model.modelData.isRefView === true) ? (16+24) : (16+12)
                        Layout.rightMargin: 6
                        Layout.topMargin: 6
                        ColumnLayout {
                            //visible: (model.modelData.isRefView === false)
                            Layout.leftMargin: 0
                            CharCircle {
                                size: (model.modelData.isRefView === true) ? 12 : 24
                                text: dataUtil.sectionForButton(model.modelData)
                                colorCircle: text === "T" ? "#E0ECF8" : "#ECF6CE"
                            }
                        } // left column
                        ColumnLayout {
                            Layout.fillWidth: true
                            // without setting a maximum width, word wrap not working
                            Layout.maximumWidth: appWindow.width-132
                            Layout.minimumWidth: appWindow.width-132
                            spacing: 0
                            LabelSubheading {
                                leftPadding: 0//(model.modelData.isRefView === true) ? 58 : 0
                                rightPadding: 12//(model.modelData.isRefView === false) ? 12 : -84
                                text: (model.modelData.isRefView === false) ? model.modelData.title : ("<b>" + model.modelData.title + "</b> » " + model.modelData.subtitle)
                                font.pixelSize: (model.modelData.isRefView === true) ? fontSizeCaption : fontSizeSubheading
                                font.bold: (model.modelData.isRefView === true) ? false : true
                                wrapMode: Label.WordWrap
                                maximumLineCount: 6
                                elide: Label.ElideRight
                            } // label
                            /*LabelBody {
                                visible: ((model.modelData.isRefView === false) && (model.modelData.subtitle.length))
                                leftPadding: (model.modelData.isRefView === true) ? 24 : 0
                                rightPadding: 12
                                text: model.modelData.subtitle
                                wrapMode: Label.WordWrap
                                maximumLineCount: (model.modelData.isRefView === true) ? 1 : 3
                                elide: Label.ElideRight
                                font.italic: (model.modelData.isRefView === true) ? true : false
                                font.pixelSize: (model.modelData.isRefView === true) ? fontSizeCaption : fontSizeSubheading
                            }*/
                            LabelBody {
                                visible: ((model.modelData.isRefView === false) && (model.modelData.description.length))
                                rightPadding: 12
                                text: model.modelData.description
                                wrapMode: Label.WordWrap
                                maximumLineCount: (model.modelData.isRefView === true) ? 1 : 3
                                elide: Label.ElideRight
                            }
                            /*RowLayout {
                                LabelBody {
                                    Layout.fillWidth: false
                                    text: model.modelData.startTime.toLocaleTimeString("HH:mm") + " - " + model.modelData.endTime.toLocaleTimeString("HH:mm") + ","
                                }
                                IconActive{
                                    imageSize: 18
                                    imageName: "directions.png"
                                }
                                LabelBody {
                                    Layout.fillWidth: false
                                    text: model.modelData.sectionAsDataObject.sectionName
                                }
                            }*/
                            /*RowLayout {
                                visible: creatorNamesLabel.text.length
                                IconActive{
                                    imageSize: 18
                                    imageName: "creator.png"
                                }
                                LabelBody {
                                    id: creatorNamesLabel
                                    font.italic: true
                                    text: dataUtil.creatorNamesForKey(model.modelData)
                                    elide: Label.ElideRight
                                }
                            }*/
                        } // middle column
                        ColumnLayout {
                            visible: (model.modelData.isRefView === false)
                            Layout.rightMargin: 16
                            IconActive {
                                transform: Translate { x: -6 }
                                imageSize: 36
                                imageName: "stars.png"
                                opacity: model.modelData.isFavorite? opacityToggleActive : opacityToggleInactive
                                ListRowButton {
                                    onClicked: {
                                        model.modelData.isFavorite = !model.modelData.isFavorite
                                        if(model.modelData.isFavorite) {
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
                        } // right column
                    } // end Row Layout
                    HorizontalListDivider{}
                } // end Col Layout creator row
            } // row item
        } // keyRowComponent
    } // keyLoader

    section.property: "sortKey"
    section.criteria: ViewSection.FullString
    section.delegate: sectionHeading

    ScrollIndicator.vertical: ScrollIndicator { }


} // end listView
