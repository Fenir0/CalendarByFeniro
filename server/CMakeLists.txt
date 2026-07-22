cmake_minimum_required(VERSION 3.15)
project(Server CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(libpqxx REQUIRED)

set(HEADERS 
    inc/webServerSocket.h 
    inc/postgresql.h
    inc/resourceManager.h 
    inc/documentHandler.h
    inc/fileWatcher.h     
    inc/sessionWatcher.h
)

set(SOURCES 
    src/webServerSocket.cpp 
    main.cpp 
    src/postgresql.cpp
    src/resourceManager.cpp 
    src/documentHandler.cpp
    src/fileWatcher.cpp 
    src/sessionWatcher.cpp
)

add_executable(Server ${SOURCES} ${HEADERS})
if(MSVC)
    target_link_options(Server PRIVATE /FORCE:MULTIPLE)
endif()

target_include_directories(Server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(Server PRIVATE
    Boost::system
    Threads::Threads
    nlohmann_json::nlohmann_json
    libpqxx::pqxx  
)
