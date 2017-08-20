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
#include "networkmanager.h"
#include "application.h"

#include <QNetworkReply>
#include <QNetworkProxy>

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent)
{

}

void NetworkManager::loadSettings()
{
    Settings settings;
    QNetworkProxy proxy;
    settings.beginGroup("Network-Settings");
    int type = settings.value("ProxyType", QNetworkProxy::NoProxy).toInt();
    proxy.setType(QNetworkProxy::ProxyType(type));
    proxy.setHostName(settings.value("HostName", QString()).toString());
    proxy.setPort(settings.value("Port", 80).toInt());
    proxy.setUser(settings.value("Username", QString()).toString());
    proxy.setPassword(settings.value("Password", QString()).toString());
    settings.endGroup();
    QNetworkProxy::setApplicationProxy(proxy);
}

QNetworkReply *NetworkManager::createRequest(QNetworkAccessManager::Operation op,
                                             const QNetworkRequest &request,
                                             QIODevice *outgoingData)
{
    QNetworkRequest req = request;
    req.setAttribute(QNetworkRequest::SpdyAllowedAttribute, true);
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}
