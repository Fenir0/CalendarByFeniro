#ifndef DAYITEM_H
#define DAYITEM_H

#include <QString>
#include <QAbstractTableModel>
#include <QtQml/QQmlEngine> 

#include "dayDataHandler.h"
#include "appState.h"
#include <qqml.h>

/*
VISUAL ELEMENTS OF THE GRID
INDEPENDENT OF APP DATA
*/

struct DayItem
{
    DayItem(uint32_t day, uint32_t month, uint32_t year, uint32_t dayOfWeek, QString content = "", bool currentMonth = true);
    uint32_t day;
    uint32_t month;
    uint32_t year;
    uint32_t dayOfWeek;
    QString content;
    bool currentMonth;
};

enum DayItemRoles{
    dayRole = Qt::UserRole + 1,
    monthRole,
    yearRole,
    dayOfWeekRole,
    contentRole,
    isCurrentMonthRole
};

class DayItemModel: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    public:
    explicit DayItemModel(QObject* parent = nullptr);
    Q_INVOKABLE void loadMonth(int year, int month);
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const ;
    
    private:
    QVector<QVector<DayItem>> gridData;

};

#endif

