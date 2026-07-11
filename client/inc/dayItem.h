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
    DayItem(int day, QString content = "");
    int day;
    QString content;
};

enum DayItemRoles{
    dayRole = Qt::UserRole + 1,
    contentRole = dayRole + 1
};

class DayItemModel: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(NOTIFY dataChanged)
    public:
    explicit DayItemModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const ;
    
    private:
    QVector<QVector<DayItem>> gridData;

    signals:
    void dataChanged();
};

#endif

