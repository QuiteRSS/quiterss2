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
#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QVector>
#include <QStringList>
#include <QNetworkCookie>
#include <QWebEngineCookieStore>

class CookieJar : public QObject
{
    Q_OBJECT
public:
    explicit CookieJar(QWebEngineCookieStore *cookieStore, QObject *parent = 0);

    void loadSettings();

    void setAllowCookies(bool allow);

    void deleteCookie(const QNetworkCookie &cookie);

    QVector<QNetworkCookie> getAllCookies() const;
    void deleteAllCookies();

signals:
    void cookieAdded(const QNetworkCookie &cookie);
    void cookieRemoved(const QNetworkCookie &cookie);

protected:
    bool matchDomain(QString cookieDomain, QString siteDomain) const;
    bool listMatchesDomain(const QStringList &list, const QString &cookieDomain) const;

private:
    void slotCookieAdded(const QNetworkCookie &cookie);
    void slotCookieRemoved(const QNetworkCookie &cookie);

    bool acceptCookie(const QUrl &firstPartyUrl, const QByteArray &cookieLine,
                      const QUrl &cookieSource) const;
    bool rejectCookie(const QString &domain, const QNetworkCookie &cookie,
                      const QString &cookieDomain) const;

    bool m_allowCookies;
    bool m_filterTrackingCookie;
    bool m_filterThirdParty;

    QStringList m_whitelist;
    QStringList m_blacklist;

    QWebEngineCookieStore *m_cookieStore;
    QVector<QNetworkCookie> m_cookies;

};

#endif // COOKIEJAR_H
