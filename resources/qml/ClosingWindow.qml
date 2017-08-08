import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    id: closingWindow
    title: "QuiteRSS"
    flags: Qt.Tool | Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint
    color: "transparent"
    minimumWidth: 150
    minimumHeight: 60
    x: (Screen.desktopAvailableWidth - width - 10)
    y: (Screen.desktopAvailableHeight - height - 10)

    Rectangle {
        enabled: false
        anchors.fill: parent
        radius: 8
        border.color: "#e6e6e6"
        border.width: 1

        Text {
            id: text
            anchors.fill: parent
            anchors.margins: 10
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: "<b>QuiteRSS</b><br><br>" + qsTr("Saving data...")
        }
    }

    onVisibleChanged: {
        console.log("closingWindow")
    }
}
