#include "../inc/dayItem.h"

DayItemModel::DayItemModel(QObject *parent): QAbstractTableModel(parent)
{
    QVector<DayItem> row;
    row.reserve(8);
    for(int i = 1; i <= 5; i++){
        row.clear();
        for(int j = 1; j <= 7; j++){
            QString cur = DayDataHandler::instance().getContentByYMD(AppState::instance().getDateAsYM()+(i-1)*7+(j-1));
            row.push_back(DayItem(j, cur));
        }
        gridData.push_back(row);
    }
    connect(&DayDataHandler::instance(), &DayDataHandler::dataChanged, this, [this](int ymd) {
        emit dataChanged(); 
});
}

int DayItemModel::rowCount(const QModelIndex &parent) const{return 5;}
int DayItemModel::columnCount(const QModelIndex &parent) const{return 7;}

QVariant DayItemModel::data(const QModelIndex &index, int role) const
{
    const auto& tx =  gridData[index.row()][index.column()];
    switch (role){
        case dayRole:         return tx.day;
        case contentRole:     return tx.content;
        default:return QVariant();
    }
}

QHash<int, QByteArray> DayItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[dayRole] = "day";
    roles[contentRole] = "content";
    return roles;
}

DayItem::DayItem(int day, QString content) : day(day), content(content) {}