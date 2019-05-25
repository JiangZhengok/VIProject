cmake_minimum_required(VERSION 3.2)

add_definitions(-std=c++11)
#set(CMAKE_CXX_STANDARD 11)

PROJECT(D_GPS)

#head file path
#INCLUDE_DIRECTORIES(
#include
#)
#source directory
#AUX_SOURCE_DIRECTORY(src DIR_SRCS)
#set environmentvariable
#SET(TEST_MATH
#${DIR_SRCS}
#)

ADD_EXECUTABLE(D_GPS D_GPS.cpp)
#add link library
#TARGET_LINK_LIBRARIES(../bin/dmc m)
