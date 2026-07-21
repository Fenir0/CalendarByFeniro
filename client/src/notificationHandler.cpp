#include "../inc/notificationHandler.h"

NotificationHandler::NotificationHandler(QObject *parent): QObject(parent)
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("../img/icon.png"));
    trayIcon->show();
}

Q_INVOKABLE void NotificationHandler::showSystemNotification(const QString &title, const QString &message)
{
    if (QSystemTrayIcon::supportsMessages()) {
        QString t = "123";
        QString tt = "456";
            trayIcon->showMessage(t, tt, QSystemTrayIcon::Information);
    }
}
