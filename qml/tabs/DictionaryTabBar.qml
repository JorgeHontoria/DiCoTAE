// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

import "../common"

ColumnLayout {
    id: myTabBar

    spacing: 2

    property bool advancedSearch: true

    property int currentIndex: 0
    onCurrentIndexChanged: {
        console.log("Tab Bar current index changed: "+ currentIndex)
        navSwipePane.currentIndex = currentIndex // navPane

        // Activamos el foco cuando cambiamos el currentIndex
        if (currentIndex === 0) {
            theSearchTextField.focus = true;
        }
    }

    RowLayout {
        spacing: 6

        TextField {
            id: theSearchTextField
            Layout.leftMargin: 18
            Layout.minimumWidth: appWindow.width - 74
            Layout.preferredWidth: appWindow.width - 74
            Layout.preferredHeight: 50
            selectByMouse: true
            placeholderText: qsTr("Search")
            visible: (currentIndex === 0) && (advancedSearch === true)

            inputMethodHints: Qt.ImhNoPredictiveText
            activeFocusOnPress : true
            onAccepted: {
                console.log("[SEARCH] - Key: " + theSearchTextField.text);
                appWindow.currentSearchText = theSearchTextField.text;
                navSwipePane.applyFilterExpression(theSearchTextField.text, appWindow.startWithSearch, appWindow.fullTextSearch);
            }
        }

        Item {
            visible: (currentIndex === 0) && (advancedSearch === true)
            Layout.preferredWidth: 50
            Layout.minimumHeight: 50

            ToolButton {
                id: buttonSearch
                focusPolicy: Qt.NoFocus
                Image {
                    anchors.centerIn: parent
                    source: (appWindow.isDarkTheme === 1) ? "qrc:/images/white/search.png" : "qrc:/images/black/search.png"
                }
                onClicked: {
                    console.log("[SEARCH] - Key: " + theSearchTextField.text);
                    appWindow.currentSearchText = theSearchTextField.text;
                    navSwipePane.applyFilterExpression(theSearchTextField.text, appWindow.startWithSearch, appWindow.fullTextSearch);
                }
            }
        }

        Item {
            visible: (currentIndex === 1) && (advancedSearch === true)
            Layout.preferredWidth: 50
            Layout.minimumHeight: 50

            ToolButton {
                id: buttonBack
                focusPolicy: Qt.NoFocus
                Image {
                    anchors.centerIn: parent
                    source: (appWindow.isDarkTheme === 1) ? "qrc:/images/white/arrow_back.png" : "qrc:/images/black/arrow_back.png"
                }
                onClicked: {
                    appWindow.currentSearchText = "Regresar";
                    navSwipePane.currentIndex = 0;
                }
            }
        }

        Label {
            visible: (currentIndex === 1) && (advancedSearch === true)
            Layout.minimumWidth: appWindow.width - 74
            Layout.preferredWidth: appWindow.width - 74
            Layout.preferredHeight: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            font.pixelSize: fontSizeSubheading
            text: appWindow.currentSearchText
        }
    }

    RowLayout {
        spacing: 6

        Item {
            visible: (currentIndex === 0) && (advancedSearch === true)
            Layout.minimumWidth: appWindow.width - 130
            Layout.preferredWidth: appWindow.width - 130
            Layout.minimumHeight: 20
            Layout.leftMargin: 20

            LabelSubheading {
                wrapMode: Text.WordWrap
                color: accentColor
                text: (appWindow.fullTextSearch === false) ? qsTr("Title search") : qsTr("Description search")
                font.pixelSize: fontSizeBodyAndButton
                anchors.fill: parent
            }
        }

        Item {
            visible: (currentIndex === 0) && (advancedSearch === true)
            Layout.preferredWidth: 50
            Layout.minimumHeight: 40

            Switch {
                id: switchFullTextSearch
                focusPolicy: Qt.NoFocus
                checked: appWindow.fullTextSearch
                onCheckedChanged: {
                    if(checked !== appWindow.fullTextSearch) {
                        appWindow.fullTextSearch = !appWindow.fullTextSearch
                        switchStartWithSearch.checked = false;
                    }
                }
            }
        }
    }

    RowLayout {
        spacing: 6
        visible: (switchFullTextSearch.checked === false)

        Item {
            visible: (currentIndex === 0) && (advancedSearch === true)
            Layout.minimumWidth: appWindow.width - 130
            Layout.preferredWidth: appWindow.width - 130
            Layout.minimumHeight: 20
            Layout.leftMargin: 20

            LabelSubheading {
                wrapMode: Text.WordWrap
                color: accentColor
                text: (appWindow.startWithSearch === false) ? qsTr("Word search") : qsTr("Start with search")
                font.pixelSize: fontSizeBodyAndButton
                anchors.fill: parent
            }
        }

        Item {
            visible: (currentIndex === 0) && (advancedSearch === true)
            Layout.preferredWidth: 50
            Layout.minimumHeight: 40

            Switch {
                id: switchStartWithSearch
                focusPolicy: Qt.NoFocus
                checked: appWindow.startWithSearch
                onCheckedChanged: {
                    if(checked !== appWindow.startWithSearch) {
                        appWindow.startWithSearch = !appWindow.startWithSearch
                    }
                }
            }
        }
    }

    Item {
        height: 10
    }

    HorizontalDivider {}
}
