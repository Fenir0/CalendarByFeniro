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
    QML_SINGLETON
    Q_PROPERTY(Map_DayData currentDataMap NOTIFY dataChanged)
    public:
    static DayDataHandler& instance();
    DayDataHandler();
    DayDataHandler(std::string filename);
    Q_INVOKABLE void        setContentByYMD(u_int32_t y_m_d, QString content);
    Q_INVOKABLE QString     getContentByYMD(u_int32_t y_m_d);

    void saveCurrentStateIntoFile(std::string filename);
    void loadCurrentStateFromFile(std::string filename);
    private:
    Map_DayData currentDataMap;

    signals:
    void dataChanged(int y_m_d = -1);
};

#endif