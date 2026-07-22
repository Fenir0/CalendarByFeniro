#ifndef DAY_DATA_HANDLER_H
#define DAY_DATA_HANDLER_H

#include <QObject>
#include <QtQml/QQmlEngine> 
#include <qqml.h>

#include <string>
#include <fstream>

#include "appState.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// ====================
// APP DATA CONTROLLER
// ====================

class DayDataHandler: public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    static DayDataHandler& instance();
    DayDataHandler();
    DayDataHandler(const std::string& filename);

    Q_INVOKABLE void        setContentByYMD(quint32 y_m_d, QString content);
    Q_INVOKABLE QString     getContentByYMD(quint32 y_m_d);

    Q_INVOKABLE bool isDaySaved(quint32 y_m_d);

    Q_INVOKABLE void setEmptyJSON();    
    Q_INVOKABLE json getEmptyJSON();
    
    Q_INVOKABLE json getDataMapAsJSON();
    void             updateDataMapFromJSON(const json& data);
    void             setDataMapFromJSON(const json& data);

signals:
    void newDataSet();
    void dayDataChanged(quint32 y_m_d);
private:
    std::map<quint32, QString> currentDataMap;
};

#endif