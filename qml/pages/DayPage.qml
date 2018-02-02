// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0
import org.msolanogadea.data 1.0
import "../common"

Page {
    id: levelListPage
    focus: true
    property string name: "levelListPage"
    property Day repositoryDay
    bottomPadding: 6
    topPadding: 6

    // LIST VIEW
    KeyListView {
        id: listView
    }

    function goToItemIndex(theIndex) {
        if(theIndex == -1) {
            appWindow.showToast(qsTr("No Key found"))
            return
        }
        if(theIndex > 0) {
            if(theIndex == listView.model.length) {
                appWindow.showToast(qsTr("Too late for a Key"))
            }

            theIndex = theIndex-1
        }

        listView.positionViewAtIndex(theIndex, ListView.Beginning)
    }

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from levelListPage")
        console.log("Day# "+dataManager.dayPropertyList.length)
        repositoryDay = dataManager.dayPropertyList[index]

        console.log(repositoryDay.repositoryDay)
        console.log("Keys:"+repositoryDay.keysPropertyList.length)
        listView.model = repositoryDay.keysPropertyList
    }

    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from levelListPage")
    }
} // end primaryPage
