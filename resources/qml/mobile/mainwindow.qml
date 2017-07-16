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
import QtQuick.Window 2.3
import QtWebView 1.1

Window {
  width: 1024
  height: 750
  visible: true
  title: "QuiteRSS"

  WebView {
      id: webView
      anchors.fill: parent
      url: "https://quiterss.org"
      onLoadingChanged: {
          if (loadRequest.errorString)
              console.error(loadRequest.errorString);
      }
  }
}
