#include "../inc/requestHandler.h"
#include <QMetaObject>
#include <QJSValue>
#include <QJSValueList>
#include "../inc/dayDataHandler.h"

ACTION_RESULT getActionResultFromString(const std::string& action_result){
    if(action_result == "SUCCESS") return SUCCESS;
    if(action_result == "FAILURE") return FAILURE;

    if(action_result == "USERNAME TAKEN") return USERNAME_TAKEN;
    if(action_result == "FILENAME TAKEN") return FILENAME_TAKEN;
    if(action_result == "WRONG PASSWORD") return WRONG_PASSWORD;
    if(action_result == "FORBIDDEN SYMBOLS") return FORBIDDEN_SYMBOLS;
    
    if(action_result == "USER NOT FOUND") return USER_NOT_FOUND;
    if(action_result == "FILE NOT FOUND") return FILE_NOT_FOUND;
    if(action_result == "FILE NOT CHOSEN") return FILE_NOT_CHOSEN;

    if(action_result == "PERMISSION WRITE") return PERMISSION_WRITE;
    if(action_result == "PERMISSION READ") return PERMISSION_READ;
    if(action_result == "PERMISSION DENIED") return PERMISSION_DENIED;
}

void RequestHandler::login(const QString &username, const QString &pwd, QJSValue callback)
{
    json request;
    request["action"] = "LOGIN";
    request["username"] = username.toStdString();
    request["password"] = pwd.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                AppState::instance().setUsername(QString::fromStdString(response["username"]));
                AppState::instance().setUserId(response["user_id"]);
                AppState::instance().setLoggedIn(true);
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Log in successful";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case WRONG_PASSWORD:

                break;
            case USER_NOT_FOUND:

                break;
            }
        }, Qt::QueuedConnection);
    });
}
Q_INVOKABLE void RequestHandler::signup(const QString &username, const QString &pwd, QJSValue callback)
{
    json request;
    request["action"] = "SIGNUP";
    request["username"] = username.toStdString();
    request["password"] = pwd.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                AppState::instance().setUsername(QString::fromStdString(response["username"]));
                AppState::instance().setUserId(response["user_id"]);
                AppState::instance().setLoggedIn(true);
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Sign up successful";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case USERNAME_TAKEN:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "Username taken";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}
Q_INVOKABLE void RequestHandler::logout(QJSValue callback)
{
    json request;
    request["action"] = "LOGOUT";
    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                AppState::instance().setUsername("");
                AppState::instance().setUserId(-1);
                AppState::instance().setLoggedIn(false);
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Log out successful";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}
Q_INVOKABLE void RequestHandler::update(const quint32 ymd, QString content, QJSValue callback)
{
    json request;
    request["action"] = "UPDATE";
    uint32_t data_ymd = ymd;
    std::string data_content = content.toStdString();
    request[ymd] = data_content;

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Updated successully";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::create(const QString& filename, const json &data, QJSValue callback)
{
    json request;
    request["action"] = "CREATE";
    request["filename"] = filename.toStdString();
    request["data"] = data;
    std::string datadump = data.dump();
    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Saved successully";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case FILENAME_TAKEN:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Document with this name already exists. Overwrite?";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case FAILURE:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Error";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::loadList(QJSValue callback)
{
    json request;
    request["action"] = "LOADLIST";
    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                if(callback.isCallable()){
                    json data = response["data"];
                    std::string cstr = data.dump();
                    QString qstr = QString::fromStdString(cstr);

                    QJSValueList args;
                    args << true <<  qstr;
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::loadFromServer(const QString &filename, QJSValue callback)
{
    json request;
    request["action"] = "LOAD";
    request["filename"] = filename.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                json newData = result["data"];
                DayDataHandler::instance().setDataMapFromJSON(newData);

                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Loaded successully";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::share(const std::string &documentname, const std::string &username, int accesslevel, QJSValue callback)
{
    return Q_INVOKABLE void();
}

Q_INVOKABLE void RequestHandler::loadDocumentList(QJSValue callback)
{
    return Q_INVOKABLE void();
}
