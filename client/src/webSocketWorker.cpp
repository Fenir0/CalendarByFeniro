#include "../inc/webSocketWorker.h"
#include "../inc/dayDataHandler.h"
#include <iostream>
#include "webSocketWorker.h"

uint32_t generateRequestId(){
    static std::atomic<uint32_t> counter (1);
    return counter.fetch_add(1);
}

WebSocketWorker::WebSocketWorker(QObject *parent){
                                                 
    m_client = std::make_shared<WebSocketClient>(ip, std::to_string(port),  [this](const std::string& raw_msg) {
                QMetaObject::invokeMethod(this, [this, raw_msg]() {
                    m_messages.append(QString::fromStdString(raw_msg));
                    emit messagesUpdated(); 
                }, Qt::QueuedConnection);
            },
            
            [this](bool connected) {
                m_isConnected = connected;
                QMetaObject::invokeMethod(this, [this, connected]() {
                    if(m_isConnected) emit connectionSucceeded(); 
                    else emit connectionFailed();
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

Q_INVOKABLE void WebSocketWorker::connectToServer(QString IP)
{
    std::string ip_host = IP.toStdString();
    m_connectionStatus = "Connecting";
    m_client->async_connect_and_handshake(ip_host);
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
        json newData = json::parse(raw_msg);
        DayDataHandler::instance().updateDataMapFromJSON(newData["data"]);
        emit messagesUpdated(); 
    }
}

void WebSocketWorker::setIp(std::string ip)
{
    this->ip = ip;
}
void WebSocketWorker::setPort(uint16_t port)
{
    this->port = port;
}
std::string WebSocketWorker::getIp()
{
    return ip;
}
uint16_t WebSocketWorker::getPort()
{
    return port;
}
void WebSocketWorker::onRawConnectionChanged(bool connected)
{
    m_isConnected = connected;
    m_connectionStatus = connected ? "Connected" : "Disconnected";
    if(m_isConnected) emit connectionSucceeded();
    else emit connectionFailed();
}
