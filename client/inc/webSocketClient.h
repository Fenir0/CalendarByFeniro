#ifndef WEB_SOCKET_CLIENT_H
#define WEB_SOCKET_CLIENT_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <nlohmann/json.hpp>

#include <functional>
#include <string>
#include <deque>
#include <memory>

using MessageCallback = std::function<void(const std::string&)>;
using StatusCallback = std::function<void(bool)>;

namespace asio      = boost::asio;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;
using tcp           = asio::ip::tcp;
using json          = nlohmann::json;

class WebSocketClient: public std::enable_shared_from_this<WebSocketClient>{
public:
    WebSocketClient(std::string host, std::string port, MessageCallback on_msg, StatusCallback on_status);
    ~WebSocketClient();
    void async_connect_and_handshake();
    void connectionStatus(bool statuc);
    
    void send(json&& message);
    void stop();
private:
    asio::io_context ioc_;
    asio::executor_work_guard<asio::io_context::executor_type> work_guard_;

    std::thread m_thread;
    tcp::resolver resolver_{ioc_};
    websocket::stream<tcp::socket> ws_{ioc_};  
    std::string host_;
    std::string port_;

    beast::flat_buffer incoming_;
    std::deque<std::string> outbox_;
    void doReadLoop();

    void doWriteLoop();

    MessageCallback on_message_;
    StatusCallback on_status_;
};

#endif
