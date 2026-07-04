#ifndef DAYMODEL_H
#define DAYMODEL_H

#include <QObject>
#include <QtQml/QQmlEngine> 
#include <qqml.h>

class DayModel: public QObject{
    Q_OBJECT
    QML_ADDED_IN_VERSION(1, 1)
    QML_ELEMENT
public:
    explicit DayModel(QObject *parent = nullptr);

    Q_INVOKABLE void setContent (QString content);
    Q_INVOKABLE QString getContent();

private:
    QString content;
};

#endif

