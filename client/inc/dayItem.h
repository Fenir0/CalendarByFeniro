#ifndef DAYITEM_H
#define DAYITEM_H

#include <QString>
#include <QAbstractTableModel>
#include <QtQml/QQmlEngine> 

#include <qqml.h>

/*
VISUAL ELEMENTS OF THE GRID
INDEPENDENT OF APP DATA
*/

struct DayItem
{
    DayItem(int day);
    int day;
};

enum DayItemRoles{
    dayRole = Qt::UserRole + 1,
};

class DayItemModel: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    public:
    explicit DayItemModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const ;
    
    private:
    QVector<QVector<DayItem>> gridData;
};

#endif

