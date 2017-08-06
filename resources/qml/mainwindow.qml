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
import QtQuick.Window 2.3

ApplicationWindow {
    id: mainWindow

    property var splashScreen: SplashScreen {
        id: splashScreen
        visible: mainApp.showSplashScreenEnabled();
    }

    title: "QuiteRSS"
    height: 750
    width: 1024

    WebView {
        anchors.fill: parent
    }

    Connections {
        target: systemTray

        onSignalShowWindow: {
            mainWindow.show()
            mainWindow.raise()
            mainWindow.requestActivate()
        }

        onSignalSingleClick: {
            mainWindow.show()
            mainWindow.raise()
            mainWindow.requestActivate()
        }
    }
    Connections {
        target: mainApp

        onCloseWindow: {
            close();
        }

        onShowWindow: {
            mainWindow.visible = true
            splashScreen.visible = false
        }
        onSetSplashScreenValue: {
            splashScreen.setValue(value)
        }
    }

    Component.onCompleted: {
        analytics.sendScreenview("mainWindow")
    }

    onClosing: {
        mainApp.quitApp()
    }
}
