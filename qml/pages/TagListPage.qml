// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: tagListPage
    focus: true
    property string name: "tagListPage"
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
        // QList<KeyTag*>
        //model: dataManager.keyTagPropertyList

        // important: use Loader to avoid errors because of https://bugreports.qt.io/browse/QTBUG-49224
        delegate: Loader {
            id: keyTagLoader
            // define Components inside Loader to enable direct access to ListView functions and modelData
            sourceComponent: keysPropertyList.length? tagRowComponent :emptyTagComponent

            Component {
                id: emptyTagComponent
                // can happen after Dictionary Updates
                // that a Tag has no more keys
                Item {}
            }

            // LIST ROW DELEGTE
            Component {
                id: tagRowComponent
                Item {
                    id: theItem
                    height: tagRow.height
                    implicitWidth: appWindow.width
                    Rectangle {
                        anchors.top: theItem.top
                        height: tagRow.height-2
                        width: 8
                        color: dataUtil.tagColor(model.modelData.tagId)
                    }

                    ColumnLayout {
                        id: tagRow
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
                                    text: model.modelData.name != "*****" ? dataUtil.textForKeyTag(model.modelData) : qsTr("* No Tag assigned *")
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
                                    navPane.pushTagKeys(model.modelData.tagId)
                                }
                            }
                        }
                        HorizontalListDivider{}
                    } // end Col Layout creator row

                } // row item



            } // tagRowComponent

        } // keyTagLoader

        ScrollIndicator.vertical: ScrollIndicator { }

    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from tagListPage")
        console.log("Tags # "+dataManager.keyTagPropertyList.length)
        dataUtil.resolveKeysForTags()
        listView.model = dataManager.keyTagPropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from tagListPage")
    }
} // end taglistPage
