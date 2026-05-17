cmake_minimum_required(VERSION 3.8)
project(rover_telemetry)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(rover_telemetry
  src/main.cpp
  src/TelemetryNode.cpp
  src/TelemetrySubsystem.cpp
)

target_include_directories(rover_telemetry PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

ament_target_dependencies(rover_telemetry
  rclcpp
  std_msgs
)

install(TARGETS
  rover_telemetry
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
