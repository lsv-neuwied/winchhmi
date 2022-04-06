import QtQuick 2.0
import QtQuick.Layouts 1.0

GridLayout {
    property real labelWidth: 200
    property real ledRadius: 10.0
    property variant ledModel

    Layout.fillWidth: true
    Layout.fillHeight: true

    columns: 2

    Repeater {
        model: ledModel
        Rectangle {
            Layout.margins: ledRadius * 0.25
            Layout.column: 0
            Layout.row: index
            height: ledRadius * 2
            width: ledRadius * 2
            radius: ledRadius
            color: model.color
        }
    }

    Repeater {
        model: ledModel
        Item {
            Layout.margins: ledRadius * 0.25
            Layout.column: 1
            Layout.row: index
            height: ledRadius * 2.0
            width: labelWidth;
            Text {
                anchors.left: parent.left
                color: "lightgray"
                text: model.caption
                font.pixelSize: ledRadius * 1.5
            }
        }
    }
}
