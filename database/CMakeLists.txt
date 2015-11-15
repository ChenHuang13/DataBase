cmake_minimum_required(VERSION 3.3)
project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp RecordManager/RecordManager.h RecordManager/Record.h RecordManager/Query.h RecordManager/Field.h RecordManager/Data.h RecordManager/Table.h SystemManager/SystemManager.h IndexManager/IndexManager.h QueryManager/QueryManager.h define.h DataBaseManage.h Debug.h Order.h RecordManager/RecordID.h SystemManager/InfoManeger.h Print.h)
add_executable(database ${SOURCE_FILES})