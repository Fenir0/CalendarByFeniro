#ifndef APP_STATE_H
#define APP_STATE_H

#include <QObject>
#include <QtQml/QQmlEngine> 
#include <qqml.h>

#include "date.h"
using date::YEAR_MONTH_DAY;

class AppState: public QObject{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int weekDayStartOfMonth READ getWeekDayStartOfMonth  CONSTANT)
    Q_PROPERTY(int dayAmountPrevious READ getDayAmountPrevious   CONSTANT)
    Q_PROPERTY(int dayAmountCurrent READ getDayAmountCurrent   CONSTANT)

    Q_PROPERTY(int visibleMonth READ getVisibleMonth   CONSTANT)
    Q_PROPERTY(int visibleYear READ getVisibleYear   CONSTANT)

        
    Q_PROPERTY(int previousMonth READ getPreviousMonth   CONSTANT)
    Q_PROPERTY(int previousYear READ getPreviousYear   CONSTANT)

    Q_PROPERTY(int nextMonth READ getNextMonth   CONSTANT)
    Q_PROPERTY(int nextYear  READ getNextYear   CONSTANT)

    Q_PROPERTY(int highlightedDay READ getHighlightedDay WRITE setHighlightedDay NOTIFY parameterChanged)
    public:
    void setDate(YEAR_MONTH_DAY new_date);

    int  getVisibleYear()const;
    void setVisibleYear(int year);

    int  getVisibleMonth()const;
    void setVisibleMonth(int month);
    
    int  getWeekDayStartOfMonth()  const;
    int  getDayAmountCurrent()  const;
    int  getDayAmountPrevious() const;

    int  getHighlightedDay()  const;
    void setHighlightedDay(int day);

    int  getPreviousMonth()  const;
    void setPreviousMonth(int month);

    int  getPreviousYear()  const;
    void setPreviousYear(int year);

    int  getNextMonth()  const;
    void setNextMonth(int month);

    int  getNextYear()  const;
    void setNextYear(int year);

    signals:
    void parameterChanged();

    private:
    YEAR_MONTH_DAY visibleDate;
    int weekDayStartOfMonth;
    int dayAmountCurrent;
    int dayAmountPrevious;

    int highlightedDay = -1;
};

#endif