#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <QObject>
#include <QString>
#include <QJSValue>

#include "webSocketWorker.h"
#include "appState.h"

enum ACTION_RESULT{
    SUCCESS,
    FAILURE,

    USERNAME_TAKEN,
    WRONG_PASSWORD,
    FORBIDDEN_SYMBOLS,
    
    USER_NOT_FOUND,
    FILE_NOT_FOUND,

    PERMISSION_WRITE,
    PERMISSION_READ,
    PERMISSION_DENIED,

    DATA_STREAM
};



class RequestHandler: public QObject{
    Q_OBJECT
    public:
    Q_INVOKABLE void login (const QString& username, const QString& pwd, QJSValue callback);    
    Q_INVOKABLE void signup(const QString& username, const QString& pwd, QJSValue callback);   
    Q_INVOKABLE void logout(QJSValue callback); 
    
    Q_INVOKABLE void update(const json& data, QJSValue callback);
    Q_INVOKABLE void   save(const json& data, QJSValue callbask);
    Q_INVOKABLE void loadFromServer(const QString& documentname, QJSValue callback);
    
    Q_INVOKABLE void share(const std::string& documentname, const std::string& username, int accesslevel, QJSValue callback);

    void openDocument();
    private:

    

};

#endif