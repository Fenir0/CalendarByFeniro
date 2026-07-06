#include "../inc/date.h"

date::YEAR_MONTH_DAY date::getCurrentDayAsYMD(){
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    std::chrono::year_month_day currentDay = today;

    YEAR_MONTH_DAY ymd;
    ymd.YEAR  = (int)currentDay.year();
    ymd.MONTH = (unsigned)currentDay.month();
    ymd.DAY   = (unsigned)currentDay.day();
    return ymd;
}

int date::getDaysInMonth(int month, int year){
    if(month == 2 && (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
        return 29;
    }
    return daysInMonths[month-1];
}

int date::getWeekDay(int year, int month, int day){
    std::chrono::year_month_day date = std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day);
    std::chrono::weekday dayOfWeek = std::chrono::sys_days(date);
    return dayOfWeek.iso_encoding();
}
