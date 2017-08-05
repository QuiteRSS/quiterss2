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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QSessionManager>
#include <qtsingleapplication.h>

#include "common.h"
#include "ganalytics.h"

#define mainApp Application::getInstance()

class SystemTray;

class Application : public QtSingleApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    ~Application();

    static Application *getInstance();

    Q_INVOKABLE QString version() const { return APP_VERSION; }
    Q_INVOKABLE QString reversion() const { return APP_REVISION; }
    Q_INVOKABLE QString releaseDate() const { return APP_DATE; }

    bool isPortable() const { return m_isPortable; }
    bool isPortableAppsCom() const { return m_isPortableAppsCom; }
    QString resourcesDir() const { return m_resourcesDir; }
    QString dataDir() const { return m_dataDir; }
    bool isNoDebugOutput() const { return m_noDebugOutput; }
    GAnalytics *analytics() const { return m_analytics; }

signals:
    void setSplashScreenValue(int value);
    void showWindow();

public slots:
    void quitApp();

private slots:
    void commitData(QSessionManager &manager);

private:
    void checkPortable();
    void checkDir();
    void createSettings();
    void initTranslator();
    void createGoogleAnalytics();
    void createSystemTray();

    bool m_isPortable;
    bool m_isPortableAppsCom;
    bool m_isClosing;

    QString m_resourcesDir;
    QString m_dataDir;
    QString m_cacheDir;
    QString m_soundDir;

    bool m_noDebugOutput;
    bool m_storeDBMemory;
    bool m_dbFileExists;
    bool m_isSaveDataLastFeed;
    bool m_showSplashScreen;
    bool m_updateFeedsStartUp;

    GAnalytics *m_analytics;
    QQmlApplicationEngine m_engine;
    SystemTray *m_systemTray;
    QTranslator *m_translator;
    QString m_langFileName;

};

#endif // APPLICATION_H
