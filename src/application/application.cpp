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
#include "application.h"
#include "systemtray.h"

#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QLibraryInfo>
#include <QQmlFileSelector>
#include <QQmlContext>
#include <QStandardPaths>
#include <QTranslator>
#include <QTimer>

Application::Application(int &argc, char **argv) :
    QtSingleApplication(argc, argv),
    m_isPortable(false),
    m_isPortableAppsCom(false),
    m_isClosing(false),
    m_dirPathInitialized(false),
    m_writeDebugMsgLog(false),
    m_analytics(0),
    m_qtTranslator(0),
    m_appTranslator(0)
{
    QString message = arguments().value(1);
    if (isRunning()) {
        if (argc == 1) {
            sendMessage("--show");
        } else {
            for (int i = 2; i < argc; ++i)
                message += '\n' + arguments().value(i);
            sendMessage(message);
        }
        setClosing();
        return;
    }

    setApplicationName("QuiteRSS");
    setOrganizationName("QuiteRSS");
    setOrganizationDomain("quiterss.org");
    setApplicationVersion(APP_VERSION);
    setWindowIcon(QIcon(":/images/quiterss.png"));
    setQuitOnLastWindowClosed(false);

    checkPortable();
    initDirPaths();
    initSettings();

    qWarning() << "Run application";

    m_networkManager = new NetworkManager(this);
    m_qmlEngine.rootContext()->setContextProperty("networkManager", m_networkManager);

    loadTranslation();
    initGoogleAnalytics();
    initSystemTray();
    initWebEngine();
    initQmlFileSelector();

    m_qmlEngine.rootContext()->setContextProperty("mainApp", this);
    m_qmlEngine.load(QUrl(QStringLiteral("qrc:/qml/mainwindow.qml")));
    // Loading slow components
    emit setSplashScreenValue(7);

    if (showSplashScreenEnabled())
        QTimer::singleShot(2000, this, &Application::showMainWindow);
    else
        emit showMainWindow();

    receiveMessage(message);
    connect(this, &Application::messageReceived, this, &Application::receiveMessage);
    connect(this, &Application::commitDataRequest, this, &Application::commitData);
    connect(this, &Application::saveStateRequest, this, &Application::saveState);

//    QUrl collect_url("https://mail.google.com/mail/u/0/feed/atom");
//    QNetworkRequest request;
//    QNetworkReply *reply;
//    request.setUrl(collect_url);
//    reply = m_networkManager->get(request);
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
    if (isClosing())
        return;

    setClosing();
    emit showClosingWindow();
    processEvents();

    if (m_analytics) {
        m_analytics->endSession();
        m_analytics->waitForIdle();
        delete m_analytics;
    }

    qWarning() << "End application";
    quit();
}

void Application::commitData(QSessionManager &manager)
{
    qWarning() << "commitData";
    manager.setRestartHint(QSessionManager::RestartNever);
    manager.release();
    quitApp();
}

void Application::saveState(QSessionManager &manager)
{
    qWarning() << "saveState";
    manager.setRestartHint(QSessionManager::RestartNever);
    manager.release();
}

void Application::receiveMessage(const QString &message)
{
    if (!message.isEmpty()) {
        qWarning() << QString("Received message: %1").arg(message);

        QStringList params = message.split('\n');
        foreach (QString param, params) {
            if (param == "--show") {
                if (isClosing())
                    return;
                emit showMainWindow();
            }
            if (param == "--exit")
                quitApp();
            if (param.contains("feed:", Qt::CaseInsensitive)) {
                QClipboard *clipboard = QApplication::clipboard();
                if (param.contains("https://", Qt::CaseInsensitive)) {
                    param.remove(0, 5);
                    clipboard->setText(param);
                } else {
                    param.remove(0, 7);
                    clipboard->setText("http://" + param);
                }
                emit addFeed();
            }
        }
    }
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

void Application::initDirPaths()
{
#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
    m_resourcesDirPath = QCoreApplication::applicationDirPath();
#else
#if defined(Q_OS_MAC)
    m_resourcesDirPath = QCoreApplication::applicationDirPath() + "/../Resources";
#elif defined(Q_OS_ANDROID)
    m_resourcesDirPath = ":";
#else
    m_resourcesDirPath = RESOURCES_DIR;
#endif
#endif

    if (isPortable()) {
        m_dataDirPath = QCoreApplication::applicationDirPath() + "/data";
        m_cacheDirPath = m_dataDirPath + "/cache";
    } else {
        m_dataDirPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        m_cacheDirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    }
    QDir dir(m_dataDirPath);
    dir.mkpath(m_dataDirPath);

    m_dirPathInitialized = true;
}

QString Application::defaultSoundNotifyFile() const
{
    return m_resourcesDirPath + "/sound/notification.wav";
}

void Application::initSettings()
{
    QString fileName;
    if (isPortable())
        fileName = dataDirPath() + "/" + QCoreApplication::applicationName() + ".ini";
    Settings::createSettings(fileName);

    Settings settings;
    settings.beginGroup("General-Settings");
    m_showSplashScreen = settings.value("ShowSplashScreen", true).toBool();
    m_writeDebugMsgLog = settings.value("WriteDebugMsgLog", false).toBool();
    settings.endGroup();
}

void Application::loadTranslation()
{
    Settings settings;
    m_language = settings.value("General-Settings/Language", getDefaultLanguage()).toString();

    if (!m_appTranslator)
        m_appTranslator = new QTranslator(this);
    removeTranslator(m_appTranslator);
    m_appTranslator->load("quiterss_" + m_language, resourcesDirPath() + "/translations");
    installTranslator(m_appTranslator);

    if (!m_qtTranslator)
        m_qtTranslator = new QTranslator(this);
    removeTranslator(m_qtTranslator);
    m_qtTranslator->load("qt_" + m_language,
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if (m_qtTranslator->isEmpty())
        m_qtTranslator->load("qt_" + m_language, resourcesDirPath() + "/translations");
    installTranslator(m_qtTranslator);
}

QString Application::getDefaultLanguage()
{
    QString language;
    QString localLanguage = QLocale::system().name();
    bool isFindLanguage = false;
    QDir translationsDir(resourcesDirPath() + "/translations");
    foreach (QString file, translationsDir.entryList(QStringList("*.qm"), QDir::Files)) {
        language = file.section('.', 0, 0).section('_', 1);
        if (localLanguage == language) {
            language = localLanguage;
            isFindLanguage = true;
            break;
        }
    }
    if (!isFindLanguage) {
        localLanguage = localLanguage.left(2);
        foreach (QString file, translationsDir.entryList(QStringList("*.qm"), QDir::Files)) {
            language = file.section('.', 0, 0).section('_', 1);
            if (localLanguage.contains(language, Qt::CaseInsensitive)) {
                language = localLanguage;
                isFindLanguage = true;
                break;
            }
        }
    }
    if (!isFindLanguage)
        language = "en";
    return language;
}

void Application::initGoogleAnalytics()
{
    Settings settings;
    bool statisticsEnabled = settings.value("General-Settings/StatisticsEnabled", true).toBool();
    if (statisticsEnabled) {
        QString clientID;
        if (!settings.contains("GAnalytics-cid")) {
            settings.setValue("GAnalytics-cid", QUuid::createUuid().toString());
        }
        clientID = settings.value("GAnalytics-cid").toString();
        m_analytics = new GAnalytics(this, TRACKING_ID, clientID);
        m_qmlEngine.rootContext()->setContextProperty("analytics", m_analytics);
        m_analytics->generateUserAgentEtc();
        m_analytics->startSession();
    }
}

void Application::initSystemTray()
{
    m_systemTray = new SystemTray(this);
    m_systemTray->retranslateStrings();
    m_qmlEngine.rootContext()->setContextProperty("systemTray", m_systemTray);
    connect(m_systemTray, &SystemTray::signalQuit,
            this, &Application::quitApp);

    m_systemTray->show();
}

void Application::initWebEngine()
{
#ifndef DISABLE_BROWSER
    m_webEngine = new WebEngine(this);
#endif
}

void Application::initQmlFileSelector()
{
    QQmlFileSelector *qfs = new QQmlFileSelector(&m_qmlEngine, &m_qmlEngine);
    QStringList selectors;
#ifndef DISABLE_BROWSER
    selectors.append(webEngine()->getQmlSelectors());
#endif
    qfs->setExtraSelectors(selectors);
}
