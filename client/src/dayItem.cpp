#include "../inc/dayItem.h"

DayItemModel::DayItemModel(QObject *parent): QAbstractTableModel(parent)
{   
    gridData.reserve(5);
    QVector<DayItem> row;
    row.reserve(8);
    for(int i = 1; i <= 5; i++){
        row.clear();
        for(int j = 1; j <= 7; j++){
            row.push_back(DayItem(0,0,0,0, "", false));
        } 
        gridData.push_back(row);
    }

    connect(&DayDataHandler::instance(), &DayDataHandler::dayDataChanged, this, [this](quint32 ymd) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 7; j++) {
                int curYMD = gridData[i][j].year * 10000 + gridData[i][j].month * 100 + gridData[i][j].day;
                
                if (curYMD == ymd) {
                    gridData[i][j].content = DayDataHandler::instance().getContentByYMD(ymd);

                    QModelIndex topLeft = index(i, j);
                    QModelIndex bottomRight = index(i, j);
                    
                    emit dataChanged(topLeft, bottomRight);
                
                }
            }
        }
    });
}

Q_INVOKABLE void DayItemModel::loadMonth(int year, int month)
{
    beginResetModel();

    QDate firstDayOfMonth(year, month, 1);
    int startDayOfWeek = firstDayOfMonth.dayOfWeek();
    int amountOfDaysCurrent = firstDayOfMonth.daysInMonth();

    QDate prevMonth = firstDayOfMonth.addMonths(-1);
    int amountOfDaysPrev = prevMonth.daysInMonth();

    int dayCounter = 1;
    int nextMonthDayCounter = 1;

    for(int row = 0; row < 5; row++){
        for(int col = 0; col < 7; col++){
            int ind = row*7 + col;
            int indexInGrid = row * 7 + col;
            u_int32_t d_day = 0;
            u_int32_t d_month = month;
            u_int32_t d_year = year;
            bool isCurrent = true;

            if (indexInGrid < startDayOfWeek - 1) {
                // Previous 
                d_day = amountOfDaysPrev - (startDayOfWeek - 2) + indexInGrid;
                d_month = prevMonth.month();
                d_year = prevMonth.year();
                isCurrent = false;
            } else if (dayCounter <= amountOfDaysCurrent) {
                // Current 
                d_day = dayCounter++;
            } else {
                // Next 
                d_day = nextMonthDayCounter++;
                QDate nextMonthDate = firstDayOfMonth.addMonths(1);
                d_month = nextMonthDate.month();
                d_year = nextMonthDate.year();
                isCurrent = false;
            }
            u_int32_t ymd = d_year * 10000 + d_month * 100 + d_day;
            QString content = DayDataHandler::instance().getContentByYMD(ymd);
            if(content.size() > 0){
                int a = 133;
            }
            uint32_t dayOfWeek = QDate(d_year, d_month, d_day).dayOfWeek();
            gridData[row][col] = {d_day, d_month, d_year, dayOfWeek, content, isCurrent};
        }
    }
    endResetModel();
}

int DayItemModel::rowCount(const QModelIndex &parent) const { return 5; }
int DayItemModel::columnCount(const QModelIndex &parent) const{return 7;}

QVariant DayItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    const auto& tx =  gridData[index.row()][index.column()];
    switch (role){
        case dayRole:         return tx.day;
        case monthRole:       return tx.month;
        case yearRole:        return tx.year;
        case dayOfWeekRole:   return tx.dayOfWeek;
        case contentRole:     return tx.content;
        case isCurrentMonthRole:     return tx.currentMonth;
        default:return QVariant();
    }
}

QHash<int, QByteArray> DayItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[dayRole] = "day";
    roles[monthRole] = "month";
    roles[yearRole] = "year";
    roles[dayOfWeekRole] = "dayOfWeek";
    roles[contentRole] = "content";
    roles[isCurrentMonthRole] = "isCurrent";
    return roles;
}

DayItem::DayItem(uint32_t day, uint32_t month, uint32_t year, uint32_t dayOfWeek, QString content, bool currentMonth) 
: day(day), month(month), year(year), content(content), dayOfWeek(dayOfWeek), currentMonth(currentMonth) {}