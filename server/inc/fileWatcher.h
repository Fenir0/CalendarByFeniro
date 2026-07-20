#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H

#include<map>
#include<set>
#include <mutex>
#include <unordered_map>
#include <cstdint>     
#include<nlohmann/json.hpp>

using json = nlohmann::json;

#include "webServerSocket.h"
#include "sessionWatcher.h"

using FILE_ID = uint32_t;
using SESSION_ID = uint32_t;


class FileWatcher{
public:
    void subscribe(FILE_ID file_id, SESSION_ID session);
    void unsubscribe(FILE_ID file_id, SESSION_ID session);
    void unsubscribeAll(SESSION_ID session);

    void notifyChanged(FILE_ID file, const json& data, SESSION_ID current_session = 0);

    static FileWatcher& instance();

private:
    std::unordered_map<FILE_ID, std::set<SESSION_ID>> subscribers_;
    std::mutex mutex_;
};

#endif