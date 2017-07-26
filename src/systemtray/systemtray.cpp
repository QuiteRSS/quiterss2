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
#include "systemtray.h"

SystemTray::SystemTray(QObject *parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/images/tray-icon.png"));
    setToolTip("QuiteRSS");

    m_showWindowAct = new QAction(this);
    connect(m_showWindowAct, &QAction::triggered,
            this, &SystemTray::signalShowWindow);
    m_quitAct_ = new QAction(this);
    connect(m_quitAct_, &QAction::triggered,
            this, &SystemTray::signalQuit);

    m_menu = new QMenu();
    m_menu->addAction(m_showWindowAct);
    m_menu->addSeparator();
    m_menu->addAction(m_quitAct_);
    setContextMenu(m_menu);

    connect(this, &QSystemTrayIcon::activated,
            this, &SystemTray::iconActivated);
}

SystemTray::~SystemTray()
{
    m_menu->deleteLater();
}

void SystemTray::retranslateStrings()
{
    m_showWindowAct->setText(tr("Show Window"));
    m_quitAct_->setText(tr("Quit"));
}

void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        emit signalSingleClick();
        break;
    case QSystemTrayIcon::DoubleClick:
        emit signalDoubleClick();
        break;
    default:
        break;
    }
}
