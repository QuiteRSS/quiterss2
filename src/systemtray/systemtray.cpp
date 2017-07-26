#include "systemtray.h"
#include <QDebug>

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
