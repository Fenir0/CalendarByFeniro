#include "../inc/webSocketClient.h"
#include "../inc/webSocketWorker.h"
#include "../inc/appState.h"


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

void WebSocketClient::async_connect_and_handshake(std::string host){
    std::cout << host << ' ' << port_;
    host_ = host;
    port_ = "9002";
    resolver_.async_resolve(host_, port_, 
    [this](beast::error_code ec, tcp::resolver::results_type results){
        if(ec) return;

        asio::async_connect(ws_.next_layer(), results.begin(), results.end(),
            
        [this](beast::error_code ec, auto){
                if(ec) return;
                ws_.async_handshake(host_, "/",
                
                    [this](beast::error_code ec){
                    if(!ec){
                        connectionStatus(true);
                        doReadLoop();
                    }
                    else{
                        connectionStatus(false);
                    }
                });
            });
    });
}

void WebSocketClient::connectionStatus(bool status){
    on_status_(status);
}


void WebSocketClient::send(json &&message){
    asio::post(ws_.get_executor(), [this, m = std::move(message)]() mutable { //
        outbox_.push_back(std::move(m));
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
        std::string raw_message = beast::buffers_to_string(incoming_.data());
        if(json::accept(raw_message)){
            json received_json = json::parse(raw_message);
            WebSocketWorker::instance().onRawMessageReceived(raw_message);
        } else{

        }
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
