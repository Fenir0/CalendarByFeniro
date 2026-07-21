cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION "0.0.${PATCH_VERSION}")

project(CalendarByFeniro LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui Widgets)
find_package(OpenSSL REQUIRED) 
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.11.3 REQUIRED)
 
qt_standard_project_setup()

qt_add_executable(CalendarByFeniro
  main.cpp  src/date.cpp src/webSocketClient.cpp 
  inc/date.h  inc/webSocketWorker.h inc/webSocketClient.h
)

target_include_directories(CalendarByFeniro PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

qt_add_qml_module(CalendarByFeniro
    URI Calendar      
    VERSION 1.0
    SOURCES src/dayItem.cpp src/appState.cpp src/dayDataHandler.cpp src/webSocketWorker.cpp src/requestHandler.cpp src/notificationHandler.cpp
                    inc/dayItem.h inc/appState.h inc/dayDataHandler.h inc/webSocketWorker.h inc/requestHandler.h inc/notificationHandler.h
    QML_FILES qml/Main.qml qml/DayEdit.qml qml/LoadMenu.qml qml/DialogShare.qml qml/ServerIpEnter.qml
              qml/models/DocumentListModel.qml 
              qml/models/DayModel.qml 
              qml/models/UserListModel.qml
    RESOURCES 
        img/networkButton.png
        img/editButton.png 
        img/monthBackgroundCurrent.png 
        img/monthBackground.png
        img/monthBackgroundChosen.png
        img/monthBackgroundSaved.png
        img/documentTrashButton.png
        img/documentShareButton.png
        img/documentLoadButton.png
        img/okButton.png
        img/icon.png
        )
 
target_link_libraries(CalendarByFeniro
    PRIVATE 
    Qt6::Quick 
    Qt6::Gui
    Qt6::Widgets
    nlohmann_json::nlohmann_json
    Boost::system 
    Boost::headers
    OpenSSL::SSL 
    OpenSSL::Crypto
)
 
include(GNUInstallDirs)
install(TARGETS CalendarByFeniro
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)