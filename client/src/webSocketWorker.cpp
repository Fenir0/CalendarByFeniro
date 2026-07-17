#include "../inc/webSocketWorker.h"
#include <iostream>

uint32_t generateRequestId(){
    static std::atomic<uint32_t> counter (1);
    return counter.fetch_add(1);
}

WebSocketWorker::WebSocketWorker(QObject *parent){
                                                    // local
    m_client = std::make_shared<WebSocketClient>("127.0.0.1", "9002",  [this](const std::string& raw_msg) {
                QMetaObject::invokeMethod(this, [this, raw_msg]() {
                    m_messages.append(QString::fromStdString(raw_msg));
                    emit messagesUpdated(); 
                }, Qt::QueuedConnection);
            },
            
            [this](bool connected) {
                m_isConnected = connected;
                QMetaObject::invokeMethod(this, [this, connected]() {
                    emit connectionStateChanged(); 
                }, Qt::QueuedConnection);
            });
}

WebSocketWorker &WebSocketWorker::instance()
{
    static WebSocketWorker instance;
    return instance;
}

WebSocketWorker::~WebSocketWorker()
{if (m_client) m_client->stop();}

bool WebSocketWorker::isConnected() const
{
    return m_isConnected; 
}

QStringList WebSocketWorker::messages() const
{
    return m_messages; 
}

QString WebSocketWorker::connectionStatus() const
{return m_isConnected? "connected": "Disconnected"; }

Q_INVOKABLE void WebSocketWorker::connectToServer()
{
    m_connectionStatus = "Connecting";
    emit connectionStateChanged();
    m_client->async_connect_and_handshake();
}

Q_INVOKABLE void WebSocketWorker::disconnectFromServer()
{
    m_client->stop();
}

Q_INVOKABLE void WebSocketWorker::sendRequest(const json &message, ResponseCallback callback)
{
    uint32_t requestId = generateRequestId();

    json msg = message;
    msg["request_id"] =  requestId;
    {
        QMutexLocker lock(&m_mutex);
        m_pendingRequests[requestId] = callback;
    }
    sendMessage(msg.dump());
}

Q_INVOKABLE void WebSocketWorker::sendMessage(const std::string &message)
{
    if (!m_isConnected) return;
    m_client->send(message);
}

Q_INVOKABLE void WebSocketWorker::sendMessage(const QString& message)
{
    if (!m_isConnected) return;
    m_client->send(message.toStdString());
}

void WebSocketWorker::onRawMessageReceived(const std::string& raw_msg)
{   
    json msg = json::parse(raw_msg);
    uint32_t requestId = msg["request_id"];

    if(requestId){
        QMutexLocker lock (&m_mutex);
        auto it = m_pendingRequests.find(requestId);
        if(it != m_pendingRequests.end()){
            ResponseCallback callback = it.value();
            m_pendingRequests.erase(it);

            QMetaObject::invokeMethod(this, [callback, msg](){
                callback(msg);
            }, Qt::QueuedConnection);
        }
    }else{
        m_messages.append(QString::fromStdString(raw_msg));
        emit messagesUpdated(); 
    }
}


void WebSocketWorker::onRawConnectionChanged(bool connected)
{
    m_isConnected = connected;
    m_connectionStatus = connected ? "Connected" : "Disconnected";
    emit connectionStateChanged();
}
