#include "../inc/webServerSocket.h"
#include "../inc/postgresql.h"
#include "../inc/resourceManager.h"
#include "webServerSocket.h"

std::string getResultAsString(ACTION_RESULT result){
    switch (result)
    {
    case SUCCESS: return "SUCCESS";
    case FAILURE: return "FAILURE";

    case USERNAME_TAKEN:    return "USERNAME TAKEN";
    case WRONG_PASSWORD:    return "WRONG PASSWORD";
    case FORBIDDEN_SYMBOLS: return "FORBIDDEN SYMBOLS";
    
    case USER_NOT_FOUND:    return "USER NOT FOUND";
    case FILE_NOT_FOUND:    return "FILE NOT FOUND";

    case FILE_NOT_CHOSEN:   return "FILE NOT CHOSEN";

    case PERMISSION_WRITE:  return "PERMISSION WRITE";
    case PERMISSION_READ:   return "PERMISSION READ";
    case PERMISSION_DENIED: return "PERMISSION DENIED";
    
    default: return "What did you do lol";
    }
}

std::vector<std::string> getStringVectorFromString(const std::string& transmission){
    std::vector<std::string> res;
    std::string tmp = "";
    size_t i = 0;
    while(i <= transmission.length()){
        if(transmission[i] != '$') tmp += transmission[i];
        else{
            if(!tmp.empty())res.push_back(tmp);
            tmp = "";
        }
        i++;
        if(i == transmission.length()){
            if(!tmp.empty()) res.push_back(tmp);
            return res;
        }
    }
    return res;
}

std::vector<std::string> getStringVectorFromTransmission(const json& msg){
    std::vector<std::string> res;
    std::string tmp = "";
    std::string transmission = msg.dump();
    size_t i = 0;
    while(i <= transmission.length()){
        if(transmission[i] != '$') tmp += transmission[i];
        else{
            res.push_back(tmp);
            tmp = "";
        }
        i++;
        if(i == transmission.length()){
            if(tmp.length() > 0) res.push_back(tmp);
            return res;
        }
    }
    return res;
}

TASK getTaskByString(std::string task){
    if(task == "LOGIN")  return LOGIN;
    if(task == "SIGNUP") return SIGNUP;
    if(task == "LOGOUT") return LOGOUT;
    if(task == "UPDATE") return UPDATE;
    if(task == "SHARE")  return SHARE;
    if(task == "LOAD")   return LOAD;
    if(task == "DELETE") return DELETE;
}

WebSocketSession::WebSocketSession(tcp::socket socket, asio::thread_pool& pool) : 
            ws_(std::move(socket)), thread_pool_(pool), database(), workspace(){}

WebSocketSession::~WebSocketSession(){}

void WebSocketSession::run()
{  ws_.async_accept(beast::bind_front_handler(&WebSocketSession::onAccept, shared_from_this()));  }

void WebSocketSession::onAccept(beast::error_code ec)
{// websocket handshake 
    std::cerr << "Accept: " << ec.message() << " for " << peer_ << std::endl;
    if(!ec){ doReadLoop(); }
}

void WebSocketSession::doReadLoop()
{  ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::onRead, shared_from_this()));  }


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

            std::string raw  = beast::buffers_to_string(buffer_.data());
            json transmission = json::parse(raw);
            buffer_.consume(n);

            asio::post(thread_pool_, [self = shared_from_this(), data = std::move(transmission)]() mutable {
                std::string task =  data["action"];
                TASK currentTask = getTaskByString(task);

                auto[taskResult, taskResponse] = self->handleTask(currentTask, data);
                taskResponse["result"] = getResultAsString(taskResult);
                taskResponse["request_id"] = data["request_id"];
                std::string res = taskResponse.dump();
                self->sendResponse(taskResponse);
            });

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

void WebSocketSession::sendResponse(const json &response)
{
    response_ = response.dump();
    ws_.async_write( asio::buffer(response_),beast::bind_front_handler(&WebSocketSession::onWrite, shared_from_this()));
} 

/*
         HANDLERS
*/
RESULT_RESPONSE WebSocketSession::handleTask(TASK task, const json& input)
{
    switch (task)
    {
    case LOGIN:
        return handleLOGINrequest(input);

    case SIGNUP:
        return handleSIGNUPrequest(input);

    case LOGOUT:
        return handleLOGOUTrequest();

    case SAVE:
        return handleSAVErequest(input);

    case SHARE:
        return handleSHARErequest(input);

    case LOAD:
        return handleLOADrequest(input);

    case DELETE:
        return handleDELETErequest(input);
                                      
    default:
        break;
    }    
}

RESULT_RESPONSE WebSocketSession::handleLOGINrequest(const json& input)
{   
    std::string username = input["username"];
    std::string pwd      = input["password"];
    ACTION_RESULT result = database.logUserPwd(username, pwd);
    if(result == SUCCESS){
        logged = true;
        current_user_id = database.getUserId(username);
        current_user = username;
    }
    json res;
    res["user_id"]  = current_user_id;
    res["username"] = current_user;
    return {result, res};

}

RESULT_RESPONSE WebSocketSession::handleSIGNUPrequest(const json& input)
{
    std::string username = input["username"];
    std::string pwd      = input["password"];
    ACTION_RESULT result = database.signUserPwd(username, pwd);
    if(result == SUCCESS){
        logged = true;
        current_user_id = database.getUserId(username);        
        current_user = username;
    }
    json res;
    res["user_id"]  = current_user_id;
    res["username"] = current_user;
    return {result, res};
}

RESULT_RESPONSE WebSocketSession::handleLOGOUTrequest()
{
    logged = false;
    current_user = "";
    current_user_id = -1;
    return {SUCCESS, {}};
}

RESULT_RESPONSE WebSocketSession::handleUPDATErequest(const json &input)
{
    json data = input["data"];
    workspace.saveDataOnServer(data);
}

RESULT_RESPONSE WebSocketSession::handleRENAMErequest(const json &input)
{
    std::string filename = input["filename"];
    ACTION_RESULT result;
    // renaming existing file
    if(current_file_id > -1){
        result = database.checkExistanceAndPermission(current_file_id, current_user_id);
        if(result == SUCCESS){
            result = database.renameFile(current_file_id, filename);
        } 
    }
    // new file entry (ask for a file name on log in)
    else{
        result = database.registerFile(current_file_id, current_user_id, filename);
    }
    if(result == SUCCESS){
        current_file = filename;
        if(current_file_id == -1) current_file_id = database.getFileIdByFilename_UserID(filename, current_user_id);
    }
    json res;
    return {result, res};
}

RESULT_RESPONSE WebSocketSession::handleSAVErequest(const json& input)
{
    if(workspace.getFileId() == -1){
        return handleCREATErequest(input);
    }

    ACTION_RESULT result = database.checkExistanceAndPermission(current_file_id, current_user_id);
    if(result == PERMISSION_WRITE){
        workspace.saveDataOnServer(input["data"]);
    }
    else if(result == PERMISSION_READ){
        return {PERMISSION_DENIED, {"No access to saving"}};
    }
    else if(result == FILE_NOT_CHOSEN){
        return {FILE_NOT_CHOSEN, {"Choose a file first"}};
    }
    return RESULT_RESPONSE();
}

RESULT_RESPONSE WebSocketSession::handleSHARErequest(const json& input)
{
    std::string filename = input["filename"];
    int accessLevel      = input["access_level"];
    std::string username = input["username"];
    ACTION_RESULT result = database.checkExistanceAndPermission(current_file_id, current_user_id);   
    if(result == PERMISSION_WRITE){
        result = database.changeAccessLevelForUser(current_file_id, username, accessLevel);
        return {result, PERMISSION_DENIED};
    }
    else if(result == PERMISSION_READ){
        return {PERMISSION_DENIED, PERMISSION_DENIED};
    }
}

RESULT_RESPONSE WebSocketSession::handleCREATErequest(const json &input)
{
    ACTION_RESULT result = database.registerFile(input["filename"]);
    return RESULT_RESPONSE();
}

RESULT_RESPONSE WebSocketSession::handleLOADrequest(const json &input)
    
{
    sendResponse();
    

}

RESULT_RESPONSE WebSocketSession::handleDELETErequest(const json& input)
{
    ACTION_RESULT result = database.checkExistanceAndPermission(current_file_id, current_user_id); 
    if(result == PERMISSION_WRITE){
        result = database.deleteFile(current_file_id);
    }
    return {result, result};
}

/*
         - - - - -
 */

WebSocketServer::WebSocketServer(asio::io_context &ioc, tcp::endpoint endpoint): acceptor_(ioc, endpoint), thread_pool_(4)
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
        std::make_shared<WebSocketSession>(std::move(socket), thread_pool_)->run();
    }
    // listen for the next connection
    doAccept();
}
