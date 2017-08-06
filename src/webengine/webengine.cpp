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
#include "webengine.h"

#ifndef DISABLE_BROWSER
#ifdef MOBILE
#include <QtWebView>
#else
#include <QtWebEngine>
#include <QQuickWebEngineProfile>
#endif
#include "application.h"
#endif

WebEngine::WebEngine(QObject *parent) : QObject(parent)
{

}

void WebEngine::initialize()
{
#ifndef DISABLE_BROWSER
#ifdef MOBILE
    QtWebView::initialize();
#else
    QtWebEngine::initialize();
    loadSettings();
#endif
#endif
}

void WebEngine::loadSettings()
{
#ifndef DISABLE_BROWSER
    Settings settings;
    settings.beginGroup("Browser-Settings");
    QQuickWebEngineProfile *profile = QQuickWebEngineProfile::defaultProfile();
    profile->setPersistentCookiesPolicy(QQuickWebEngineProfile::AllowPersistentCookies);
    profile->setPersistentStoragePath(mainApp->dataDirPath());

    const QString &cachePath = settings.value("CachePath", mainApp->cacheDirPath()).toString();
    profile->setCachePath(cachePath);

    const bool allowCache = settings.value("AllowLocalCache", true).toBool();
    profile->setHttpCacheType(allowCache ? QQuickWebEngineProfile::DiskHttpCache
                                         : QQuickWebEngineProfile::MemoryHttpCache);

    const int cacheSize = settings.value("LocalCacheSize", 50).toInt() * 1000 * 1000;
    profile->setHttpCacheMaximumSize(cacheSize);

    profile->setSpellCheckEnabled(settings.value("SpellCheckEnabled", false).toBool());
    profile->setSpellCheckLanguages(settings.value("SpellCheckLanguages").toStringList());
    settings.endGroup();
#endif
}

QStringList WebEngine::getQmlSelectors()
{
    QStringList selectors;
#ifndef DISABLE_BROWSER
#ifdef MOBILE
    selectors.append("mobile");
#else
    selectors.append("webview");
#endif
#endif
    return selectors;
}
