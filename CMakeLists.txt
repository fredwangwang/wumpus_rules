cmake_minimum_required(VERSION 2.8)
project(wumpus_world)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Source.cpp check_true_false.h check_true_false.cpp wumpus_logic.h)
add_executable(wumpus_world ${SOURCE_FILES})