/****************************************************************************
**
** Copyright (C) 2010-2017 David Rosca <nowrep@gmail.com>
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
#include "cookiejar.h"
#include "application.h"

#ifndef COOKIE_DEBUG
#define COOKIE_DEBUG 0
#endif

CookieJar::CookieJar(QWebEngineCookieStore *cookieStore, QObject *parent) :
    QObject(parent),
    m_cookieStore(cookieStore)
{
    loadSettings();
    m_cookieStore->loadAllCookies();

    connect(m_cookieStore, &QWebEngineCookieStore::cookieAdded,
            this, &CookieJar::slotCookieAdded);
    connect(m_cookieStore, &QWebEngineCookieStore::cookieRemoved,
            this, &CookieJar::slotCookieRemoved);
}

void CookieJar::loadSettings()
{
    Settings settings;
    settings.beginGroup("Cookie-Settings");
    m_allowCookies = settings.value("AllowCookies", true).toBool();
    m_filterThirdParty = settings.value("FilterThirdPartyCookies", false).toBool();
    m_filterTrackingCookie = settings.value("FilterTrackingCookie", false).toBool();
    m_whitelist = settings.value("Whitelist", QStringList()).toStringList();
    m_blacklist = settings.value("Blacklist", QStringList()).toStringList();
    settings.endGroup();
}

void CookieJar::setAllowCookies(bool allow)
{
    m_allowCookies = allow;
}

void CookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    m_cookieStore->deleteCookie(cookie);
}

QVector<QNetworkCookie> CookieJar::getAllCookies() const
{
    return m_cookies;
}

void CookieJar::deleteAllCookies()
{
    m_cookieStore->deleteAllCookies();
}

bool CookieJar::matchDomain(QString cookieDomain, QString siteDomain) const
{
    if (cookieDomain.startsWith(QLatin1Char('.'))) {
        cookieDomain = cookieDomain.mid(1);
    }

    if (siteDomain.startsWith(QLatin1Char('.'))) {
        siteDomain = siteDomain.mid(1);
    }

    return Common::matchDomain(cookieDomain, siteDomain);
}

bool CookieJar::listMatchesDomain(const QStringList &list, const QString &cookieDomain) const
{
    foreach (const QString &d, list) {
        if (matchDomain(d, cookieDomain)) {
            return true;
        }
    }

    return false;
}

void CookieJar::slotCookieAdded(const QNetworkCookie &cookie)
{
    if (rejectCookie(QString(), cookie, cookie.domain())) {
        m_cookieStore->deleteCookie(cookie);
        return;
    }

    m_cookies.append(cookie);
    emit cookieAdded(cookie);
}

void CookieJar::slotCookieRemoved(const QNetworkCookie &cookie)
{
    if (m_cookies.removeOne(cookie))
        emit cookieRemoved(cookie);
}

bool CookieJar::acceptCookie(const QUrl &firstPartyUrl, const QByteArray &cookieLine,
                             const QUrl &cookieSource) const
{
    const QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies(cookieLine);
    Q_ASSERT(cookies.size() == 1);

    const QNetworkCookie cookie = cookies.at(0);
    return !rejectCookie(firstPartyUrl.host(), cookie, cookieSource.host());
}

bool CookieJar::rejectCookie(const QString &domain, const QNetworkCookie &cookie,
                             const QString &cookieDomain) const
{
    if (!m_allowCookies) {
        bool result = listMatchesDomain(m_whitelist, cookieDomain);
        if (!result) {
#if COOKIE_DEBUG
            qDebug() << "not in whitelist" << cookie;
#endif
            return true;
        }
    }

    if (m_allowCookies) {
        bool result = listMatchesDomain(m_blacklist, cookieDomain);
        if (result) {
#if COOKIE_DEBUG
            qDebug() << "found in blacklist" << cookie;
#endif
            return true;
        }
    }

    if (m_filterThirdParty) {
        bool result = matchDomain(cookieDomain, domain);
        if (!result) {
#if COOKIE_DEBUG
            qDebug() << "purged for domain mismatch" << cookie << cookieDomain << domain;
#endif
            return true;
        }
    }

    if (m_filterTrackingCookie && cookie.name().startsWith("__utm")) {
#if COOKIE_DEBUG
        qDebug() << "purged as tracking " << cookie;
#endif
        return true;
    }

    return false;
}
