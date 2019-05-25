cmake_minimum_required(VERSION 2.8.3)
project(magnetic)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
  geometry_msgs
  
)

 add_message_files(
   FILES
   dmc_msgs.msg
 )

generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
 )

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include

)

add_executable(${PROJECT_NAME}_pub 
	src/dmc.cpp
	src/dmc_node.cpp
	)

target_link_libraries(${PROJECT_NAME}_pub
  ${catkin_LIBRARIES}
)
