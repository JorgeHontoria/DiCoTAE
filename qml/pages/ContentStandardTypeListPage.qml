// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: contentStandardTypeListPage
    focus: true
    property string name: "contentStandardTypeListPage"
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
        // QList<KeyContentStandardType*>
        //model: dataManager.keyContentStandardTypePropertyList

        // important: use Loader to avoid errors because of https://bugreports.qt.io/browse/QTBUG-49224
        delegate: Loader {
            id: keyContentStandardTypeLoader
            // define Components inside Loader to enable direct access to ListView functions and modelData
            sourceComponent: keysPropertyList.length? contentStandardTypeRowComponent :emptyContentStandardTypeComponent

            Component {
                id: emptyContentStandardTypeComponent
                // can happen after Dictionary Updates
                // that a ContentStandardType has no more keys
                Item {}
            }

            // LIST ROW DELEGTE
            Component {
                id: contentStandardTypeRowComponent
                Item {
                    id: theItem
                    height: contentStandardTypeRow.height
                    implicitWidth: appWindow.width
                    Rectangle {
                        anchors.top: theItem.top
                        height: contentStandardTypeRow.height-2
                        width: 8
                        //color: dataUtil.contentStandardTypeColor(model.modelData.contentStandardTypeId)
                    }

                    ColumnLayout {
                        id: contentStandardTypeRow
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
                                    text: model.modelData.name != "*****" ? dataUtil.textForKeyContentStandardType(model.modelData) : qsTr("* No ContentStandardType assigned *")
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
                                    navPane.pushContentStandardTypeKeys(model.modelData.contentStandardTypeId)
                                }
                            }
                        }
                        HorizontalListDivider{}
                    } // end Col Layout creator row

                } // row item



            } // contentStandardTypeRowComponent

        } // keyContentStandardTypeLoader

        ScrollIndicator.vertical: ScrollIndicator { }

    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from contentStandardTypeListPage")
        console.log("ContentStandardTypes # "+dataManager.keyContentStandardTypePropertyList.length)
        dataUtil.resolveKeysForContentStandardTypes()
        listView.model = dataManager.keyContentStandardTypePropertyList
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from contentStandardTypeListPage")
    }
} // end contentStandardTypelistPage
