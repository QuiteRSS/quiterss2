import QtQuick 2.8
import QtQuick.Window 2.2

Window {
  visible: true
  width: 640
  height: 480
  title: "QuiteRSS"
  Text {
    id: name
    text: qsTr("test")
    anchors.centerIn: parent
  }
}
