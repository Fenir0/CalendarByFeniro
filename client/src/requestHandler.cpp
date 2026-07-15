#include "../inc/requestHandler.h"
#include <QMetaObject>
#include <QJSValue>
#include <QJSValueList>

ACTION_RESULT getActionResultFromString(const std::string& action_result){
    if(action_result == "SUCCESS") return SUCCESS;
    if(action_result == "FAILURE") return FAILURE;

    if(action_result == "USERNAME TAKEN") return USERNAME_TAKEN;
    if(action_result == "WRONG PASSWORD") return WRONG_PASSWORD;
    if(action_result == "FORBIDDEN SYMBOLS") return FORBIDDEN_SYMBOLS;
    
    if(action_result == "USER NOT FOUND") return USER_NOT_FOUND;
    if(action_result == "FILE NOT FOUND") return FILE_NOT_FOUND;

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