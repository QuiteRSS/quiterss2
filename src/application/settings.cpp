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
#include "settings.h"

#include <QCoreApplication>
#include <QDebug>

QSettings *Settings::m_settings = 0;

Settings::Settings()
{
    if (!m_settings->group().isEmpty())
        m_settings->endGroup();
}

Settings::~Settings()
{
    if (!m_settings->group().isEmpty())
        m_settings->endGroup();
}

void Settings::createSettings(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        m_settings = new QSettings(fileName, QSettings::IniFormat);
    } else {
        m_settings = new QSettings(QSettings::IniFormat,
                                   QSettings::UserScope,
                                   QCoreApplication::organizationName(),
                                   QCoreApplication::applicationName());
    }
}

QSettings *Settings::getSettings()
{
    return m_settings;
}

void Settings::syncSettings()
{
    m_settings->sync();
}

QString Settings::fileName() const
{
    return m_settings->fileName();
}

void Settings::beginGroup(const QString &prefix) const
{
    m_settings->beginGroup(prefix);
}

void Settings::endGroup() const
{
    m_settings->endGroup();
}

void Settings::setValue(const QString &key, const QVariant &defaultValue) const
{
    m_settings->setValue(key, defaultValue);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

bool Settings::contains(const QString &key) const
{
    return m_settings->contains(key);
}
