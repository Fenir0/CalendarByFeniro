#include "../inc/appState.h"
#include "appState.h"

void AppState::setDate(YEAR_MONTH_DAY new_date){
    visibleDate = new_date;
    visibleDate.DAY = -123;

    weekDayStartOfMonth = date::getWeekDay(visibleDate.YEAR, 
                                          visibleDate.MONTH, 
                                                    1);
    if(visibleDate.MONTH == 1) {
        dayAmountPrevious = date::getDaysInMonth(12, visibleDate.YEAR-1);
    }
    else {
        dayAmountPrevious = date::getDaysInMonth(visibleDate.MONTH - 1, visibleDate.YEAR);
    }
    dayAmountCurrent = date::getDaysInMonth(visibleDate.MONTH, visibleDate.YEAR);

    emit parameterChanged();

}

int AppState::getVisibleMonth() const { return visibleDate.MONTH; }
int AppState::getVisibleYear() const{return visibleDate.YEAR;}
void AppState::setVisibleYear(int year)
{
    YEAR_MONTH_DAY new_date = visibleDate;
    new_date.YEAR = year;
    setDate(new_date);
}
void AppState::setVisibleMonth(int month)
{
    YEAR_MONTH_DAY new_date = visibleDate;
    if(month > 12){
        new_date.MONTH = 1;
        new_date.YEAR += 1;
    }
    else if(month < 1){
        new_date.MONTH = 12;
        new_date.YEAR -= 1;
    }
    else {
        new_date.MONTH = month;
    }
    setDate(new_date);
    emit parameterChanged();
}

int  AppState::getWeekDayStartOfMonth() const {return weekDayStartOfMonth;}
int  AppState::getDayAmountCurrent()const{return dayAmountCurrent;}
int  AppState::getDayAmountPrevious()const{return dayAmountPrevious;}
int AppState::getHighlightedDay()const{
    return highlightedDay;
}

void AppState::setHighlightedDay(int y_m_d){
    int day = y_m_d % 100;
    if(day <= date::daysInMonths[visibleDate.MONTH - 1])
        highlightedDay = y_m_d;
    else highlightedDay = -1;
    emit parameterChanged();
}

int AppState::getPreviousMonth() const
{
    if(visibleDate.MONTH == 1) return 12;
    else return visibleDate.MONTH - 1;
}

int AppState::getPreviousYear() const
{
    if(visibleDate.MONTH == 1) return visibleDate.YEAR - 1;
    else return visibleDate.YEAR;
}

int AppState::getNextMonth() const
{
    if(visibleDate.MONTH == 12) return 1;
    else return visibleDate.MONTH + 1;
}


int AppState::getNextYear() const
{
    if(visibleDate.MONTH == 12) return visibleDate.YEAR + 1;
    else return visibleDate.YEAR;
}
