/****************************************************************************
**
** QuiteRSS is a open-source cross-platform news feed reader
** Copyright (C) 2011-2018 QuiteRSS Team <quiterssteam@gmail.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/
import QtQuick 2.9
import QtQuick.Window 2.3

Window {
    id: closingWindow
    title: "QuiteRSS"
    modality: Qt.ApplicationModal
    flags: Qt.Tool | Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint
    color: "transparent"
    visible: true
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
}
