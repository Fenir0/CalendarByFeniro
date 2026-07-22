#include "../inc/requestHandler.h"
#include <QMetaObject>
#include <QJSValue>
#include <QJSValueList>
#include "../inc/dayDataHandler.h"
#include "requestHandler.h"

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
    return FAILURE;
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
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "pwd";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case USER_NOT_FOUND:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "user";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
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
                    args << false << "user";
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
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::rename(const QString filename, quint32 fileId, QJSValue callback)
{
    json request, tmp;
    request["action"] = "RENAME";
    request["file_id"] = fileId;
    request["filename"] = filename.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            std::string newname = response.value("name", "");
            switch(result){
            case SUCCESS:
                AppState::instance().setDocumentName(QString::fromStdString(newname));
                if(callback.isCallable()){
                    QJSValueList args;
                    args << true << "Renamed successully";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case FILENAME_TAKEN:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "name";
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
    json request, tmp;
    request["action"] = "UPDATE";
    uint32_t data_ymd = ymd;
    std::string data_content = content.toStdString();
    tmp[std::to_string(data_ymd)] = data_content;
    request["data"] = tmp;
    std::string ch = request.dump();
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
                    AppState::instance().setDocumentName(QString::fromStdString(response["name"]));
                    AppState::instance().setDocumentId(response["file_id"]);
                    AppState::instance().setAccessLevel(response["access_level"]);
                    QJSValueList args;
                    args << true << "Saved successully";
                    emit userListChanged();
                    emit documentListChanged();
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                    
                }
                break;
            case FILENAME_TAKEN:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "name";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            case FAILURE:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "Error";
                    QJSValue calresult = callback.call(args);
                   QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::deleteFile(const QString &filename, quint32 fileId, QJSValue callback)
{
    json request, tmp;
    request["action"] = "DELETE";
    request["file_id"] = fileId;
    request["filename"] = filename.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            switch(result){
            case SUCCESS:
                if(callback.isCallable()){
                    if(response.value("file_id", 0) == AppState::instance().getDocumentId()){
                        AppState::instance().setDocumentName("");
                        AppState::instance().setDocumentId(0);
                        AppState::instance().setAccessLevel(READ);
                    }


                    QJSValueList args;
                    emit documentListChanged();
                    args << true << "Removed successully";
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

Q_INVOKABLE void RequestHandler::loadListOfPeople(quint32 fileId, QJSValue callback)
{
    json request;
    request["action"] = "LOADPEOPLE";
    uint32_t file_id = fileId;
    request["file_id"] = file_id;

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

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

Q_INVOKABLE void RequestHandler::loadFromServer(quint32 fileId, const QString &filename, QJSValue callback)
{
    json request;
    request["action"] = "LOAD";
    uint32_t file_id = fileId;
    request["file_id"] = file_id;
    request["filename"] = filename.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        std::string ch = response.dump();

        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);

            AppState::instance().setDocumentName(QString::fromStdString(response["filename"]));
            AppState::instance().setDocumentId(response["file_id"]);
            AppState::instance().setAccessLevel(response["access_level"]);
            
            switch(result){
            case SUCCESS:

                json newData = response["data"];
                if(!newData.empty())
                    DayDataHandler::instance().setDataMapFromJSON(newData);
                else
                    DayDataHandler::instance().setEmptyJSON();
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

Q_INVOKABLE void RequestHandler::share(quint32 file_id, const QString &username, const QString & accesslevel, QJSValue callback)
{
    json request;
    request["action"] = "SHARE";
    request["file_id"] = file_id;
    request["username"] = username.toStdString();
    request["access_level"] = accesslevel.toStdString();

    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            
            switch(result){
            case SUCCESS:
                if(callback.isCallable()){
                    char userDeletion = response.value("deleted",'f');
                    QJSValueList args;
                    if(userDeletion == 't'){
                        args << true << "Excluded successully";
                        emit userListChanged(); 
                    }else{
                        args << true << "Shared successully";
                        if(AppState::instance().getDocumentId() == response["file_id"]);
                            emit userListChanged();   
                    }
                    QJSValue calresult = callback.call(args);
                    QString d = calresult.toString();
                }
                break;
            case USER_NOT_FOUND:
                if(callback.isCallable()){
                    QJSValueList args;
                    args << false << "Nouser";
                    QJSValue calresult = callback.call(args);
                    QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}

Q_INVOKABLE void RequestHandler::unsubscribeFile(QJSValue callback)
{
    json request;
    request["action"] = "UNSUBSCRIBE";
    request["file_id"] = AppState::instance().getDocumentId();


    WebSocketWorker::instance().sendRequest(request, 
    [this, callback](const json& response){
        QMetaObject::invokeMethod(this, [this, callback, response](){
            ACTION_RESULT result = getActionResultFromString(response["result"]);
            
            switch(result){
            case SUCCESS:
                if(callback.isCallable()){
                    QJSValueList args;
                    QJSValue calresult = callback.call(args);
                    QString d = calresult.toString();
                }
                break;
            }
        }, Qt::QueuedConnection);
    });
}
