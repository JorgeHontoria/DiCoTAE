// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

import "../pages"
import "../common"
import "../popups"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "DictionaryNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    // because of https://bugreports.qt.io/browse/QTBUG-54260
    // lastCurrentIndex will remember currentIndex, so we can reset before Page becomes currentItem on StackView
    property int lastCurrentIndex: 0

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "DictionaryNavPane"
        focus: true

        /*initialItem: KeyListView {
            id: initialItem
        }*/

        // STACK VIEW TRANSITIONS
        replaceEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 300
            }
        }
        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 300
            }
        }
        // end STACKVIEW TRANSITIONS


        // STACK VIEW INITIAL ITEM (Dictionary for all days)
        // immediately activated and pushed on stack as initialItem
        Loader {
            id: initialItemLoader
            source: "../pages/DaySwiper.qml"
            active: true
            visible: false
            onLoaded: {
                navPane.replace(item)
                //navPane.initialItem = item
                item.init()
                item.currentIndex = navPage.lastCurrentIndex
                appWindow.myDictionaryActive = false
                myDictionaryLoader.active = false
            }
        }
        // end STACK VIEW INITIAL ITEM
        // LAZY LOADER FOR MY DICTIONARY
        Loader {
            id: myDictionaryLoader
            source: "../pages/MyDictionaryPage.qml"
            active: false
            visible: false
            onLoaded: {
                navPane.replace(item)
                item.init()
                navPage.lastCurrentIndex = initialItemLoader.item.currentIndex
                appWindow.myDictionaryActive = true
                initialItemLoader.active = false
            }
        } // myDictionaryLoader


        Loader {
            id: creatorDetailPageLoader
            property int creatorId: -1
            active: false
            visible: false
            source: "../pages/CreatorDetailPage.qml"
            onLoaded: {
                item.creatorId = creatorId
                navPane.push(item)
                item.init()
            }
        }

        Loader {
            id: keyDetailPageLoader
            property int keyId: -1
            active: false
            visible: false
            source: "../pages/KeyDetailPage.qml"
            onLoaded: {
                item.keyId = keyId
                navPane.push(item)
                item.init()
            }
        }

        Loader {
            id: sectionDetailPageLoader
            property int sectionId: -1
            active: false
            visible: false
            source: "../pages/SectionDetailPage.qml"
            onLoaded: {
                item.sectionId = sectionId
                navPane.push(item)
                item.init()
            }
        }

        // only one Creator Detail in stack allowed to avoid endless growing stacks
        function pushCreatorDetail(creatorId) {
            if(creatorDetailPageLoader.active) {
                creatorDetailPageLoader.item.creatorId = creatorId
                var pageStackIndex = findPage(creatorDetailPageLoader.item.name)
                if(pageStackIndex > 0) {
                    backToPage(pageStackIndex)
                }
            } else {
                creatorDetailPageLoader.creatorId = creatorId
                creatorDetailPageLoader.active = true
            }
        }

        function pushKeyDetail(keyId) {
            if(keyDetailPageLoader.active) {
                keyDetailPageLoader.item.keyId = keyId
                var pageStackIndex = findPage(keyDetailPageLoader.item.name)
                if(pageStackIndex > 0) {
                    backToPage(pageStackIndex)
                }
            } else {
                keyDetailPageLoader.keyId = keyId
                keyDetailPageLoader.active = true
            }
        }

        function pushSectionDetail(sectionId) {
            sectionDetailPageLoader.sectionId = sectionId
            sectionDetailPageLoader.active = true
        }

        function findPage(pageName) {
            var targetPage = find(function(item) {
                return item.name == pageName;
            })
            if(targetPage) {
                return targetPage.StackView.index
            } else {
                console.log("Page not found in StackView: "+pageName)
                return -1
            }
        }

        function backToPage(targetStackIndex) {
            for (var i=depth-1; i > targetStackIndex; i--) {
                popOnePage()
            }
        }

        function backToRootPage() {
            for (var i=depth-1; i > 0; i--) {
                popOnePage()
            }
        }

        function popOnePage() {
            var page = pop()
            if(page.name == "CreatorDetailPage") {
                creatorDetailPageLoader.active = false
                return
            }
            if(page.name == "KeyDetailPage") {
                keyDetailPageLoader.active = false
                return
            }
            if(page.name == "SectionDetailPage") {
                sectionDetailPageLoader.active = false
                return
            }
        }

        function toggleDictionary() {
            if(myDictionaryLoader.active) {
                initialItemLoader.active = true
            } else {
                myDictionaryLoader.active = true
            }
        }

        function goToSearchResult() {
            initialItemLoader.active = false
            initialItemLoader.active = true
            initialItemLoader.item.goToPage(1)
        }

    } // navPane

    FloatingActionButton {
        visible: navPane.depth > 1 && !dataManager.settingsData().classicStackNavigation
        property string imageName: "/list.png"
        z: 1
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.backToRootPage()
        }
    }

    FloatingActionButton {
        visible: navPane.depth == 1
        property string imageName: appWindow.myDictionaryActive? "/dictionary.png" : "/stars.png"
        z: 1
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.toggleDictionary()
        }
    }

    /*// TIMEPICKER LAZY LOADED AT FIRST USE
    Loader {
        id: timePickerLoader
        active: false
        visible: false
        source: "../popups/TimePicker.qml"
        onLoaded: {
            item.modal = true
            item.titleText = qsTr("GoTo Keys @")
            item.open()
            item.setDisplay("10:00", true, true)
        }
    }
    // getting SIGNAL from TimePicker closed via Connections
    function timePickerClosed() {
        if(timePickerLoader.item.isOK) {
            // initialItemLoader.item == DaySwiper
            initialItemLoader.item.goToIndex(dataUtil.findFirstKeyItem(getCurrentIndex(),timePickerLoader.item.hrsDisplay+":"+timePickerLoader.item.minutesDisplay))
        } else {
            appWindow.showToast(qsTr("No Time selected"))
        }
    }

    Connections {
        target: timePickerLoader.item
        onClosed: timePickerClosed()
    }
    // executed from GoTo Button at TitleBar
    function pickTime() {
        if(timePickerLoader.active) {
            timePickerLoader.item.open()
            timePickerLoader.item.onHoursButtonClicked()
        } else {
            timePickerLoader.active = true
        }
    }
    // end TIMEPICKER*/

    // SECTION PICKER LAZY LOADED AT FIRST USE
    Loader {
        id: sectionPickerLoader
        active: false
        visible: false
        source: "../popups/SectionPicker.qml"
        onLoaded: {
            item.modal = true
            item.titleText = qsTr("GoTo")
            item.open()
        }
    }
    // getting SIGNAL from SectionPicker closed via Connections
    function sectionPickerClosed() {
        if(sectionPickerLoader.item.isOK) {
            initialItemLoader.item.goToIndex(dataUtil.findFirstKeyItemByLetter(getCurrentIndex(), sectionPickerLoader.item.selectedSection))
        }
    }
    Connections {
        target: sectionPickerLoader.item
        onClosed: sectionPickerClosed()
    }
    // executed from GoTo Button at TitleBar
    function pickSection() {
        if(sectionPickerLoader.active) {
            sectionPickerLoader.item.open()
        } else {
            sectionPickerLoader.active = true
        }
    }
    // end SECTION PICKER


    // sets the index of SwipeView/TabBar back to last remembered one
    function setCurrentIndex() {
        initialItemLoader.item.currentIndex = navPage.lastCurrentIndex
    }
    function getCurrentIndex() {
        return initialItemLoader.item.currentIndex
    }

    function destinationAboutToChange() {
        // nothing
    }

    // triggered from BACK KEYs:
    // a) Android system BACK
    // b) Back Button from TitleBar
    function goBack() {
        // check if goBack is allowed
        //
        navPane.popOnePage()
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT DictionaryNavPane")
        // initialItem.init() will be done from Loader
    }
    function cleanup() {
        console.log("Cleanup DictionaryNavPane")
    }

} // navPage
