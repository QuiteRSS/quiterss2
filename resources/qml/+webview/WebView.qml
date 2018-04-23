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
import QtQuick 2.8
import QtWebEngine 1.3

WebEngineView {
    id: root
    url: "https://google.com"

//    onNewViewRequested: {
//        if (!request.userInitiated)
//            print("Warning: Blocked a popup window.");
//        else if (request.destination == WebEngineView.NewViewInTab) {
//            var tab = tabs.createEmptyTab(currentWebView.profile);
//            tabs.currentIndex = tabs.count - 1;
//            request.openIn(tab.item);
//        } else if (request.destination == WebEngineView.NewViewInBackgroundTab) {
//            var backgroundTab = tabs.createEmptyTab(currentWebView.profile);
//            request.openIn(backgroundTab.item);
//        } else if (request.destination == WebEngineView.NewViewInDialog) {
//            var dialog = applicationRoot.createDialog(currentWebView.profile);
//            request.openIn(dialog.currentWebView);
//        } else {
//            var window = applicationRoot.createWindow(currentWebView.profile);
//            request.openIn(window.currentWebView);
//        }
//    }

//    onFullScreenRequested: {
//        if (request.toggleOn) {
//            webEngineView.state = "FullScreen";
//            browserWindow.previousVisibility = browserWindow.visibility;
//            browserWindow.showFullScreen();
//            fullScreenNotification.show();
//        } else {
//            webEngineView.state = "";
//            browserWindow.visibility = browserWindow.previousVisibility;
//            fullScreenNotification.hide();
//        }
//        request.accept();
//    }

    onRenderProcessTerminated: {
        var status = "";
        switch (terminationStatus) {
        case WebEngineView.NormalTerminationStatus:
            status = "(normal exit)";
            break;
        case WebEngineView.AbnormalTerminationStatus:
            status = "(abnormal exit)";
            break;
        case WebEngineView.CrashedTerminationStatus:
            status = "(crashed)";
            break;
        case WebEngineView.KilledTerminationStatus:
            status = "(killed)";
            break;
        }

        print("Render process exited with code " + exitCode + " " + status);
        reloadTimer.running = true;
    }

//    onWindowCloseRequested: {
//        if (tabs.count == 1)
//            browserWindow.close();
//        else
//            tabs.removeTab(tabs.currentIndex);
//    }

    Timer {
        id: reloadTimer
        interval: 0
        running: false
        repeat: false
        onTriggered: currentWebView.reload()
    }
}
