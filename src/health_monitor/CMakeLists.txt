cmake_minimum_required(VERSION 3.8)
project(health_monitor)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(health_monitor
  src/main.cpp
  src/HealthMonitorNode.cpp
  src/HealthSubsystem.cpp
)

target_include_directories(health_monitor PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

ament_target_dependencies(health_monitor
  rclcpp
  std_msgs
)

install(TARGETS
  health_monitor
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
