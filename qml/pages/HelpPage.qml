// solanogadea (Miguel Solano Gadea) @solanogadea
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
//import QtGraphicalEffects 1.0

import "../common"

Flickable {
    id: flickable
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: root.implicitHeight
    // StackView manages this, so please no anchors here
    // anchors.fill: parent
    property string name: "Help"

    Pane {
        id: root
        anchors.fill: parent
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            RowLayout {
                Image {
                    source: "qrc:/images/extra/dicotae-app.png"
                }
                LabelHeadline {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("DiCoTAE\nDiCoTAE, Spain\n(Qt 5.9.x - Android + iOS)")
                    color: primaryColor
                }
            }
            HorizontalDivider {}
            LabelTitle {
                leftPadding: 10
                text: qsTr("Introduction")
                color: primaryColor
            }
            HelpTextRow {
                helpText: qsTr("This work, both in content and in the computer application that has been installed, has been made with the author's own funds, without receiving any contribution or subsidy from entities, administrations or individuals. It is free to download and install without resorting to ads for its sustainability. \n\nIt only aims to help and serve others and provide knowledge acquired in years of professional performance. For this reason there is no call center or reception of emails. It is 'As It Is' (as it is or is, at a given time in time). It will be done or possible for this operation, sequentially in time in stores (Markets): Android, Windows and Apple.")
                color: primaryColor
                contentPadding: 4
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Send comment")
                color: accentColor
            }

            RowLayout {
                Layout.leftMargin: 20
                MenuItemWithIcon {
                    leftPadding: 10
                    rightPadding: 10
                    itemText: qsTr("Send suggestion to the creator") + "..."
                    imageName: "mail.png"
                    onTriggered: {
                        Qt.openUrlExternally("mailto:dicotae@solanogadea.es")
                    }
                }
            } // commentsRow

            HorizontalDivider {}
            LabelTitle {
                leftPadding: 10
                text: qsTr("Focus")
                color: accentColor
            }
            HelpTextRow {
                helpText: qsTr("It allows the reader to know the differences between a conventional dictionary of terms and this one that is, at the same time, of terms and concepts. Concepts have multiple forms of expression and must be reached by any means. The work, independently of the APPP, has an analytical index that links concepts with different writing forms.")
                contentPadding: 4
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("About the dictionary")
                color: accentColor
            }
            HelpTextRow {
                helpText: qsTr("It is the introduction to the Dictionary in its 'sequence' version that imposes the pdf, ePub or paper format. In this type of support, the terms and concepts are 'trapped' in the alphabetical order in which the word that defines them is expressed. They can not be searched, which differentiates them from the DICOTAE APP. It is explained how a solution is given to the formalization of concepts by analyzing them by including as many 'See' voices of referral, as necessary.")
                contentPadding: 4
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Instructions for use")
                color: accentColor
            }
            HelpTextRow {
                helpText: qsTr("Instructions on navigation, the direction of the screens, the text formats and their meaning, the icons and the physical or logical keys of the device. In addition, configuration elements since the APP can be executed in different operating environments (future).")
                contentPadding: 4
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("The work in numbers")
                color: accentColor
            }
            HelpTextRow {
                helpText: qsTr("The Electronic Administration can be considered only around the administrative procedure or its use in all management activities: public, private, judicial, parliamentary, notarial, registration, voting and social networks as an 'official' form of communication. The integral Electronic Administration, domain of the work, would be equivalent to 'Electronic Management' as a process of dematerialization from paper, pen, books and stamps to the electronic.")
                contentPadding: 4
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Help in detail")
                color: accentColor
            }

            RowLayout {
                Layout.leftMargin: 20
                MenuItemWithIcon {
                    leftPadding: 10
                    rightPadding: 10
                    itemText: qsTr("See detailed help") + "..."
                    imageName: "pdf.png"
                    onTriggered: {
                        Qt.openUrlExternally("http://www.solanogadea.es/dicotae/app.pdf")
                    }
                }
            } // helpRow

            HorizontalDivider {}
            LabelTitle {
                leftPadding: 10
                text: qsTr("Misc")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "menu.png"
                helpText: qsTr("Menu Button: Opens the Drawer. Drawer can also be opened with Gestures: swiping from left site.")
            }
            HelpIcoRow {
                iconName: "more_vert.png"
                helpText: qsTr("Options Button: Opens a Menu with some options.")
            }
            HelpIcoRow {
                iconName: "arrow_back.png"
                helpText: qsTr("Back Button: Top/Left from TitleBar goes one Page back. On Android you can also use OS - specific Back Button below the Page.")
            }
            HelpIcoRow {
                iconName: "list.png"
                helpText: qsTr("Speed Navigation: Back to the List below in the stack without the need to move fingers to Top/Left Back Button.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("DiCoTAE")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "home.png"
                helpText: qsTr("Homepage - the first Page. From here you can start Updates.")
            }
            /*HelpIcoRow {
                iconName: "refresh.png"
                helpText: qsTr("DiCoTAE Repository APP already contains the Repository Data, so you can start without waiting for Downloads. To get Dictionary- and Creator Updates tap on the Refresh Button.")
            }*/
            LabelTitle {
                leftPadding: 10
                text: qsTr("Dictionary")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "dictionary.png"
                helpText: qsTr("Complete Repository Dictionary separated by Days and sorted by Starttime. Switch between Repository Days by Swiping left/right or tapping on a Tab from Tab Bar.")
            }
            HelpIcoRow {
                iconName: "goto.png"
                helpText: qsTr("To find a specific Key tap on this Button and select the Section.")
            }
            HelpIcoRow {
                iconName: "stars.png"
                helpText: qsTr("Button to see your Personal Repository Dictionary. Mark Keys for your Personal Dictionary by checking the Favorites Button.")
            }
            HelpIcoRow {
                iconName: "form.png"
                helpText: qsTr("Copy all the contents of the term to the clipboard so you can use it in other places.")
            }
            /*HelpIcoRow {
                iconName: "goto.png"
                helpText: qsTr("There are many Keys listed for a Day - to make it easier to jump to a specific Timeslot tap on this Button and select the Time.")
            }*/
            LabelTitle {
                leftPadding: 10
                text: qsTr("Creator")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "creator.png"
                helpText: qsTr("List of all Creators sorted by Last Name.")
            }
            /*HelpIcoRow {
                iconName: "goto.png"
                helpText: qsTr("To find a specific Creator tap on this Button and select the Section.")
            }*/
            LabelTitle {
                leftPadding: 10
                text: qsTr("Tags")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "tag.png"
                helpText: qsTr("List of all Tags. Tap on a row to see all Keys of selected Tag.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Blocks")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "folder.png"
                helpText: qsTr("List of all Blocks. Tap on a row to see all Keys of selected Block.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Standard types")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "folder_fav.png"
                helpText: qsTr("List of all Standard types. Tap on a row to see all Keys of selected Standard type.")
            }
            /*LabelTitle {
                leftPadding: 10
                text: qsTr("Venue")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "venue.png"
                helpText: qsTr("Informations and Address of the Venue. From here you can also see a List of all Sections.")
            }
            HelpIcoRow {
                iconName: "directions.png"
                helpText: qsTr("List of all Sections. Tap on a Row to see all Keys running in this Section. Tap on the Thumbnail to see the Categoryplan of the selected Section.")
            }*/
            LabelTitle {
                leftPadding: 10
                text: qsTr("Settings")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "settings.png"
                helpText: qsTr("Customize App. You can change the Theme (light or dark) and also Primary or Accent Color. Colors can be selected from Material Style Colors.")
            }
            LabelTitle {
                leftPadding: 10
                text: qsTr("Keys")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "stars.png"
                helpText: qsTr("The Favorites Button. Tap on it to mark a Key and to add this to your Personal Dictionary. Button is a Toggle - tap again to remove from your Personal Dictionary.")
            }
            HelpIcoRow {
                iconName: "form.png"
                helpText: qsTr("Copy all text to clipboard.")
            }
            /*HelpIcoRow {
                iconName: "folder_fav.png"
                helpText: qsTr("See the standard type where the Key runs.")
            }*/
            /*HelpIcoRow {
                iconName: "calendar.png"
                helpText: qsTr("Repository Date.")
            }*/
            /*HelpIcoRow {
                iconName: "time.png"
                helpText: qsTr("Key Time from - to.")
            }*/
            HorizontalDivider {}
            /*LabelTitle {
                leftPadding: 10
                text: qsTr("Navigation")
                color: accentColor
            }
            HelpIcoRow {
                iconName: "my_location.png"
                helpText:  qsTr("DiCoTAE Repository app uses some special Navigation Styles to speed up Navigation.\nIf you don't like this way to go and want to use Google Material Classic Navigation Style you can customize this from 'Settings'.")
            }
            HorizontalDivider {}*/
            LabelTitle {
                leftPadding: 10
                text: qsTr("Need more Infos ?\nFollow at Twitter")
                color: accentColor
            }

            RowLayout {
                Layout.leftMargin: 20
                MenuItemWithIcon {
                    leftPadding: 10
                    rightPadding: 10
                    itemText: qsTr("Follow at Twitter") + "..."
                    imageName: "mail.png"
                    onTriggered: {
                        Qt.openUrlExternally("https://twitter.com/solanogadea?lang=es")
                    }
                }
            } // commentsRow
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from HELP")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from HELP")
    }
} // flickable
