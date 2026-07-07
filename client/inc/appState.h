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
    Q_PROPERTY(int weekDayStartOfMonth READ getWeekDayStartOfMonth  NOTIFY parameterChanged)
    Q_PROPERTY(int dayAmountPrevious READ getDayAmountPrevious   NOTIFY parameterChanged)
    Q_PROPERTY(int dayAmountCurrent READ getDayAmountCurrent   NOTIFY parameterChanged)

    Q_PROPERTY(int visibleMonth READ getVisibleMonth WRITE setVisibleMonth NOTIFY parameterChanged)
    Q_PROPERTY(int visibleYear READ getVisibleYear    NOTIFY parameterChanged)

        
    Q_PROPERTY(int previousMonth READ getPreviousMonth    NOTIFY parameterChanged)
    Q_PROPERTY(int previousYear READ getPreviousYear    NOTIFY parameterChanged)

    Q_PROPERTY(int nextMonth READ getNextMonth    NOTIFY parameterChanged)
    Q_PROPERTY(int nextYear  READ getNextYear    NOTIFY parameterChanged)

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
    int  getPreviousYear()  const;

    int  getNextMonth()  const;
    int  getNextYear()  const;


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