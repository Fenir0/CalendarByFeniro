#ifndef SESSION_WATCHER_H
#define SESSION_WATCHER_H

#include <memory>
#include <unordered_map>
#include <set>
#include <atomic>
#include <mutex>
#include <cstdint> 

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class WebSocketSession;

using SESSION_ID = uint32_t;

class SessionWatcher{
public:
    void add(std::shared_ptr<WebSocketSession> session);
    void remove(SESSION_ID session_id);

    void sendTo(const std::set<SESSION_ID>& ids, const json& message);
    static SessionWatcher& instance();

private:
    std::unordered_map<SESSION_ID, std::shared_ptr<WebSocketSession>> sessions_;
    std::mutex mutex_;
};

#endif