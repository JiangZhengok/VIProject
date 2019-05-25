cmake_minimum_required(VERSION 2.8)
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )
project(medFilter)
add_executable(medFilter medFilter.cpp)
