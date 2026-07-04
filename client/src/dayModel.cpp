#include "../inc/dayModel.h"

DayModel::DayModel(QObject *parent)
    : QObject{parent}
{
}

Q_INVOKABLE void DayModel::setContent(QString content)
{
    return Q_INVOKABLE void();
}

Q_INVOKABLE QString DayModel::getContent()
{
    return Q_INVOKABLE content;
}