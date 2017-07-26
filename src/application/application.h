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

#define mainApp Application::getInstance()

class SystemTray;

class Application : public QtSingleApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    ~Application();

    static Application *getInstance();

    QString dataDir() const { return m_dataDir; }
    bool isNoDebugOutput() const { return m_noDebugOutput; }

signals:

public slots:
    void quitApp();

private slots:
    void commitData(QSessionManager &manager);

private:
    void createSystemTray();

    QString m_dataDir;
    bool m_noDebugOutput;

    QQmlApplicationEngine m_engine;
    SystemTray *m_systemTray;

};

#endif // APPLICATION_H
