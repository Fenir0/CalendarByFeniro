#include "../inc/dayDataHandler.h"

DayDataHandler::DayDataHandler(std::string filename){
}

Q_INVOKABLE void DayDataHandler::setContentByYMD(u_int32_t y_m_d, std::string content)
{
    currentDataMap.set(y_m_d, content);
}

Q_INVOKABLE std::string DayDataHandler::getContentByYMD(u_int32_t y_m_d)
{
    return currentDataMap.get(y_m_d);
}

void DayDataHandler::saveCurrentStateIntoFile(std::string filename)
{
    //json entry = {y_m_d, content}
}

void DayDataHandler::loadCurrentStateFromFile(std::string filename)
{
    try{
         std::ifstream f(filename);
         json data = json::parse(f);
         //for i in data
         // currentDataMap.set(y_m_d, content);
    }
    catch (std::exception e){

    }
}
