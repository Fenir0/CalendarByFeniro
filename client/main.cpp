#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include <chrono>

#include "inc/dayModel.h"

struct YEAR_MONTH_DAY{
    int YEAR;
    int MONTH;
    int DAY;
};

// will be moved to namespace

YEAR_MONTH_DAY getCurrentDayAsYMD(){
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    std::chrono::year_month_day currentDay = today;

    YEAR_MONTH_DAY ymd;
    ymd.YEAR  = (int)currentDay.year();
    ymd.MONTH = (unsigned)currentDay.month();
    ymd.DAY   = (unsigned)currentDay.day();
    return ymd;
}

int getDaysInMonth(int month, int year){
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(month == 2 && (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
        return 29;
    }
    return days[month-1];
}

int getWeekDay(int year, int month, int day){
    std::chrono::year_month_day date = std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day);
    std::chrono::weekday dayOfWeek = std::chrono::sys_days(date);
    return dayOfWeek.iso_encoding();
}

YEAR_MONTH_DAY currentVisibleMonth = getCurrentDayAsYMD();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("dayWeekStart", getWeekDay(currentVisibleMonth.YEAR, 
                                                                        currentVisibleMonth.MONTH, 
                                                                        1));
    if(currentVisibleMonth.MONTH == 1) {
        engine.rootContext()->setContextProperty("dayAmountPrevious", getDaysInMonth(12, currentVisibleMonth.YEAR-1));
    }
    else {
        engine.rootContext()->setContextProperty("dayAmountPrevious", getDaysInMonth(currentVisibleMonth.MONTH-1, 
                                                                                     currentVisibleMonth.YEAR));
    }
    engine.rootContext()->setContextProperty("dayAmountCurrent" , getDaysInMonth(currentVisibleMonth.MONTH, 
                                                                                 currentVisibleMonth.YEAR));

    return app.exec();
}