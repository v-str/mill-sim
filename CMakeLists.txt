cmake_minimum_required(VERSION 3.20)
project(v-arm-server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0167 NEW)

add_subdirectory(varm_server)
