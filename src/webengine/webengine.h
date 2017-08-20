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
#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <QObject>
#ifndef MOBILE
#include <QQuickWebEngineProfile>
#include "cookiejar.h"
#endif

class WebEngine : public QObject
{
    Q_OBJECT
public:
    explicit WebEngine(QObject *parent = 0);
    ~WebEngine();

    void loadSettings();
    QStringList getQmlSelectors();
    QQuickWebEngineProfile *profile() const { return m_profile; }

private:
#ifndef MOBILE
    QQuickWebEngineProfile *m_profile;
    CookieJar *m_cookieJar;
#endif

};

#endif // WEBENGINE_H
