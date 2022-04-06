import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    Layout.alignment: Qt.AlignCenter

    RowLayout {
        anchors.centerIn: parent
        spacing: 30

        ColumnLayout {
            Layout.alignment: Qt.AlignTop

            Text {
                Layout.alignment: Qt.AlignLeft
                font.pixelSize: 18
                color: "white"
                text: "Digitale Eingänge"
            }

            LedPanel {
                labelWidth: 230
                ledRadius: 12
                ledModel: din
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignTop

            Text {
                Layout.alignment: Qt.AlignLeft
                font.pixelSize: 18
                color: "white"
                text: "Eingangswerte"
            }

            ValuePanel {
                labelWidth: 230
                valueWidth: 80
                ledRadius: 12
                valueModel: vin
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignTop

            Text {
                Layout.alignment: Qt.AlignLeft
                font.pixelSize: 18
                color: "white"
                text: "Digitale Ausgänge"
            }

            LedPanel {
                labelWidth: 230
                ledRadius: 12
                ledModel: dout
            }
        }
    }
}
