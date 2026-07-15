#include "../inc/postgresql.h"

PostgresqlWorker::PostgresqlWorker():
connection("user=postgres password=admin host=localhost port=5432 dbname=practice target_session_attrs=read-write"){}

ACTION_RESULT PostgresqlWorker::logUserPwd(std::string username, std::string pwd)
{
    pqxx::work w(connection);
    pqxx::result r = w.exec("SELECT user_password FROM users WHERE user_nickname = \'" +username+"\'");

    if(r[0][0].is_null()){
        return USER_NOT_FOUND;
    }
    std::string true_pwd = r[0][0].c_str();
    if(true_pwd == pwd){
        return SUCCESS;
    }
    else{
        return WRONG_PASSWORD;
    }
}

ACTION_RESULT PostgresqlWorker::signUserPwd(std::string username, std::string password)
{
    pqxx::work w(connection);
    try{
        pqxx::result r = w.exec("INSERT INTO users (user_nickname, user_password) VALUES (\'"+username+"\', \'"+password+"\')");
        w.commit();    
    }
    catch(std::exception e){
        return USERNAME_TAKEN;
    }
    return SUCCESS;
}

ACTION_RESULT PostgresqlWorker::checkExistanceAndPermission(uint32_t file_id, uint32_t user_id)
{
    return ACTION_RESULT();
}

uint32_t PostgresqlWorker::getUserId(std::string username)
{
    return 0;
}

uint32_t PostgresqlWorker::getFileIdByFilename_UserID(std::string filename, uint32_t user_id)
{
    return 0;
}

ACTION_RESULT PostgresqlWorker::changeAccessLevelForUser(uint32_t file_id, std::string username, int shared_level)
{
    return ACTION_RESULT();
}

ACTION_RESULT PostgresqlWorker::deleteFile(uint32_t file_id)
{
    return ACTION_RESULT();
}

ACTION_RESULT PostgresqlWorker::renameFile(uint32_t file_id, std::string newName)
{
    return ACTION_RESULT();
}

ACTION_RESULT PostgresqlWorker::registerFile(std::string newName)
{
    pqxx::work w(connection);
    try{
        pqxx::result r = w.exec("INSERT INTO documents (newName) VALUES (\'"+newName+"\')");
        w.commit();    
    }
    catch(std::exception e){
        return FAILURE;
    }
    return SUCCESS;
}
