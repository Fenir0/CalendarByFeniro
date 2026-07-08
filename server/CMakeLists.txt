cmake_minimum_required(VERSION 3.15)
project(Server CXX)

find_package(Boost REQUIRED CONFIG COMPONENTS system)
find_package(Threads REQUIRED)

set(HEADERS inc/webServerSocket.h)
set(SOURCES src/webServerSocket.cpp main.cpp)

add_executable(Server ${SOURCES} ${HEADERS})

target_link_libraries(Server PRIVATE
    Boost::boost  
    Boost::system     
    Threads::Threads

)