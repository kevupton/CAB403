cmake_minimum_required(VERSION 3.3)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c lib/connection.c lib/connection.h lib/control.c lib/control.h lib/game.c lib/game.h lib/gamelist.c lib/gamelist.h lib/event.c lib/event.h)
add_executable(Server ${SOURCE_FILES})