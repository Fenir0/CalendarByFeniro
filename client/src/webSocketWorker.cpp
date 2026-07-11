#include "../inc/webSocketWorker.h"

WebSocketWorker::WebSocketWorker(QObject *parent){
                                                    // local
    m_client = std::make_shared<WebSocketClient>("127.0.0.1", "9002",  [this](const std::string& raw_msg) {
                // ASIO THREAD
                QMetaObject::invokeMethod(this, [this, raw_msg]() {
                    m_messages.append(QString::fromStdString(raw_msg));
                    emit messagesUpdated(); 
                }, Qt::QueuedConnection);
            },
            
            [this](bool connected) {
                // ASIO THREAD 
                QMetaObject::invokeMethod(this, [this, connected]() {
                    m_isConnected = connected;
                    emit connectionStateChanged(); 
                }, Qt::QueuedConnection);
            });
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

#include <iostream>
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

Q_INVOKABLE void WebSocketWorker::sendMessage(const QString &message)
{
    if (!m_isConnected) return;
    m_client->send(message.toStdString());
}

void WebSocketWorker::onRawMessageReceived(const std::string& msg)
{   
    m_messages.append(QString::fromStdString(msg));
    emit messagesUpdated(); 
}


void WebSocketWorker::onRawConnectionChanged(bool connected)
{
    m_isConnected = connected;
    m_connectionStatus = connected ? "Connected" : "Disconnected";
    emit connectionStateChanged();
}
