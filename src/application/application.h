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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QSessionManager>
#include <qtsingleapplication.h>

#include "common.h"
#include "settings.h"
#include "ganalytics.h"
#include "networkmanager.h"
#ifndef DISABLE_BROWSER
#include "webengine.h"
#endif

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

    bool isClosing() const { return m_isClosing; }
    void setClosing() { m_isClosing = true; }
    bool isPortable() const { return m_isPortable; }
    bool isPortableAppsCom() const { return m_isPortableAppsCom; }
    bool dirPathInitialized() const { return m_dirPathInitialized; }
    QString resourcesDirPath() const { return m_resourcesDirPath; }
    QString dataDirPath() const { return m_dataDirPath; }
    QString cacheDirPath() const { return m_cacheDirPath; }
    QString defaultSoundNotifyFile() const;
    bool canWriteDebugMsgLog() const { return m_writeDebugMsgLog; }
    Q_INVOKABLE bool showSplashScreenEnabled() const { return m_showSplashScreen; }
    QQmlApplicationEngine *qmlEngine() { return &m_qmlEngine; }
    NetworkManager *networkManager() const { return m_networkManager; }
    GAnalytics *analytics() const { return m_analytics; }
#ifndef DISABLE_BROWSER
    WebEngine *webEngine() const { return m_webEngine; }
#endif

signals:
    void setSplashScreenValue(int value);
    void showMainWindow();
    void closeMainWindow();
    void showClosingWindow();
    void addFeed();

public slots:
    void quitApp();
    void receiveMessage(const QString &message);

private slots:
    void commitData(QSessionManager &manager);
    void saveState(QSessionManager &manager);

private:
    void checkPortable();
    void initDirPaths();
    void initSettings();
    void loadTranslation();
    QString getDefaultLanguage();
    void initGoogleAnalytics();
    void initSystemTray();
    void initWebEngine();
    void initQmlFileSelector();

    bool m_isPortable;
    bool m_isPortableAppsCom;
    bool m_isClosing;
    bool m_dirPathInitialized;

    QString m_resourcesDirPath;
    QString m_dataDirPath;
    QString m_cacheDirPath;

    bool m_writeDebugMsgLog;
    bool m_showSplashScreen;
    bool m_storeDBMemory;
    bool m_dbFileExists;
    bool m_isSaveDataLastFeed;
    bool m_updateFeedsStartUp;

    NetworkManager *m_networkManager;
    GAnalytics *m_analytics;
    QQmlApplicationEngine m_qmlEngine;
    SystemTray *m_systemTray;
    QTranslator *m_qtTranslator;
    QTranslator *m_appTranslator;
    QString m_language;
#ifndef DISABLE_BROWSER
    WebEngine *m_webEngine;
#endif


};

#endif // APPLICATION_H
