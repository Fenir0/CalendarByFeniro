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
}

void DocumentHandler::saveDayData(const json &dayData)
{
    std::string YMD = dayData["YMD"];
    std::string content = dayData["content"];
    data[YMD] = content;
}

void DocumentHandler::setNewFile(uint32_t file_id, std::string filename, json data)
{
}

void DocumentHandler::setFileId(uint32_t file_id) 
{
    this->file_id = file_id;
}

void DocumentHandler::addUser(uint32_t user_id, ACCESS_LEVEL accessLevel)
{
    if(std::find(owners_id.begin(), owners_id.end(), user_id) != owners_id.end()){

    }
}

uint32_t DocumentHandler::getFileId() const {return file_id;}

json DocumentHandler::getData(){return data;}

std::string DocumentHandler::getDataByYMD(std::string YMD){return data[YMD];}

bool DocumentHandler::isSaved() const{return changesSaved;}

void DocumentHandler::setCreatorId(uint32_t creatorId){creator_id = creatorId;}
uint32_t DocumentHandler::getCreatorId(){return creator_id;}

bool DocumentHandler::setSaved(bool newSaved){return changesSaved = newSaved;}

void DocumentHandler::onEdit()
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
        //saveDataOnServer();
        saveDataOnFile();
        std::cout << "Saved " << file_id << '\n';
    }
}
