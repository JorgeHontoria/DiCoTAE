// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: contentStandardTypeKeyListPage
    focus: true
    property string name: "contentStandardTypeKeyListPage"
    property int contentStandardTypeId
    property KeyContentStandardType keyContentStandardType
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
    /*Component {
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
    } // sectionHeading Component*/

    // LIST VIEW
    KeyListView {
        id: listView
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init from contentStandardTypeKeyListPage")
        keyContentStandardType = dataManager.findKeyContentStandardTypeByContentStandardTypeId(contentStandardTypeId)
        // model.modelData.name != "*****" model.modelData.name : qsTr("* no ContentStandardType assigned *")
        if(keyContentStandardType.name != "*****") {
            headerLabel.text = keyContentStandardType.name.substring(2, keyContentStandardType.name.length)
        } else {
            headerLabel.text = qsTr("* no ContentStandardType assigned *")
        }
        listView.model = keyContentStandardType.keysPropertyList
        console.log("ContentStandardType "+keyContentStandardType.name+" Keys:"+keyContentStandardType.keysPropertyList.length)
    }

    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from contentStandardTypeKeyListPage")
    }
} // end primaryPage
