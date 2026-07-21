#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

#include <chrono>

#include "inc/webSocketClient.h"
#include "inc/webSocketWorker.h"
#include "inc/appState.h"
#include "inc/dayItem.h"
#include "inc/dayDataHandler.h"
#include "inc/requestHandler.h"
#include "inc/notificationHandler.h"

#include "date.h"
using date::YEAR_MONTH_DAY;

YEAR_MONTH_DAY currentVisibleMonth = date::getCurrentDayAsYMD();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/editButton.png"));

    WebSocketWorker* webSocketWorker = &WebSocketWorker::instance();
    RequestHandler requestHandler;

    AppState* currentState = &AppState::instance();
    DayDataHandler* dayDataHandler = &DayDataHandler::instance();
    NotificationHandler notificationHandler;

    QQmlApplicationEngine engine;
    (*currentState).setDate(date::getCurrentDayAsYMD());

    qmlRegisterSingletonInstance("Calendar", 1, 0, "AppState", currentState);
    qmlRegisterSingletonInstance("Calendar", 1, 0, "DayDataHandler", dayDataHandler);
    qmlRegisterSingletonInstance("Calendar", 1, 0, "WebSocket", webSocketWorker);
    qmlRegisterType<DayItemModel>("Calendar", 1, 0, "DayItemModel");

    engine.rootContext()->setContextProperty("RequestHandler", &requestHandler);
    engine.rootContext()->setContextProperty("Notifier", &notificationHandler);
    

    engine.loadFromModule("Calendar", "Main");

    return app.exec();
}