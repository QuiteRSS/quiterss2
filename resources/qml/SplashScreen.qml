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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Window {
    id: splashScreen

    function setValue(value) {
        progressBar.value = value;
    }

    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    color: "transparent"
    x: (Screen.width - splashImage.width) / 2
    y: (Screen.height - splashImage.height) / 2
    width: splashImage.width
    height: splashImage.height
    visible: mainApp.showSplashScreenEnabled();
    onClosing: destroy()

    Image {
        id: splashImage
        source: "qrc:/images/splash-screen.png"
    }
    ColumnLayout {
        id: layout
        anchors.fill: parent

        Text {
            font.pixelSize: 12
            textFormat: Text.RichText
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignRight
            color: "#FFFFFF"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: 85
            Layout.rightMargin: 25

            Component.onCompleted: {
                var revisionStr = mainApp.reversion()
                if (revisionStr !== "0")
                    revisionStr = "<BR>" + qsTr("Revision") + " " + revisionStr;
                else
                    revisionStr = "";
                var versionStr =
                        "<div align=\"right\">" +
                        qsTr("Version") + " " + mainApp.version() +
                        " (" + mainApp.releaseDate() + ")" +
                        revisionStr +
                        "</div>"
                this.text = versionStr
            }
        }
        Text {
            font.pixelSize: 12
            textFormat: Text.RichText
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignRight
            color: "#FFFFFF"
            Layout.fillWidth: true
            Layout.rightMargin: 25

            Component.onCompleted: {
                var copyrightStr =
                        "<div align=\"right\">" +
                        "http://quiterss.org" +
                        "<BR>Copyright &copy; 2011-2018 QuiteRSS Team" +
                        "</div>"
                this.text = copyrightStr
            }
        }
        ProgressBar {
            id: progressBar
            minimumValue: 0
            maximumValue: 10
            height: 3
            Layout.fillWidth: true
            Layout.leftMargin: 1
            Layout.rightMargin: 1
            Layout.bottomMargin: 20

            style: ProgressBarStyle {
                background: Rectangle {
                    color: "transparent"
                }
                progress: Rectangle {
                    color: "#FFE766"
                }
            }
        }
    }
}
