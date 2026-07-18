#include "../inc/sessionWatcher.h"
#include "../inc/webServerSocket.h"

void SessionWatcher::add(std::shared_ptr<WebSocketSession> session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_[session->getId()] = session;
}

void SessionWatcher::remove(SESSION_ID session_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session_id);
}

void SessionWatcher::sendTo(const std::set<SESSION_ID> &ids, const json &message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for(auto it: ids){
        if(sessions_.find(it) != sessions_.end()){
            sessions_[it]->sendResponse(message);
        }
    }
}

SessionWatcher &SessionWatcher::instance()
{
    static SessionWatcher sessionWatcher;
    return sessionWatcher;
}
