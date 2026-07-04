cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION "0.0.${PATCH_VERSION}")

project(CalendarByFeniro LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
find_package(Qt6 REQUIRED COMPONENTS Quick Gui)
 
qt_standard_project_setup()

qt_add_executable(CalendarByFeniro
  main.cpp src/dayModel.cpp src/dayData.cpp inc/dayModel.h inc/dayData.h
)

qt_add_qml_module(CalendarByFeniro
    URI calendar      
    VERSION 1.0
    SOURCES src/dayModel.cpp inc/dayModel.h
    QML_FILES main.qml DayModel.qml
)
 
target_link_libraries(CalendarByFeniro
    PRIVATE Qt6::Quick Qt6::Gui
)
 
include(GNUInstallDirs)
install(TARGETS CalendarByFeniro
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)