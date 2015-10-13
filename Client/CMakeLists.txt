cmake_minimum_required(VERSION 3.3)
project(HangMan)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c lib/game.c lib/game.h lib/control.c lib/control.h lib/connection.c lib/connection.h lib/event.c lib/event.h lib/leaderboard.c lib/leaderboard.h)
add_executable(HangMan ${SOURCE_FILES})