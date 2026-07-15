#include "../inc/documentHandler.h"
#include "../inc/resourceManager.h"

DocumentHandler::DocumentHandler()
{data = {};}

ACTION_RESULT DocumentHandler::saveDataOnServer(json data)
{
    try{
        this->data = data;
        return SUCCESS;
    }
    catch(std::exception e){
        return FAILURE;
    }
}

ACTION_RESULT DocumentHandler::saveDataOnFile()
{
    return ACTION_RESULT();
}

ACTION_RESULT DocumentHandler::saveDayData(const json& dayData)
{
    std::string YMD = dayData["YMD"];
    std::string content = dayData["content"];
    data[YMD] = content;

}

ACTION_RESULT DocumentHandler::setFileId(uint32_t file_id)
{
    this->file_id = file_id;
}

ACTION_RESULT DocumentHandler::addUser(uint32_t user_id, ACCESS_LEVEL accessLevel)
{
    if(std::find(owners_id.begin(), owners_id.end(), user_id) != owners_id.end()){

    }
}

uint32_t DocumentHandler::getFileId() const {return file_id;}

json DocumentHandler::getData(){return data;}

std::string DocumentHandler::getDataByYMD(std::string YMD){return data[YMD];}

bool DocumentHandler::isSaved() const
{
    return changesSaved;
}

bool DocumentHandler::setSaved(bool newSaved)
{
    return changesSaved = newSaved;
}
