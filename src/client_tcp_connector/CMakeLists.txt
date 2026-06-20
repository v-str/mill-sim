cmake_minimum_required(VERSION 3.20)
project(client_tcp_connector)

cmake_policy(SET CMP0167 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(client_tcp_connector
  src/client_tcp_connector.cpp
  src/client_session.cpp
  src/tcp_server.cpp
)
target_include_directories(client_tcp_connector PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(client_tcp_connector PRIVATE cxx_std_23)
target_compile_options(client_tcp_connector PRIVATE -fcoroutines)
target_link_libraries(client_tcp_connector PRIVATE
  rclcpp::rclcpp
  Boost::headers
  Boost::program_options
)

install(TARGETS client_tcp_connector DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
