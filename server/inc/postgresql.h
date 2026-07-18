#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include <iostream>
#include <pqxx/pqxx>

enum ACTION_RESULT{
    SUCCESS,
    FAILURE,

    USERNAME_TAKEN,
    FILENAME_TAKEN,
    WRONG_PASSWORD,
    FORBIDDEN_SYMBOLS,
    
    USER_NOT_FOUND,
    FILE_NOT_FOUND,

    FILE_NOT_CHOSEN,

    OWNER,
    PERMISSION_WRITE,
    PERMISSION_READ,
    PERMISSION_DENIED,
    CANT_CHANGE_OWNER_LEVEL,

    DATA_STREAM
};

enum ACCESS_LEVEL{
    CREATOR,
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
    std::string               getUsername(uint32_t id);

    std::vector<std::pair<uint32_t, std::string>> getVectorOfFilesByUserID(uint32_t user_id);
    std::vector<uint32_t> getOtherListenersOfFile(u_int32_t file_id);
    std::vector<std::pair<uint32_t, std::string>> getAllUsersOfFile(uint32_t file_id);
    uint32_t getFileCreatorId(uint32_t file_id);

    ACTION_RESULT changeAccessLevelForUser(uint32_t file_id, std::string username, int shared_level);
    ACTION_RESULT deleteFile              (uint32_t file_id, uint32_t user_id, ACTION_RESULT permission);
    ACTION_RESULT renameFile              (uint32_t file_id, uint32_t user_id, std::string newName);
    uint32_t registerFile            (std::string newName, uint32_t user_id);

    private:
    pqxx::connection connection;

};


#endif