cmake_minimum_required(VERSION 3.20)

project(vmill-sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0167 NEW)

find_package(Boost REQUIRED)

set(SOURCES
    sources/main.cpp
    sources/mill_machine.cpp
)

set(HEADERS
    include/mill_machine.hpp
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
