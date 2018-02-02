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
    property string name: "About"

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
                    text: qsTr("DiCoTAE\nDiCoTAE, Spain\n(Qt 5.9.x - Android + iOS + UWP)")
                    color: primaryColor
                }
            }
            HorizontalDivider {}
            RowLayout {
                Image {
                    id: imagePhoto
                    source: "qrc:/images/extra/solanogadea-thumbnail.png"
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("<a href=\"https://es.linkedin.com/in/solanogadea\">Miguel Solano Gadea</a>, I have been an ICT consultant (Information and Communication Technologies) for Public Administrations (AAPPs) on management issues oriented to Electronic Administration, in the last 25 years (since 1992).")
                    color: accentColor
                    onLinkActivated: Qt.openUrlExternally("https://es.linkedin.com/in/solanogadea")
                }

            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("<a href=\"http://www.solanogadea.es/Diccionario_AE_MSG.pdf\">PDF format dictionary</a>")
                    color: accentColor
                    onLinkActivated: Qt.openUrlExternally("http://www.solanogadea.es/Diccionario_AE_MSG.pdf")
                }
            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("<a href=\"mailto:miguel@solanogadea.es\">Contact me</a>")
                    color: accentColor
                    onLinkActivated: Qt.openUrlExternally("mailto:miguel@solanogadea.es")
                }
            }
            HorizontalDivider {}
            RowLayout {
                Image {
                    source: "qrc:/images/extra/tipesoft-thumbnail.png"
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("<a href=\"https://es.linkedin.com/in/jorgehontoria\">Jorge Hontoria Jimenez</a>, architect of software solutions for more than 10 years for the AAPP and private companies of all sizes.")
                    color: accentColor
                    onLinkActivated: Qt.openUrlExternally("https://es.linkedin.com/in/jorgehontoria")
                }
            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("<a href=\"mailto:jorge.hontoria@tipesoft.com\">Contact me</a>")
                    color: accentColor
                    onLinkActivated: Qt.openUrlExternally("mailto:jorge.hontoria@tipesoft.com")
                }
            }
            HorizontalDivider {}
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("This is one of my first APPs for Android and iOS.")
                }
            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("This is also one of my first APPs I developed using Qt. This APP is developed with Qt 5.9.x and brandnew <a href=\"http://doc.qt.io/qt-5/qtquickcontrols2-index.html\">QtQuickControls2</a>.")
                    onLinkActivated: Qt.openUrlExternally("http://doc.qt.io/qt-5/qtquickcontrols2-index.html")
                }
            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("All work is done in my spare time and complete sourcecode is available at <a href=\"https://github.com/solanogadea/c2gDiCoTAE_x\">Github</a>")
                    onLinkActivated: Qt.openUrlExternally("https://github.com/solanogadea/c2gDiCoTAE_x")
                }
            }
            HorizontalDivider {}
            RowLayout {
                LabelTitle {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Need a Business APP for Android, iOS or Windows?")
                    color: primaryColor
                }
            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("TipeSoft is doing development for Enterprise and SMB (Small and Medium Business). Most APPs are 'hidden' because they're running as inhouse APP at customer site. Need an APP? ask <a href=\"https://tipesoft.com/contacto\">TipeSoft</a>")
                    onLinkActivated: Qt.openUrlExternally("https://tipesoft.com/contacto")
                }
            }
            HorizontalDivider {}
            RowLayout {
                LabelTitle {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Thanks !")
                    color: primaryColor
                }
            }
            RowLayout {
                Item {
                    width: imagePhoto.width
                }
                LabelSubheading {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Thanks to our families for joining us in all these tasks.")
                }
            }
            HorizontalDivider {}
        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log("Init done from ABOUT")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from ABOUT")
    }
} // flickable
