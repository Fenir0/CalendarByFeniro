#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include <iostream>
#include <pqxx/pqxx>

enum ACTION_RESULT{
    SUCCESS,
    FAILURE,

    USERNAME_TAKEN,
    WRONG_PASSWORD,
    FORBIDDEN_SYMBOLS,
    
    USER_NOT_FOUND,
    FILE_NOT_FOUND,

    FILE_NOT_CHOSEN,

    PERMISSION_WRITE,
    PERMISSION_READ,
    PERMISSION_DENIED,

    DATA_STREAM
};

enum ACCESS_LEVEL{
    EDIT,
    READ
};

class PostgresqlWorker{
    public:
    PostgresqlWorker();
    
    ACTION_RESULT logUserPwd (std::string username, std::string password);
    ACTION_RESULT signUserPwd(std::string username, std::string password);

    ACTION_RESULT checkExistanceAndPermission(uint32_t file_id,  uint32_t user_id);

    uint32_t                  getUserId(std::string username);
    uint32_t getFileIdByFilename_UserID(std::string filename, uint32_t user_id);

    ACTION_RESULT changeAccessLevelForUser(uint32_t file_id, std::string username, int shared_level);
    ACTION_RESULT deleteFile              (uint32_t file_id);
    ACTION_RESULT renameFile              (uint32_t file_id, std::string newName);
    ACTION_RESULT registerFile            (std::string newName);

    private:
    pqxx::connection connection;

};


#endif