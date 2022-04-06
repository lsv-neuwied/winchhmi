import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    Layout.alignment: Qt.AlignCenter

    LedPanel {
        anchors.centerIn: parent

        labelWidth: 550
        ledModel: errors
        ledRadius: 26.0
    }
}
