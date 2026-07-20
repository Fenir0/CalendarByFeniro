#ifndef DATE_H

#define DATE_H
#include <chrono>

// ================
// DATE FUNCTIONS
// ================

namespace date {
    inline int daysInMonths[] =  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    struct YEAR_MONTH_DAY{
        int YEAR;
        int MONTH;
        int DAY;
    };

    YEAR_MONTH_DAY getCurrentDayAsYMD();

    int getDaysInMonth(int month, int year);

    int getWeekDay(int year, int month, int day);
}

#endif