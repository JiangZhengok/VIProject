cmake_minimum_required(VERSION 2.8.3)
project(show_gps)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
  #"/usr/include/eigen3"
)
include_directories("/usr/include/eigen3")
add_executable(gpspath_pub src/path.cpp)
target_link_libraries(gpspath_pub ${catkin_LIBRARIES})


