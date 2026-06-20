cmake_minimum_required(VERSION 3.20)
project(client_connector)

cmake_policy(SET CMP0167 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ament_cmake REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(client_connector src/client_connector.cpp)
target_compile_features(client_connector PRIVATE cxx_std_23)
target_compile_options(client_connector PRIVATE -fcoroutines)
target_link_libraries(client_connector PRIVATE
  Boost::headers
  Boost::program_options
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS client_connector DESTINATION lib/${PROJECT_NAME})
ament_package()
