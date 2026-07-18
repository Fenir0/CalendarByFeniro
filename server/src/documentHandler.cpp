#include "../inc/documentHandler.h"
#include "../inc/resourceManager.h"
#include <system_error>


DocumentHandler::DocumentHandler(boost::asio::io_context& ioc)
    : file_id(0), changesSaved(false), saveTimer(ioc), editTimer(ioc)
{data = json::object();}

void DocumentHandler::saveDataOnCreate(const json& data)
{
    try{
        std::string s = data.dump();
        this->data = data;
        saveDataOnFile();
    }
    catch(std::exception e){

    }
}

void DocumentHandler::saveDataOnFile()
{
    try{
        ResourceManager::saveAllIntoFile(std::to_string(creator_id), std::to_string(file_id), data);
    }
    catch (std::exception e){

    }

}

void DocumentHandler::saveDataOnServer(const json& data)
{
    this->data = data;
    std::string r = data.dump();
}

void DocumentHandler::updateDataOnServer(const json &data)
{
    std::map<std::string, std::string> newData = data.get<std::map<std::string, std::string>>();
    
    for(const auto& [key, value]: newData){
        this->data[key] = value;
    }

    changesSaved = false;
}

void DocumentHandler::resetFile()
{
    file_id = 0;
    creator_id = 0;
    data = {};
}

void DocumentHandler::setNewFile(uint32_t file_id, uint32_t creator_id, std::string filename, json data) 
{
    this->file_id = file_id;
    this->creator_id = creator_id;
    if(!data.empty()) this->data = data;
    else this->data = {};
}

void DocumentHandler::setFileId(uint32_t file_id) 
{
    this->file_id = file_id;
}

uint32_t DocumentHandler::getFileId() const {return file_id;}

json DocumentHandler::getData(){return data;}

std::string DocumentHandler::getDataByYMD(std::string YMD){return data[YMD];}

bool DocumentHandler::isSaved() const{return changesSaved;}

void DocumentHandler::setCreatorId(uint32_t creatorId){creator_id = creatorId;}
uint32_t DocumentHandler::getCreatorId(){return creator_id;}

bool DocumentHandler::setSaved(bool newSaved){return changesSaved = newSaved;}

void DocumentHandler::saveOnEdit()
{
    changesSaved = false;

    editTimer.cancel();
    editTimer.expires_after(std::chrono::seconds(editSaveDelay));
    editTimer.async_wait([this](const boost::system::error_code& ec){
        if(!ec && !changesSaved){
            saveDataOnServer(data);
            changesSaved = true;
        }
    });
}

void DocumentHandler::startPeriodicSave()
{
    scheduleNextSave();
}

void DocumentHandler::stopPeriodicSave()
{
    saveTimer.cancel();
    editTimer.cancel();
    if(!changesSaved){
        saveDataOnServer(data);
        saveDataOnFile();
    }
}

void DocumentHandler::scheduleNextSave()
{
    saveTimer.expires_after(saveInterval);
    saveTimer.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            performPeriodicSave();
            changesSaved = true;
        }
        scheduleNextSave(); 
    });
}

void DocumentHandler::performPeriodicSave()
{
    if(!changesSaved && file_id != 0){
        saveDataOnServer(data);
        saveDataOnFile();
        std::cout << "Saved " << file_id << '\n';
    }
}
