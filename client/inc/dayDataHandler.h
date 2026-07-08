#ifndef DAY_DATA_HANDLER_H
#define DAY_DATA_HANDLER_H

#include <QObject>
#include <QtQml/QQmlEngine> 
#include <qqml.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "dayData.h"

#include <string>
#include <fstream>

class DayDataHandler: public QObject {
    Q_OBJECT
    public:
    DayDataHandler(std::string filename);
    Q_INVOKABLE void        setContentByYMD(u_int32_t y_m_d, std::string content);
    Q_INVOKABLE std::string getContentByYMD(u_int32_t y_m_d);

    void saveCurrentStateIntoFile(std::string filename);
    void loadCurrentStateFromFile(std::string filename);
    private:
    Map_DayData currentDataMap;

};

#endif