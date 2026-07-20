#include "../inc/fileWatcher.h"

void FileWatcher::subscribe(FILE_ID file_id, SESSION_ID session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(subscribers_.find(file_id) == subscribers_.end()){
        subscribers_[file_id] = {};
    }
    subscribers_[file_id].insert(session);
}

void FileWatcher::unsubscribe(FILE_ID file_id, SESSION_ID session)
{
    if(file_id == 0) return;
    std::lock_guard<std::mutex> lock(mutex_);
    if(subscribers_.find(file_id) != subscribers_.end()){
        if(subscribers_[file_id].find(session) != subscribers_[file_id].end()){
            subscribers_[file_id].erase(session);
        }
        if(subscribers_[file_id].size() == 0){
            subscribers_.erase(file_id);
        }
    }
}

void FileWatcher::unsubscribeAll(SESSION_ID session)
{
    for(auto it: subscribers_){
        if(it.second.find(session) != it.second.end()){
            it.second.erase(session);
        }
    }
}

void FileWatcher::notifyChanged(FILE_ID file_id, const json &data, SESSION_ID current_session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(subscribers_.find(file_id) != subscribers_.end()){
        SessionWatcher::instance().sendTo(subscribers_[file_id], data, current_session);
    } 
}

FileWatcher &FileWatcher::instance()
{
    static FileWatcher fileWatcher;
    return fileWatcher;
}
