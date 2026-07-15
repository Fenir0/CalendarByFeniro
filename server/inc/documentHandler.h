#ifndef DOCUMENT_HANDLER_H
#define DOCUMENT_HANDLER_H

#include <memory>
#include <string>
#include <mutex>
#include "postgresql.h"
#include <unordered_set>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class DocumentHandler{
public:
    DocumentHandler();
    ACTION_RESULT saveDataOnServer(json data);
    ACTION_RESULT saveDataOnFile();

    ACTION_RESULT saveDayData(const json& dayData);

    ACTION_RESULT setFileId(uint32_t file_id);
    ACTION_RESULT addUser(uint32_t user_id, ACCESS_LEVEL accessLevel);

    uint32_t getFileId() const;
    json getData();
    std::string getDataByYMD(std::string YMD);

    bool isSaved() const;
    bool setSaved(bool newSaved);

private:
    uint32_t file_id;
    std::unordered_set<uint32_t> owners_id;
    std::unordered_set<uint32_t> readers_id;
    json data;
    bool changesSaved = false;
};

#endif