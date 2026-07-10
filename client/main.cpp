#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include <chrono>

#include "inc/appState.h"
#include "inc/dayItem.h"
#include "inc/dayDataHandler.h"


using date::YEAR_MONTH_DAY;

YEAR_MONTH_DAY currentVisibleMonth = date::getCurrentDayAsYMD();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    AppState currentState;
    DayDataHandler dayDataHandler;
    QQmlApplicationEngine engine;
    currentState.setDate(date::getCurrentDayAsYMD());

    qmlRegisterSingletonInstance("Calendar", 1, 0, "AppState", &currentState);
    qmlRegisterSingletonInstance("Calendar", 1, 0, "DayDataHandler", &dayDataHandler);
    qmlRegisterType<DayItemModel>("Calendar", 1, 0, "DayItemModel");
    

    engine.loadFromModule("Calendar", "Main");

    return app.exec();
}