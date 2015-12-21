cmake_minimum_required(VERSION 3.3)
project(database)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES database.cpp)
add_executable(database ${SOURCE_FILES})