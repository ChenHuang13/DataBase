cmake_minimum_required(VERSION 3.3)
project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp systemManager/systemManager.h systemManager/databaseInfo.h systemManager/tableInfo.h indexManager/indexManager.h queryManager/queryManager.h recordManager/recordManager.h indexManager/nodeManager.h indexManager/tree.h queryManager/print.h)
add_executable(database ${SOURCE_FILES})