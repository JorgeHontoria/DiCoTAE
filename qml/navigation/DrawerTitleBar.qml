// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import "../common"

ToolBar {
    id: myTitleBar

    RowLayout {
        focus: false
        spacing: 6
        anchors.fill: parent
        Item {
            width: 4
        }

        ToolButton {
            visible: !backButton.visible && (appWindow.isLandscape || !appWindow.hasOnlyOneMenu)
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/menu.png"
            }
            onClicked: {
                appWindow.openNavigationBar()
            }
        } // menu button

        // F A K E
        // fake button to avoid flicker and repositioning of titleLabel
        ToolButton {
            visible: !backButton.visible && appWindow.hasOnlyOneMenu && !appWindow.isLandscape
            enabled: false
            focusPolicy: Qt.NoFocus
        } // fake button

        ToolButton {
            id: backButton
            focusPolicy: Qt.NoFocus
            visible: initDone && navigationModel[navigationIndex].canGoBack && destinations.itemAt(navigationIndex).item.depth > 1
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/arrow_back.png"
            }
            onClicked: {
                destinations.itemAt(navigationIndex).item.goBack()
            }
        } // backButton

        LabelTitle {
            id: titleLabel
            text: currentTitle
            leftPadding: 6
            rightPadding: 6
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            color: textOnPrimary
        }

        /* TODO: CONTROL DE IS ONLINE
        // I S   O N L I N E
        ItemDelegate {
            id: isOnlineButton
            width: 36
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/download.png"
            }
            Rectangle {
                id: onlineMarker
                width: 8
                height: 8
                radius: width / 2
                // adjust colors or position in the middle
                color: appWindow.isOnline? titleBarOnlineColor : titleBarOfflineColor
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 12
            }
            // colors not working with selected palette:
            // use this diagonal line
            Rectangle {
                visible: !appWindow.isOnline
                width: 2
                height: 32
                color: textOnPrimary
                anchors.centerIn: parent
                rotation: -45
            }

            onClicked: {
                appWindow.showInfo(dataServer.networkInfo())
            }
        } // isOnlineButton
        */

        ToolButton {
            id: dictionaryGoToButton
            visible: !myDictionaryActive && navigationIndex == dictionaryNavigationIndex && destinations.itemAt(dictionaryNavigationIndex).item.depth == 1
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/goto.png"
            }
            onClicked: {
                //destinations.itemAt(dictionaryNavigationIndex).item.pickTime()
                destinations.itemAt(dictionaryNavigationIndex).item.pickSection()
            }
        } // dictionaryGoToButton

        /*ToolButton {
            id: creatorGoToButton
            visible: navigationIndex == creatorNavigationIndex && destinations.itemAt(creatorNavigationIndex).item.depth == 1
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/goto.png"
            }
            onClicked: {
                destinations.itemAt(3).item.pickSection()
            }
        } // dictionaryGoToButton
        */

        ToolButton {
            id: homeOptionsButton
            visible: navigationIndex == homeNavigationIndex
            focusPolicy: Qt.NoFocus
            Image {
                anchors.centerIn: parent
                source: "qrc:/images/"+iconOnPrimaryFolder+"/more_vert.png"
            }
            onClicked: {
                homeOptionsMenu.open()
            }
            Menu {
                id: homeOptionsMenu
                modal:true
                dim: false
                closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
                x: parent.width - width
                transformOrigin: Menu.TopRight
                MenuItem {
                    text: qsTr("Help")
                    onTriggered: {
                        navigationIndex = helpNavigationIndex
                    }
                }
                MenuItem {
                    text: qsTr("Settings")
                    onTriggered: {
                        navigationIndex = settingsNavigationIndex
                    }
                }
                MenuItem {
                    text: qsTr("About")
                    onTriggered: {
                        navigationIndex = aboutNavigationIndex
                    }
                }
                onAboutToShow: {
                    appWindow.modalMenuOpen = true
                }
                onAboutToHide: {
                    appWindow.modalMenuOpen = false
                    appWindow.resetFocus()
                }
            } // end optionsMenu

        } // end homeOptionsButton

        // F A K E
        // fake button to avoid flicker and repositioning of titleLabel
        /*ToolButton {
            visible: !dictionaryGoToButton.visible && !creatorGoToButton.visible && !homeOptionsButton.visible
            enabled: false
            focusPolicy: Qt.NoFocus
        } // fake button*/
    } // end RowLayout
} // end ToolBar


