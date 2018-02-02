// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

import "../pages"
import "../common"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "ContentBlockNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "ContentBlockNavPane"
        focus: true

        initialItem: ContentBlockListPage {
            id: initialItem
        }

        Loader {
            id: contentBlockKeyListPageLoader
            property int contentBlockId: -1
            active: false
            visible: false
            source: "../pages/ContentBlockKeyListPage.qml"
            onLoaded: {
                item.contentBlockId = contentBlockId
                navPane.push(item)
                item.init()
            }
        }

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


        function pushContentBlockKeys(contentBlockId) {
            contentBlockKeyListPageLoader.contentBlockId = contentBlockId
            contentBlockKeyListPageLoader.active = true
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
            if(page.name == "contentBlockKeyListPage") {
                contentBlockKeyListPageLoader.active = false
                return
            }
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
        } // popOnePage

    } // navPane

    FloatingActionButton {
        visible: navPane.depth > 2 && !dataManager.settingsData().classicStackNavigation
        property string imageName: "/list.png"
        z: 1
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.backToPage(1)
        }
    } // FAB
    FloatingActionButton {
        visible: navPane.depth == 2 && !dataManager.settingsData().classicStackNavigation
        property string imageName: "/folder.png"
        z: 1
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            navPane.backToRootPage()
        }
    } // FAB

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
        console.log("INIT ContentBlockNavPane")
        initialItem.init()
    }
    function cleanup() {
        console.log("Cleanup ContentBlockNavPane")
    }

} // navPage
