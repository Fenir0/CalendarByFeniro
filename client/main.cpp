#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include <chrono>

#include "inc/appState.h"
#include "inc/dayItem.h"


using date::YEAR_MONTH_DAY;

YEAR_MONTH_DAY currentVisibleMonth = date::getCurrentDayAsYMD();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    AppState currentState;
    QQmlApplicationEngine engine;
    currentState.setDate(date::getCurrentDayAsYMD());

    qmlRegisterSingletonInstance("Calendar", 1, 0, "AppState", &currentState);
    qmlRegisterType<DayItemModel>("Calendar", 1, 0, "DayItemModel");

    engine.loadFromModule("Calendar", "Main");

    // separate thread: ASIO

    return app.exec();
}