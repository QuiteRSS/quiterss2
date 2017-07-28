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
#include "systemtray.h"
#include "webengine.h"

#include <QQmlFileSelector>
#include <QQmlContext>
#include <QTranslator>
#include <QTimer>

Application::Application(int &argc, char **argv) :
    QtSingleApplication(argc, argv),
    m_noDebugOutput(false)
{
    qWarning() << "Run application";

    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    setQuitOnLastWindowClosed(false);

    WebEngine::initialize();
    createSystemTray();

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
  return static_cast<Application*>(QCoreApplication::instance());
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

void Application::createSystemTray()
{
    m_systemTray = new SystemTray(this);
    m_systemTray->retranslateStrings();
    m_engine.rootContext()->setContextProperty("systemTray", m_systemTray);
    connect(m_systemTray, &SystemTray::signalQuit,
            this, &Application::quitApp);

    m_systemTray->show();
}
