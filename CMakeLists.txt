cmake_minimum_required(VERSION 3.20)

project(vmill-sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0167 NEW)

find_package(Boost REQUIRED)

set(SOURCES
    sources/main.cpp
    sources/appcore.cpp
    sources/tcp_server.cpp
)

set(HEADERS
    include/appcore.hpp
    include/tcp_server.hpp
    include/common.hpp
)

add_executable(vmill-sim
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(vmill-sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vmill-sim PRIVATE
    Boost::headers
)

add_custom_target(project_files SOURCES ${CMAKE_SOURCE_DIR}/README.md)
