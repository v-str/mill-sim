# Минимальная версия CMake, необходимая для сборки проекта
cmake_minimum_required(VERSION 3.20)

# Название проекта и используемые языки (C++)
project(vmill_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0167 NEW)

add_subdirectory(vmill_server)

# Файл для отображения в QtCreator
target_sources(vmill_server PRIVATE ${CMAKE_SOURCE_DIR}/README.md)
