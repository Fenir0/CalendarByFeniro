#ifndef WEB_SOCKET_WORKER_H
#define WEB_SOCKET_WORKER_H

#include "webSocketClient.h"

#include <QThread>
#include <QObject>
#include <QString>
#include <QMutex>

#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <thread>
#include <memory>

class WebSocketWorker: public QObject{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStateChanged)
    Q_PROPERTY(QStringList messages READ messages NOTIFY messagesUpdated)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStateChanged)
public:
    explicit WebSocketWorker(QObject *parent = nullptr);
    ~WebSocketWorker();

    bool isConnected() const;
    QStringList messages() const;
    QString connectionStatus() const;

    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendMessage(const QString& message);
    
signals:
    void messagesUpdated();
    void connectionStateChanged();

private slots:
    void onRawMessageReceived(const std::string& msg);
    void onRawConnectionChanged(bool connected); 

private:
    std::shared_ptr<WebSocketClient> m_client;  

    bool m_isConnected;
    QStringList m_messages;
    QString m_connectionStatus = "disconnected";
};

#endif