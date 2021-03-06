// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: sectionKeyListPage
    focus: true
    property string name: "sectionKeyListPage"
    property int sectionId
    property Section section
    bottomPadding: 6
    topPadding: 6

    header:
        ColumnLayout {
        Layout.fillWidth: true
        LabelHeadline {
            topPadding: 10
            leftPadding: 24
            rightPadding: 12
            id: headerLabel
            color: accentColor
            elide: Label.ElideRight
        }
        // workaround for BUG: if elide then bottompadding lost
        Item {
            height: 2
        }
    }

    // SECTION HEADER DELEGATE
    Component {
        id: sectionHeading
        Pane {
            topPadding: 0
            bottomPadding: 12
            leftPadding: 0
            rightPadding: 0
            width: parent.width
            background: Rectangle{color: Material.listHighlightColor}
            ColumnLayout {
                y: -6
                width: parent.width
                height: 48
                RowLayout {
                    Layout.topMargin: 6
                    spacing: 10
                    // TODO BUG IconColored sometimes washed out in list
                    IconActive {
                        Layout.leftMargin: 16 +36 + 20
                        imageName: "time.png"
                    }
                    LabelTitle {
                        text: dataUtil.localWeekdayAndTime(section)
                        anchors.verticalCenter: parent.verticalCenter
                        color: primaryColor
                        font.bold: true
                    }
                } // section row
                //HorizontalListDivider{}
            } // section col
        } // section Pane
    } // sectionHeading Component

    // LIST VIEW
    KeyListView {
        id: listView
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init from sectionKeyListPage")
        section = dataManager.findSectionBySectionId(sectionId)
        headerLabel.text = section.sectionName
        listView.model = section.keysPropertyList
        console.log("Section "+section.sectionName+" Keys:"+section.keysPropertyList.length)
    }

    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from sectionKeyListPage")
    }
} // end primaryPage
