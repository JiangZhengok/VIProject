cmake_minimum_required(VERSION 2.8.3)
project(stim)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
)
add_executable(${PROJECT_NAME}_pub 
	src/stim_main.cpp
	src/stimsrc/MyPortSerial.cpp
	src/stimsrc/STIM300Driver.cpp
	)

target_link_libraries(${PROJECT_NAME}_pub
  ${catkin_LIBRARIES}
)

