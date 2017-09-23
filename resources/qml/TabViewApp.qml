/****************************************************************************
**
** QuiteRSS is a open-source cross-platform news feed reader
** Copyright (C) 2011-2017 QuiteRSS Team <quiterssteam@gmail.com>
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
import QtQuick 2.8
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

TabView {
    id: tabs
    function createEmptyTab(profile) {
        var tab = addTab("", tabComponent);
        tab.active = true;
        tab.title = Qt.binding(function() { return tab.item.title });
        //        tab.icon = Qt.binding(function() { return tab.item.icon });
        //        tab.item.profile = profile;
        return tab;
    }

    Component.onCompleted: {
        createEmptyTab("defaultProfile")
        createEmptyTab("defaultProfile")
        createEmptyTab("defaultProfile")
    }

    style: TabViewStyle {
        frameOverlap: 0
        tabOverlap: 0
        tab: Rectangle {
            color: styleData.selected ? "#ffffff" :"#e1e1e1"
            implicitWidth: Math.max(text.width + image.width + 14, 80)
            implicitHeight: text.height + 16
            Row {
                anchors.fill: parent
                leftPadding: 5
                rightPadding: 5
                spacing: 5
                Image {
                    id: image
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.width: 16
                    sourceSize.height: 16
                    asynchronous: true
                    source: {
                        if (control.getTab(styleData.index).item)
                            control.getTab(styleData.index).item.icon
                        else
                            ""
                    }
                }
                Text {
                    id: text
                    anchors.verticalCenter: parent.verticalCenter
                    renderType: Text.NativeRendering
                    text: styleData.title
                }
            }
            Rectangle {
                color: "#c1c1c1"
                width: 1
                height: {
                    if (styleData.nextSelected || styleData.selected)
                        parent.height
                    else
                        text.height
                }
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }
            }
        }
        tabBar: Rectangle { color: "#e1e1e1" }
        frame: Rectangle { color: "#ffffff" }
    }

    Component {
        id: tabComponent
        WebView {

        }
    }
}
