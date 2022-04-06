import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Window 2.2

ApplicationWindow {
    visible: true
    //visibility: Window.FullScreen
    width: 1024
    height: 600
    color: "black"

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        StackLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: bar.currentIndex

            MeteoPage {
            }

            ErrorPage {
            }

            IOPage {
            }
        }

        TabBar {
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            id: bar
            TabButton {
                text: "Wetter"
                font.pixelSize: 24
            }
            TabButton {
                text: "Fehler"
                font.pixelSize: 24
            }
            TabButton {
                text: "ECU I/O"
                font.pixelSize: 24
            }
        }
    }

}

