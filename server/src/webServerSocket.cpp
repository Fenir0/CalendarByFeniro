#include "../inc/webServerSocket.h"

WebSocketSession::WebSocketSession(tcp::socket socket) : ws_(std::move(socket)){}

WebSocketSession::~WebSocketSession(){}

void WebSocketSession::run()
{
    ws_.async_accept(beast::bind_front_handler(&WebSocketSession::onAccept, shared_from_this()));
}

void WebSocketSession::onAccept(beast::error_code ec)
{
    // websocket handshake
    std::cerr << "Accept: " << ec.message() << " for " << peer_ << std::endl;
    if(!ec){
        doReadLoop();
    }
}

void WebSocketSession::doReadLoop()
{
    ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::onRead, shared_from_this()));
}


void WebSocketSession::onRead(beast::error_code ec, size_t n)
{
    if (ec) {
        if (ec == websocket::error::closed) {
            return;
        }
        std::cerr << "Read error: " << ec.message() << std::endl;
        return;
    }
        try {
             response_.clear();

             auto it  = buffers_begin(buffer_.data());
             json msg = json::parse(it, it + n);
             buffer_.consume(n);

             if (msg.contains("$getfile$"))
                // read json file
                 response_ = json{{"type", "response"}, {"original", std::move(msg)}}.dump();
            
             if (response_.empty()) {
                 doReadLoop();
             } else {
                 ws_.text(true);
                 ws_.async_write(asio::buffer(response_),
                                 beast::bind_front_handler(&WebSocketSession::onWrite, shared_from_this()));
             }
         } catch (std::exception const& e) {
             std::cerr << "Processing error: " << e.what() << std::endl;
        }
}

void WebSocketSession::onWrite(beast::error_code ec, size_t n)
{
    std::cerr << "Write: " << n << " bytes (" << ec.message() << ")" << std::endl;
         if (!ec)
             doReadLoop();
}

/*
        - - - - -
*/

WebSocketServer::WebSocketServer(asio::io_context &ioc, tcp::endpoint endpoint): acceptor_(ioc, endpoint)
{
    doAccept();
}

// async wait for someone to connect
void WebSocketServer::doAccept()
{
    acceptor_.async_accept(beast::bind_front_handler(&WebSocketServer::onAccept, this));
}

// tcp handshake
void WebSocketServer::onAccept(beast::error_code ec, tcp::socket socket)
{
    if(ec){
        std::cerr << "Accept error: " << ec.message() << std::endl;
    }
    else{
        // handler for the new connection
        std::make_shared<WebSocketSession>(std::move(socket))->run();
    }
    // listen for the next connection
    doAccept();
}
