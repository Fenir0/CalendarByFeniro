#include "../inc/dayDataHandler.h"
#include "dayDataHandler.h"

DayDataHandler &DayDataHandler::instance()
{
    static DayDataHandler instance;
    return instance;
}

DayDataHandler::DayDataHandler(){}
DayDataHandler::DayDataHandler(std::string filename){
    try{
         std::ifstream f(filename);
         json data = json::parse(f);
         //for i in data
         // currentDataMap.set(y_m_d, content);
    }
    catch (std::exception e){

    }
}

Q_INVOKABLE void DayDataHandler::setContentByYMD(u_int32_t y_m_d, QString content)
{
    currentDataMap.set(y_m_d, content.toStdString());
    emit dataChanged(y_m_d);
}

Q_INVOKABLE QString DayDataHandler::getContentByYMD(u_int32_t y_m_d)
{
    return QString::fromStdString(currentDataMap.get(y_m_d));
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
         emit dataChanged();
    }
    catch (std::exception e){

    }
}
