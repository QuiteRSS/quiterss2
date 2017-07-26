#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);
    ~SystemTray();

    QMenu *menu() const { return m_menu; }

signals:
    void signalSingleClick();
    void signalDoubleClick();
    void signalShowWindow();
    void signalQuit();

public slots:
    void retranslateStrings();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QMenu *m_menu;
    QAction *m_showWindowAct;
    QAction *m_quitAct_;

};

#endif // SYSTEMTRAY_H
