#include "../inc/webSocketClient.h"
#include "webSocketClient.h"
#include <iostream>

WebSocketClient::WebSocketClient(std::string host, std::string port, MessageCallback on_msg, StatusCallback on_status)
:host_(std::move(host)), port_(std::move(port)), 
        on_message_(std::move(on_msg)), 
        on_status_(std::move(on_status)),
        ioc_(1), ws_(ioc_), resolver_(ioc_),
        work_guard_(asio::make_work_guard(ioc_))
{
    m_thread = std::thread([this]{ioc_.run();});
}

WebSocketClient::~WebSocketClient()
{
    stop();
    if(m_thread.joinable()) m_thread.join();
}

void WebSocketClient::async_connect_and_handshake(){
    std::cout << host_ << ' ' << port_;
    resolver_.async_resolve(host_, port_, 
    [this](beast::error_code ec, tcp::resolver::results_type results){
        std::cerr << ec.message();
        if(ec) return;

        asio::async_connect(ws_.next_layer(), results.begin(), results.end(),
            
        [this](beast::error_code ec, tcp::resolver::iterator){
                if(ec) return;
                ws_.async_handshake(host_, "/",
                
                    [this](beast::error_code ec){
                    if(!ec){
                        connectionStatus(true);
                        doReadLoop();
                    }
                });
            });
    });
}

void WebSocketClient::connectionStatus(bool status){}

void WebSocketClient::send(json &&message){
    asio::post(ws_.get_executor(), [this, m = std::move(message)]() mutable { //
        outbox_.push_back(std::move(m).dump());
        if (outbox_.size() == 1)
            doWriteLoop(); 
    });
}
void WebSocketClient::stop(){
    work_guard_.reset();  
        ioc_.stop();
        if (m_thread.joinable()) m_thread.join();
}

void WebSocketClient::doReadLoop(){
    ws_.async_read(incoming_, [this](beast::error_code ec, size_t n){
        std::cout << "Received " << n << " bytes (" << ec.message() << ")" << std::endl;
        if(ec) return;

        auto received_message = beast::buffers_to_string(incoming_.data()).substr(0, n);
        json received_json = json::parse(received_message);
        
        incoming_.consume(n);
        doReadLoop();
    });
}

void WebSocketClient::doWriteLoop(){
    if(outbox_.empty()) return;

    ws_.async_write(asio::buffer(outbox_.front()), [this](beast::error_code ec, size_t n){
        std::cout << "Sent " << n << " bytes (" << ec.message() << ")" << std::endl;
            if (!ec) {
                outbox_.pop_front();
                doWriteLoop();
            }
    });
}
