// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: sectionListPage
    focus: true
    property string name: "SectionListPage"
    bottomPadding: 6
    topPadding: 6

    // LIST VIEW
    ListView {
        id: listView
        focus: true
        clip: true
        // highlight: Rectangle {color: Material.listHighlightColor }
        currentIndex: -1
        anchors.fill: parent
        // setting the margin to be able to scroll the list above the FAB to use the Switch on last row
        // bottomMargin: 40
        // QList<Section*>
        //model: dataManager.sectionPropertyList

        // important: use Loader to avoid errors because of https://bugreports.qt.io/browse/QTBUG-49224
        delegate: Loader {
            id: sectionLoader
            // define Components inside Loader to enable direct access to ListView functions and modelData
            sourceComponent: keysPropertyList.length? sectionRowComponent :emptySectionComponent

            Component {
                id: emptySectionComponent
                Item {}
            }

            // LIST ROW DELEGTE
            Component {
                id: sectionRowComponent
                ColumnLayout {
                    id: sectionRow
                    visible: model.modelData.keysPropertyList.length
                    // without this divider not over total width
                    implicitWidth: appWindow.width
                    spacing: 0
                    RowLayout {
                        //implicitWidth: appWindow.width

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.leftMargin: 36
                            Layout.rightMargin: 12
                            Layout.topMargin: 12
                            Layout.bottomMargin: 12
                            // without setting a maximum width, word wrap not working
                            Layout.maximumWidth: appWindow.width-60
                            Layout.minimumWidth: appWindow.width-60

                            LabelHeadline {
                                text: model.modelData.sectionName
                                color: primaryColor
                                wrapMode: Label.WordWrap
                            } // label

                            LabelBody {
                                text: model.modelData.keysPropertyList.length + qsTr(" Keys")
                                wrapMode: Label.WordWrap
                                maximumLineCount: 2
                                elide: Label.ElideRight
                            }
                        }
                        ListRowButton {
                            onClicked: {
                                navPane.pushSectionKeys(model.modelData.sectionId)
                            }
                        }
                        /*ColumnLayout {
                            Layout.maximumWidth: 64
                            Layout.minimumWidth: 64
                            Item {
                                visible: model.modelData.inAssets
                                width: 64
                                height: 64
                                Image {
                                    id: sectionImage
                                    width: 64
                                    height: 64
                                    fillMode: Image.PreserveAspectFit
                                    source: model.modelData.inAssets? "qrc:/data-assets/repository/categoryplan/section_"+model.modelData.sectionId+".png" : ""
                                    horizontalAlignment: Image.AlignLeft
                                    verticalAlignment: Image.AlignTop
                                    transform: Translate {x: -16 }
                                } // image
                                ListRowButton {
                                    onClicked: {
                                        navPane.pushSectionDetail(model.modelData.sectionId)
                                    }
                                }
                            } // item
                        } // col section image*/
                    }

                    HorizontalListDivider{}
                } // end Col Layout creator row
            } // sectionRowComponent

        } // sectionLoader

        ScrollIndicator.vertical: ScrollIndicator { }

    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from SectionListPage")
        console.log("Tags # "+dataManager.sectionPropertyList.length)
        dataUtil.resolveKeysForSections()
        listView.model = dataManager.sectionPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from SectionListPage")
    }
} // end SectionListPage
