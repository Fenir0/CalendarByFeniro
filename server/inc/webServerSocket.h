#ifndef WEB_SERVER_SOCKET_H
#define WEB_SERVER_SOCKET_H

 #include <boost/asio.hpp>
 #include <boost/beast.hpp>
 #include <iostream>
 #include <nlohmann/json.hpp>

namespace asio      = boost::asio;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;
using tcp           = asio::ip::tcp;
using json          = nlohmann::json;

class WebSocketSession: public std::enable_shared_from_this<WebSocketSession>{
    public:
    WebSocketSession(tcp::socket socket);
    ~WebSocketSession();
    void run();
    private:
    void onAccept(beast::error_code ec);
    void onRead(beast::error_code ec, size_t n);
    void doReadLoop();
    void onWrite(beast::error_code ec, size_t n);

    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    std::string response_;
    tcp::endpoint peer_ = ws_.next_layer().remote_endpoint();
};

class WebSocketServer{
    public:
    WebSocketServer(asio::io_context& ioc, tcp::endpoint endppint);

    private:
    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);

    tcp::acceptor acceptor_;
};

#endif
