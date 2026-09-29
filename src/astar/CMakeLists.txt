cmake_minimum_required(VERSION 3.14)
project(astar)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

add_library(astar_core SHARED src/astar.cpp)
target_include_directories(astar_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_executable(astar_node src/astar_node.cpp)
target_include_directories(astar_node PUBLIC include)

# Bypass the buggy ROS macro and use standard C++ linking instead
target_link_libraries(astar_node
  astar_core
  rclcpp::rclcpp
  nav_msgs::nav_msgs
  geometry_msgs::geometry_msgs
)

install(TARGETS 
  astar_node
  astar_core
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY include/ DESTINATION include)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_astar test/test_astar.cpp)
  target_include_directories(test_astar PUBLIC include)
  target_link_libraries(test_astar astar_core)
endif()

ament_package()
