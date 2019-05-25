cmake_minimum_required(VERSION 2.8.3)
project(rtk)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
  geometry_msgs  
)

#add_dependencies(dmc_node dmc_msgs)
## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   rtk_msgs.msg
 )

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
 )

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}_pub 
	src/rtk.cpp
	src/rtk_node.cpp
	)

target_link_libraries(${PROJECT_NAME}_pub
  ${catkin_LIBRARIES}
)

