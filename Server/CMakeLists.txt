cmake_minimum_required(VERSION 3.3)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c lib/connection.c lib/connection.h lib/control.c lib/control.h lib/game.c lib/game.h lib/userlist.c lib/userlist.h lib/event.c lib/event.h lib/leaderboard.c lib/leaderboard.h lib/user.c lib/user.h)
add_executable(Server ${SOURCE_FILES})