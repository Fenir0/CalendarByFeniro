#include "../inc/dayItem.h"

DayItemModel::DayItemModel(QObject *parent): QAbstractTableModel(parent)
{
    QVector<DayItem> row;
    row.reserve(8);
    for(int i = 1; i <= 5; i++){
        row.clear();
        for(int j = 1; j <= 7; j++){
            // POPULATE WITH DATA HERE
            row.push_back(DayItem(j));
        }
        gridData.push_back(row);
    }
}

int DayItemModel::rowCount(const QModelIndex &parent) const{return 5;}
int DayItemModel::columnCount(const QModelIndex &parent) const{return 7;}

QVariant DayItemModel::data(const QModelIndex &index, int role) const
{
    const auto& tx =  gridData[index.row()][index.column()];
    switch (role){
        case dayRole:     return tx.day;
        default:return QVariant();
    }
}

QHash<int, QByteArray> DayItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[dayRole] = "day";
    return roles;
}

DayItem::DayItem(int day) : day(day) {}