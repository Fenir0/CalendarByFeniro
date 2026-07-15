#include "../inc/dayDataHandler.h"
#include "../inc/appState.h"

DayDataHandler &DayDataHandler::instance()
{
    static DayDataHandler instance;
    return instance;
}

DayDataHandler::DayDataHandler(){}
DayDataHandler::DayDataHandler(const std::string& filename){
    try{
         std::ifstream f(filename);
         json data = json::parse(f);
         //for i in data
         // currentDataMap.set(y_m_d, content);
    }
    catch (std::exception e){

    }
}

Q_INVOKABLE void DayDataHandler::setContentByYMD(quint32 y_m_d, QString content)
{

    currentDataMap[y_m_d] = content;
    emit dayDataChanged(y_m_d);
}

Q_INVOKABLE QString DayDataHandler::getContentByYMD(quint32 y_m_d)
{
    return currentDataMap[y_m_d];
}

void DayDataHandler::saveCurrentStateIntoFile(const std::string& filename)
{
    //json entry = {y_m_d, content}
}

void DayDataHandler::loadCurrentStateFromFile(const std::string& filename)
{
    try{
         std::ifstream f(filename);
         json data = json::parse(f);
         //for i in data
         // currentDataMap.set(y_m_d, content);
         //emit dayDataChanged(y);
    }
    catch (std::exception e){

    }
}
