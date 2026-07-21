#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QIcon>

class NotificationHandler: public QObject{
    Q_OBJECT
public:
    explicit NotificationHandler(QObject* parent = nullptr);

    Q_INVOKABLE void showSystemNotification(const QString& title, const QString& message);
private:
    QSystemTrayIcon *trayIcon;

};

#endif