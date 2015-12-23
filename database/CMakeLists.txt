cmake_minimum_required(VERSION 3.3)
project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp systemManager/systemManager.h systemManager/DB.h systemManager/Table.h indexManager/indexManager.h queryManager/queryManager.h recordManager/recordManager.h)
add_executable(database ${SOURCE_FILES})