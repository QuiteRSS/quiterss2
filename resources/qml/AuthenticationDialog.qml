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
import QtQuick.Controls 1.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3

Window {
    id: root
    property string server
    property var auth
    property SystemPalette palette: SystemPalette { }

    signal accepted()
    signal rejected()

    title: qsTr("Authorization required")
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    color: palette.window
    minimumWidth: 400
    visible: true

//    onClosing: destroy()
    Component.onCompleted: {
        console.log(root.server)
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 10
        GridLayout {
            columns: 2
            anchors.fill: parent

            Label { text: qsTr("Server:"); }
            Label { text: root.server; Layout.fillWidth: true; }
            Label { text: qsTr("Message:"); }
            Label { Layout.fillWidth: true; }
            Label { text: qsTr("Username:"); }
            TextField { Layout.fillWidth: true; }
            Label { text: qsTr("Password:"); }
            TextField { Layout.fillWidth: true; }
            Rectangle { implicitHeight: 10; Layout.fillHeight: true; Layout.columnSpan: 2; }
            CheckBox { text: qsTr("Save password"); Layout.columnSpan: 2; }
        }

        Row {
            spacing: 5
            anchors.right: parent.right

            Button {
                id: dialogButtonOk
                text: qsTr("Ok")
                onClicked: {
                    accepted()
                    close()
                }
            }
            Button {
                id: dialogButtonCancel
                text: qsTr("Cancel")
                onClicked: {
                    rejected()
                    close()
                }
            }
        }
    }
}
