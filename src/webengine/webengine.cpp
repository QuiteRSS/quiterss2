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
#include "webengine.h"

#ifdef MOBILE
#include <QtWebView>
#else
#include <QtWebEngine>
#endif
#include "application.h"

WebEngine::WebEngine(QObject *parent) : QObject(parent)
{
#ifdef MOBILE
    QtWebView::initialize();
#else
    QtWebEngine::initialize();
    m_profile = QQuickWebEngineProfile::defaultProfile();
    m_cookieJar = new CookieJar(m_profile->cookieStore(), this);
#endif
    loadSettings();
}

WebEngine::~WebEngine()
{

}

void WebEngine::loadSettings()
{
    Settings settings;
    settings.beginGroup("Browser-Settings");
#ifndef MOBILE
    m_profile->setPersistentCookiesPolicy(QQuickWebEngineProfile::AllowPersistentCookies);
    m_profile->setPersistentStoragePath(mainApp->dataDirPath());

    const QString &cachePath = settings.value("CachePath", mainApp->cacheDirPath()).toString();
    m_profile->setCachePath(cachePath);

    const bool allowCache = settings.value("AllowLocalCache", true).toBool();
    m_profile->setHttpCacheType(allowCache ? QQuickWebEngineProfile::DiskHttpCache
                                         : QQuickWebEngineProfile::MemoryHttpCache);

    const int cacheSize = settings.value("LocalCacheSize", 50).toInt() * 1000 * 1000;
    m_profile->setHttpCacheMaximumSize(cacheSize);

    m_profile->setSpellCheckEnabled(settings.value("SpellCheckEnabled", false).toBool());
    m_profile->setSpellCheckLanguages(settings.value("SpellCheckLanguages").toStringList());
#endif
    settings.endGroup();
}

QStringList WebEngine::getQmlSelectors()
{
    QStringList selectors;
#ifdef MOBILE
    selectors.append("mobile");
#else
    selectors.append("webview");
#endif
    return selectors;
}
