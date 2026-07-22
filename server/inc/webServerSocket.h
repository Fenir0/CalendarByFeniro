#ifndef WEB_SERVER_SOCKET_H
#define WEB_SERVER_SOCKET_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

#include "postgresql.h"
#include "documentHandler.h"
#include "sessionWatcher.h"
#include "fileWatcher.h"

#include <vector>
#include <string>
#include <tuple>
#include <deque>


enum TASK{
    LOGIN,  
    SIGNUP, 
    LOGOUT, 

    UPDATE, // continuous
    CREATE,  
    SHARE,
    UNSUBSCRIBE,

    RENAME,
    LOAD,  
    LOADLIST, 
    LOADPEOPLE,
    REMOVE  
};



namespace asio      = boost::asio;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;
using tcp           = asio::ip::tcp;
using json          = nlohmann::json;

#define RESULT_RESPONSE std::tuple<ACTION_RESULT, json>

class WebSocketSession: public std::enable_shared_from_this<WebSocketSession>{
    public:
    WebSocketSession(tcp::socket socket, asio::io_context& ioc, asio::thread_pool& pool, uint32_t session_id);
    ~WebSocketSession();
    void run();

    uint32_t getId();

    void sendResponse(const json& response = "");

private:
    std::deque<std::string> write_queue_;
    std::mutex write_mutex_;
    bool is_writing_ = false;

    uint32_t session_id;
    bool logged = false;
    std::string current_user;
    std::string current_file;

    uint32_t current_file_id;
    uint32_t current_user_id;


    DocumentHandler workspace;

    void onAccept(beast::error_code ec);
    void onRead    (beast::error_code ec, size_t n);
    void doReadLoop();
    void onWrite   ();

    void kick();

    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    std::string response_;
    tcp::endpoint peer_ = ws_.next_layer().remote_endpoint();

    void onClose();

    RESULT_RESPONSE handleTask(TASK task, const json& input);
// logging actions
    RESULT_RESPONSE handleLOGINrequest(const json& input);
    RESULT_RESPONSE handleSIGNUPrequest(const json& input);
    RESULT_RESPONSE handleLOGOUTrequest();
// current document actions
    RESULT_RESPONSE handleCREATErequest(const json& input);
    RESULT_RESPONSE handleUPDATErequest(const json& input);
    RESULT_RESPONSE handleRENAMErequest(const json& input);
    RESULT_RESPONSE handleUNSUBSCRIBErequest(const json& input);
// documents overview action
    RESULT_RESPONSE handleSHARErequest(const json& input);
    RESULT_RESPONSE handleLOADLISTrequest(const json& input);
    RESULT_RESPONSE handleLOADPEOPLErequest(const json& input);
    RESULT_RESPONSE handleLOADrequest(const json& input);
    RESULT_RESPONSE handleDELETErequest(const json& input);

    asio::thread_pool& thread_pool_;
    PostgresqlWorker database;
};

class WebSocketServer{
    public:
    WebSocketServer(asio::io_context& ioc, tcp::endpoint endppint);

    void onSessionConnect(std::shared_ptr<WebSocketSession> session);
    void onSessionDisconnect(uint32_t session_id);


    private:
    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);

    tcp::acceptor acceptor_;
    asio::io_context& ioc;
    asio::thread_pool thread_pool_;
};

#endif


