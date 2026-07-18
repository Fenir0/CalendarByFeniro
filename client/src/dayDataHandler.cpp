#include "../inc/dayDataHandler.h"
#include "../inc/appState.h"
#include "../inc/requestHandler.h"
#include "dayDataHandler.h"

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

bool DayDataHandler::isDaySaved(quint32 y_m_d)
{
    return currentDataMap.find(y_m_d) != currentDataMap.end() && currentDataMap[y_m_d].length() > 0 ;
}
void DayDataHandler::saveCurrentStateIntoFile(const std::string &filename)
{

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

json DayDataHandler::getDataMapAsJSON()
{
    json data;
    for(auto it = currentDataMap.begin(); it != currentDataMap.end(); it++){
        if(!it->second.toStdString().empty()) 
            data [QString::number(it->first).toStdString()] = it->second.toStdString();
    }
    return data;
}

void DayDataHandler::updateDataMapFromJSON(const json& data)
{
    std::map<std::string, std::string> newDataMap = data.get<std::map<std::string, std::string>>();
    for(const auto& [key, value]: newDataMap){
        currentDataMap[static_cast<quint32>(std::stoul(key))] = QString::fromStdString(value);
    }
    emit newDataSet();
}

void DayDataHandler::setDataMapFromJSON(const json& data){
    currentDataMap.clear();
    std::map<std::string, std::string> newDataMap = data.get<std::map<std::string, std::string>>();
    for(const auto& [key, value]: newDataMap){
        currentDataMap[static_cast<quint32>(std::stoul(key))] = QString::fromStdString(value);
    }
    emit newDataSet();
}
