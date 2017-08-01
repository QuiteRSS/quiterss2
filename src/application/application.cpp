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
#include "application.h"
#include "settings.h"
#include "systemtray.h"
#include "webengine.h"

#include <QDir>
#include <QFile>
#include <QQmlFileSelector>
#include <QQmlContext>
#include <QStandardPaths>
#include <QTranslator>
#include <QTimer>

Application::Application(int &argc, char **argv) :
    QtSingleApplication(argc, argv),
    m_noDebugOutput(false)
{
    setApplicationName("QuiteRSS");
    setOrganizationName("QuiteRSS");
    setOrganizationDomain("quiterss.org");
    setApplicationVersion(APP_VERSION);
    setWindowIcon(QIcon(":/images/quiterss.png"));
    setQuitOnLastWindowClosed(false);

    setAttribute(Qt::AA_UseHighDpiPixmaps);
    setAttribute(Qt::AA_EnableHighDpiScaling);

    checkPortable();
    checkDir();
    createSettings();

    qWarning() << "Run application";

    createSystemTray();
    WebEngine::initialize();

    QQmlFileSelector *qfs = new QQmlFileSelector(&m_engine, &m_engine);
    QStringList selectors = WebEngine::getQmlSelectors();
    qfs->setExtraSelectors(selectors);

    m_engine.rootContext()->setContextProperty("mainApp", this);
    m_engine.load(QUrl(QStringLiteral("qrc:/qml/mainwindow.qml")));
    // Loading slow components
    emit setSplashScreenValue(7);
    QTimer::singleShot(2000, this, &Application::showWindow);
}

Application::~Application()
{

}

Application *Application::getInstance()
{
    return static_cast<Application *>(QCoreApplication::instance());
}

void Application::quitApp()
{
    qWarning() << "End application";
    quit();
}

void Application::commitData(QSessionManager &manager)
{
    manager.release();
    quitApp();
}

void Application::checkPortable()
{
#if defined(Q_OS_WIN)
    m_isPortable = true;
    QString fileName(QCoreApplication::applicationDirPath() + "/portable.dat");
    if (!QFile::exists(fileName)) {
        m_isPortable = false;
    }
    if (m_isPortable) {
        fileName = QCoreApplication::applicationDirPath() + "/../../QuiteRSSPortable.exe";
        if (QFile::exists(fileName)) {
            m_isPortableAppsCom = true;
            QFile::remove(QCoreApplication::applicationDirPath() + "/updater.exe");
        }
    }
#endif
}

void Application::checkDir()
{
#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
    m_resourcesDir = QCoreApplication::applicationDirPath();
#else
#if defined(Q_OS_MAC)
    m_resourcesDir = QCoreApplication::applicationDirPath() + "/../Resources";
#else
    m_resourcesDir = RESOURCES_DIR;
#endif
#endif

    if (isPortable()) {
        m_dataDir = QCoreApplication::applicationDirPath() + "/data";
        m_cacheDir = m_dataDir + "/cache";
        m_soundDir = m_resourcesDir + "/sound";
    } else {
        m_dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        m_soundDir = m_resourcesDir + "/sound";
    }
    QDir dir(m_dataDir);
    dir.mkpath(m_dataDir);
}

void Application::createSettings()
{
    QString fileName;
    if (isPortable())
        fileName = dataDir() + "/" + QCoreApplication::applicationName() + ".ini";
    Settings::createSettings(fileName);

    Settings settings;
    settings.beginGroup("Main");
    m_storeDBMemory = settings.value("storeDBMemory", true).toBool();
    m_isSaveDataLastFeed = settings.value("createLastFeed", false).toBool();
    m_showSplashScreen = settings.value("showSplashScreen", true).toBool();
    m_updateFeedsStartUp = settings.value("autoUpdatefeedsStartUp", false).toBool();
    m_noDebugOutput = settings.value("noDebugOutput", true).toBool();

    QString lang;
    QString localLang = QLocale::system().name();
    bool findLang = false;
    QDir langDir(m_resourcesDir + "/translations");
    foreach (QString file, langDir.entryList(QStringList("*.qm"), QDir::Files)) {
        lang = file.section('.', 0, 0).section('_', 1);
        if (localLang == lang) {
            lang = localLang;
            findLang = true;
            break;
        }
    }
    if (!findLang) {
        localLang = localLang.left(2);
        foreach (QString file, langDir.entryList(QStringList("*.qm"), QDir::Files)) {
            lang = file.section('.', 0, 0).section('_', 1);
            if (localLang.contains(lang, Qt::CaseInsensitive)) {
                lang = localLang;
                findLang = true;
                break;
            }
        }
    }
    if (!findLang)
        lang = "en";

    m_langFileName = settings.value("langFileName", lang).toString();

    settings.endGroup();
}

void Application::createSystemTray()
{
    m_systemTray = new SystemTray(this);
    m_systemTray->retranslateStrings();
    m_engine.rootContext()->setContextProperty("systemTray", m_systemTray);
    connect(m_systemTray, &SystemTray::signalQuit,
            this, &Application::quitApp);

    m_systemTray->show();
}
