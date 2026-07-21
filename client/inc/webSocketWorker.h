#ifndef WEB_SOCKET_WORKER_H
#define WEB_SOCKET_WORKER_H

#include "webSocketClient.h"

#include <QThread>
#include <QObject>
#include <QString>
#include <QMutex>
#include <QHash>
#include<QJsonObject>

#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <thread>
#include <memory>
#include <atomic>

// =========================
// COMMUNICATION WITH SERVER
// =========================

class WebSocketWorker: public QObject{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStateChanged)
    Q_PROPERTY(QStringList messages READ messages NOTIFY messagesUpdated)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStateChanged)
public:
    using ResponseCallback = std::function<void(const json& response)>;

    static WebSocketWorker& instance();

    bool isConnected() const;
    QStringList messages() const;
    QString connectionStatus() const;

    Q_INVOKABLE void connectToServer(QString IP);
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendRequest(const json& message, ResponseCallback callback);
    Q_INVOKABLE void sendMessage(const std::string& message);
    Q_INVOKABLE void sendMessage(const QString& message);
    
    void onRawMessageReceived(const std::string& msg);
    void onDocumentsStateChange();

    void setIp(std::string ip);
    void setPort(uint16_t port);

    std::string getIp();
    uint16_t    getPort();
signals:
    void connectionStateChanged();
    void messagesUpdated();
    void documentsStateChanged();
    void connectionSucceeded();
    void connectionFailed();

private slots:
    void onRawConnectionChanged(bool connected); 

private:
    std::string ip;
    uint16_t port;
explicit WebSocketWorker(QObject *parent = nullptr);
    std::shared_ptr<WebSocketClient> m_client;  

    std::atomic<bool> m_isConnected;
    QStringList m_messages;
    QString m_connectionStatus = "disconnected";

    QHash<uint32_t, ResponseCallback> m_pendingRequests;
    QMutex m_mutex;

    WebSocketWorker(const WebSocketWorker&) = delete;
    WebSocketWorker& operator=(const WebSocketWorker&) = delete;
    WebSocketWorker(WebSocketWorker&&) = delete;
    WebSocketWorker& operator=(WebSocketWorker&&) = delete;

    ~WebSocketWorker() override;
};

#endif