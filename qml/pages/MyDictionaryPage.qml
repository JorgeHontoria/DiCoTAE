// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"
import "../popups"

Page {
    id: myDictionaryListPage
    focus: true
    property string name: "myDictionaryListPage"
    property KeyLists keyLists
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
            text: qsTr("My Personal Dictionary")
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

    PopupInfo {
        id: popupDictionaryEmpty
        text: qsTr("Your Personal Dictionary is empty.\nTap on the 'Star' to add or remove")
        buttonText: "OK"
        modal: true
        closePolicy: Popup.NoAutoClose
        onClosed: {
            navPane.toggleDictionary()
        }
    } // popupInfo

    function myDictionaryRefreshed() {
        keyLists = dataUtil.myDictionary()
        listView.model = keyLists.dictionarydKeysPropertyList
        if(keyLists.dictionarydKeysPropertyList.length == 0) {
            popupDictionaryEmpty.open()
        }
    }

    Connections {
        target: dataUtil
        onMyDictionaryRefreshed: myDictionaryRefreshed()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init from myDictionaryListPage")

        // send signal to refresh the list model
        dataUtil.refreshMyDictionary()
        console.log("MyDictionary Keys:"+keyLists.dictionarydKeysPropertyList.length)
    }

    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from myDictionaryListPage")
    }
} // end primaryPage
