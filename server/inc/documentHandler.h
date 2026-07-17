#ifndef DOCUMENT_HANDLER_H
#define DOCUMENT_HANDLER_H

#include <memory>
#include <string>
#include <mutex>
#include <chrono>
#include "postgresql.h"
#include <unordered_set>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/steady_timer.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace asio = boost::asio;

class DocumentHandler{
public:
    DocumentHandler(asio::io_context& ioc);
    void saveDataOnCreate(const json& data);
    void saveDataOnFile();
    void saveDataOnServer(const json& data);

    void saveDayData(const json& dayData);

    void setNewFile(uint32_t file_id, std::string filename, json data);
    void          setFileId(uint32_t file_id);
    void addUser(uint32_t user_id, ACCESS_LEVEL accessLevel);

    uint32_t    getFileId() const;
    json        getData();
    std::string getDataByYMD(std::string YMD);

    void setCreatorId(uint32_t creatorId);
    uint32_t getCreatorId();

    bool isSaved() const;
    bool setSaved(bool newSaved);

    asio::steady_timer saveTimer;
    std::chrono::seconds saveInterval{30};

    asio::steady_timer editTimer;
    std::chrono::seconds editSaveDelay{5};

    void onEdit();

    void startPeriodicSave();
    void  stopPeriodicSave();

private:
    void scheduleNextSave();
    void performPeriodicSave();

    uint32_t file_id = 0;
    std::uint32_t creator_id;
    std::unordered_set<uint32_t> owners_id;
    std::unordered_set<uint32_t> readers_id;
    json data;
    bool changesSaved = false;
};

#endif