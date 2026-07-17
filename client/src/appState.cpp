#include "../inc/appState.h"
#include "appState.h"


AppState& AppState::instance(){
    static AppState instance;
    return instance;
}

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


}

int AppState::getVisibleMonth() const { return visibleDate.MONTH; }
int AppState::getDateAsYM() const {return visibleDate.YEAR*1e4 + visibleDate.MONTH*1e2;}
int AppState::getVisibleYear() const { return visibleDate.YEAR; }
void AppState::setVisibleYear(int year)
{
    YEAR_MONTH_DAY new_date = visibleDate;
    new_date.YEAR = year;
    setDate(new_date);
    emit parameterChanged();
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
int AppState::getHighlightedDay()const{return highlightedDay;}

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

bool AppState::isSaved() const
{return saved;}

void AppState::setSaved(bool newSaved)
{saved = newSaved;}
bool AppState::isLoggetIn() const{return loggedIn;}

void AppState::setLoggedIn(bool newLoggedIn){loggedIn = newLoggedIn;emit userChanged();}

QString AppState::getUsername() const{return username;}

void AppState::setUsername(QString newUserName){username = newUserName;emit userChanged();}

uint32_t AppState::getUserId() const{return user_id;}

void AppState::setUserId(uint32_t id){user_id = id;emit userChanged();}

QString AppState::getDocumentName() const{return documentName;}

void AppState::setDocumentName(QString newDocumentName){documentName = newDocumentName;emit userChanged();}

uint32_t AppState::getDocumentId() const{return document_id;}

void AppState::setDocumentId(uint32_t id){document_id = id;emit userChanged();}

AccessLevel AppState::getAccessLevel() const{return accessLevel;}

void AppState::setAccessLevel(AccessLevel newAccessLevel){accessLevel = newAccessLevel;}

QString AppState::getVisibleMonthString() const
{
    int a = getVisibleMonth();
    switch (a)
    {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    }
}
