// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: creatorListPage
    focus: true
    property string name: "CreatorListPage"
    bottomPadding: 6
    topPadding: 6

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

    // LIST ROW DELEGTE
    Component {
        id: creatorRowComponent
        ItemDelegate {
            id: theItem
            height: creatorRow.height
            implicitWidth: appWindow.width
            onClicked: {
                navPane.pushCreatorDetail(model.modelData.creatorId)
            }
            ColumnLayout {
                id: creatorRow
                // without this divider not over total width
                implicitWidth: appWindow.width
                RowLayout {
                    spacing: 20
                    Layout.leftMargin: 16+12
                    Layout.rightMargin: 6
                    Layout.topMargin: 6
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
                            rightPadding: 12
                            text: model.modelData.bio.length? model.modelData.bio.substring(0, 240) + "..." : qsTr("Undrescription Creator")
                            wrapMode: Label.WordWrap
                            maximumLineCount: 6
                            elide: Label.ElideRight
                        } // label

                        /*LabelBody {
                            rightPadding: 12
                            text: dataUtil.keyInfoForCreator(model.modelData)
                            wrapMode: Label.WordWrap
                            maximumLineCount: 3
                            elide: Label.ElideRight
                        }*/
                    }
                    //                MouseArea {
                    //                    anchors.fill: parent
                    //                    onClicked: {
                    //                        navPane.pushCreatorDetail(model.modelData.creatorId)
                    //                    }
                    //                } // mouse
                } // end Row Layout
                HorizontalListDivider{}
            } // end Col Layout creator row
        } // item delegate
    } // creatorRowComponent

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
        // QList<Creator*>
        //model: dataManager.creatorPropertyList

        delegate: creatorRowComponent
        // header: headerComponent
        // in Landscape header scrolls away
        // in protrait header always visible
        // headerPositioning: isLandscape? ListView.PullBackHeader : ListView.OverlayHeader

        //section.property: "sortGroup"
        //section.criteria: ViewSection.FullString
        //section.delegate: sectionHeading

        ScrollIndicator.vertical: ScrollIndicator { }
    } // end listView

    function goToItemIndex(theIndex) {
        if(theIndex == -1) {
            return
        }
        if(theIndex > 0) {
            theIndex = theIndex-1
        }

        listView.positionViewAtIndex(theIndex, ListView.Beginning)
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from CreatorListPage")
        console.log("Creator# "+dataManager.creatorPropertyList.length)
        listView.model = dataManager.creatorPropertyList
    }

    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from CreatorListPage")
    }
} // end primaryPage
