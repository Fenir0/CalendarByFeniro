cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION "0.0.${PATCH_VERSION}")

project(CalendarByFeniro LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
find_package(Qt6 REQUIRED COMPONENTS Quick Gui)
find_package(nlohmann_json 3.11.3 REQUIRED)
 
qt_standard_project_setup()

qt_add_executable(CalendarByFeniro
  main.cpp src/dayData.cpp src/date.cpp inc/dayData.h inc/date.h
)

target_include_directories(CalendarByFeniro PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

qt_add_qml_module(CalendarByFeniro
    URI Calendar      
    VERSION 1.0
    SOURCES src/dayItem.cpp src/appState.cpp src/dayDataHandler.cpp inc/dayItem.h inc/appState.h inc/dayDataHandler.h
    QML_FILES Main.qml DayModel.qml DayEdit.qml 
    RESOURCES 
        img/editButton.png 
        img/monthBackgroundCurrent.png 
        img/monthBackground.png
        img/monthBackgroundChosen.png
)
 
target_link_libraries(CalendarByFeniro
    PRIVATE Qt6::Quick Qt6::Gui
    PRIVATE nlohmann_json::nlohmann_json
)
 
include(GNUInstallDirs)
install(TARGETS CalendarByFeniro
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)