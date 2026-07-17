#include "../inc/postgresql.h"
#include <format>
#include <string>
#include <vector>


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
    pqxx::params p (username, password);
    try{
        pqxx::result r = w.exec("INSERT INTO users (user_nickname, user_password) VALUES ($1, $2)", p);
        w.commit();    
    }
    catch(std::exception e){
        return USERNAME_TAKEN;
    }
    return SUCCESS;
}

ACTION_RESULT PostgresqlWorker::checkExistanceAndPermission(uint32_t file_id, uint32_t user_id)
{
    try{
        pqxx::work w(connection);
        pqxx::params p (user_id, file_id);
        pqxx::result r = w.exec("SELECT access_level FROM relations WHERE user_id = $1 AND document_id = $2", p);
        if(r.empty()) return FILE_NOT_FOUND;
        int level = r[0][0].as<int>();
        if(level == 0) return PERMISSION_WRITE;
        if(level == 1) return PERMISSION_READ;
        else return PERMISSION_DENIED;
    }
    catch(const std::exception& e){
        return FAILURE;
    }
}

std::vector<std::pair<uint32_t, std::string>> PostgresqlWorker::getVectorOfFilesByUserID(uint32_t user_id)
{
    std::vector<std::pair<uint32_t, std::string>> res;
    pqxx::work w(connection);
    pqxx::result r = w.exec(
"SELECT relations.document_id, documents.document_name  FROM relations, documents \
WHERE relations.document_id = documents.document_id AND relations.user_id = " + std::to_string(user_id));
    for(auto const& row: r){
        res.push_back({std::stoul(row[0].c_str()), row[1].c_str()});
    }
    return res;   
}
uint32_t PostgresqlWorker::getUserId(std::string username)
{
    pqxx::work w(connection);
    pqxx::result r = w.exec("SELECT user_id FROM users WHERE user_nickname = '" + username + "'");
    return std::stoul(r[0][0].c_str());
}

uint32_t PostgresqlWorker::getFileIdByFilename_UserID(std::string filename, uint32_t user_id)
{
    return 0;
}

std::vector<uint32_t> PostgresqlWorker::getOtherListenersOfFile(u_int32_t file_id)
{
    std::vector<uint32_t> res;
    pqxx::work w(connection);
    pqxx::params p(file_id);
    pqxx::result r = w.exec("SELECT user_id FROM relations WHERE file_id = $1", p);
    for (const auto& row: r){
        res.push_back(row[0].as<uint32_t>());
    }
    return res;
}

uint32_t PostgresqlWorker::getFileCreatorId(uint32_t file_id)
{
    pqxx::work w(connection);
    pqxx::params p(file_id);
    pqxx::result r = w.exec("SELECT user_id FROM relations WHERE file_id = $1 AND access_level_id = 0", p);

    return r[0][0].as<uint32_t>();
}
ACTION_RESULT PostgresqlWorker::changeAccessLevelForUser(uint32_t file_id, std::string username, int shared_level)
{
    uint32_t user_id = getUserId(username);
    pqxx::work w(connection);
    pqxx::params p (shared_level, user_id, file_id);
    if(checkExistanceAndPermission(file_id, user_id) == FILE_NOT_FOUND){
        pqxx::result r = w.exec("INSERT INTO relations (user_id, document_id, access_level_id) VALUES \
             ($2, $3, $1) ON CONFLICT (user_id, document_id) DO UPDATE SET access_level_id = EXCLUDED.access_level_id", p);
        w.commit();
        return SUCCESS;
    }
    else{
        pqxx::result r = w.exec("UPDATE relations SET access_level_id = $1 WHERE user_id = $2 AND document_id = $3", p);
        w.commit();
        return SUCCESS;
    }
}

ACTION_RESULT PostgresqlWorker::deleteFile(uint32_t file_id)
{
    return ACTION_RESULT();
}

ACTION_RESULT PostgresqlWorker::renameFile(uint32_t file_id, std::string newName)
{
    return ACTION_RESULT();
}


uint32_t PostgresqlWorker::registerFile(std::string newName, uint32_t user_id)
{
    uint32_t file_id;
    pqxx::work w(connection);
    try{
        pqxx::params p (newName);
        pqxx::result r = w.exec("insert into documents (document_name, last_edit_date) VALUES \
            ($1, CURRENT_DATE) returning document_id", p);
        
        file_id =  r[0][0].as<uint32_t>();
        p = pqxx::params(user_id, file_id);

        w.exec("insert into relations (user_id, document_id, access_level_id) VALUES"
            "($1, $2, 0)", p);  
        w.commit();
    }
    catch(std::exception e){
        return 0;
    }
    return file_id;
}
