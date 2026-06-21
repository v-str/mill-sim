cmake_minimum_required(VERSION 3.20)
project(tcp_connector)

cmake_policy(SET CMP0167 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(tcp_connector
  src/tcp_connector.cpp
  src/client_session.cpp
  src/tcp_server.cpp
)
target_include_directories(tcp_connector PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(tcp_connector PRIVATE cxx_std_23)
target_compile_options(tcp_connector PRIVATE -fcoroutines)
target_link_libraries(tcp_connector PRIVATE
  rclcpp::rclcpp
  std_msgs::std_msgs
  Boost::headers
  Boost::program_options
)

install(TARGETS tcp_connector DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
