cmake_minimum_required(VERSION 3.15)
project(Server CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED CONFIG COMPONENTS system)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

set(HEADERS inc/webServerSocket.h inc/postgresql.h 
            inc/resourceManager.h inc/documentHandler.h 
            inc/fileWatcher.h     inc/sessionWatcher.h)
set(SOURCES src/webServerSocket.cpp main.cpp src/postgresql.cpp 
            src/resourceManager.cpp src/documentHandler.cpp 
            src/fileWatcher.cpp src/sessionWatcher.cpp)

add_executable(Server ${SOURCES} ${HEADERS})

target_link_libraries(Server PRIVATE
    Boost::boost  
    Boost::system     
    Threads::Threads
    pqxx

)