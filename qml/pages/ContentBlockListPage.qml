// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: contentBlockListPage
    focus: true
    property string name: "contentBlockListPage"
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
        // QList<KeyContentBlock*>
        //model: dataManager.keyContentBlockPropertyList

        // important: use Loader to avoid errors because of https://bugreports.qt.io/browse/QTBUG-49224
        delegate: Loader {
            id: keyContentBlockLoader
            // define Components inside Loader to enable direct access to ListView functions and modelData
            sourceComponent: keysPropertyList.length? contentBlockRowComponent :emptyContentBlockComponent

            Component {
                id: emptyContentBlockComponent
                // can happen after Dictionary Updates
                // that a ContentBlock has no more keys
                Item {}
            }

            // LIST ROW DELEGTE
            Component {
                id: contentBlockRowComponent
                Item {
                    id: theItem
                    height: contentBlockRow.height
                    implicitWidth: appWindow.width
                    Rectangle {
                        anchors.top: theItem.top
                        height: contentBlockRow.height-2
                        width: 8
                        //color: dataUtil.contentBlockColor(model.modelData.contentBlockId)
                    }

                    ColumnLayout {
                        id: contentBlockRow
                        anchors.top: theItem.top
                        // without this divider not over total width
                        implicitWidth: appWindow.width
                        spacing: 0
                        RowLayout {
                            //implicitWidth: appWindow.width
                            ColumnLayout {
                                id: theColumn
                                Layout.fillWidth: true
                                Layout.leftMargin: 36
                                Layout.rightMargin: 12
                                Layout.topMargin: 12
                                Layout.bottomMargin: 12
                                // without setting a maximum width, word wrap not working
                                Layout.maximumWidth: appWindow.width-60
                                Layout.minimumWidth: appWindow.width-60

                                LabelHeadline {
                                    text: model.modelData.name != "*****" ? dataUtil.textForKeyContentBlock(model.modelData) : qsTr("* No ContentBlock assigned *")
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
                                    navPane.pushContentBlockKeys(model.modelData.contentBlockId)
                                }
                            }
                        }
                        HorizontalListDivider{}
                    } // end Col Layout creator row

                } // row item



            } // contentBlockRowComponent

        } // keyContentBlockLoader

        ScrollIndicator.vertical: ScrollIndicator { }

    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from contentBlockListPage")
        console.log("ContentBlocks # "+dataManager.keyContentBlockPropertyList.length)
        dataUtil.resolveKeysForContentBlocks()
        listView.model = dataManager.keyContentBlockPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from contentBlockListPage")
    }
} // end contentBlocklistPage
