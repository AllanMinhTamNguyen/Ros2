# CMake generated Testfile for 
# Source directory: /home/allan/ros2_ws/src/astar
# Build directory: /home/allan/ros2_ws/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_astar "/usr/bin/python3" "-u" "/opt/ros/lyrical/share/ament_cmake_test/cmake/run_test.py" "/home/allan/ros2_ws/build/test_results/astar/test_astar.gtest.xml" "--package-name" "astar" "--output-file" "/home/allan/ros2_ws/build/ament_cmake_gtest/test_astar.txt" "--command" "/home/allan/ros2_ws/build/test_astar" "--gtest_output=xml:/home/allan/ros2_ws/build/test_results/astar/test_astar.gtest.xml")
set_tests_properties(test_astar PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/allan/ros2_ws/build/test_astar" TIMEOUT "60" WORKING_DIRECTORY "/home/allan/ros2_ws/build" _BACKTRACE_TRIPLES "/opt/ros/lyrical/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/lyrical/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;95;ament_add_test;/opt/ros/lyrical/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/allan/ros2_ws/src/astar/CMakeLists.txt;40;ament_add_gtest;/home/allan/ros2_ws/src/astar/CMakeLists.txt;0;")
