# Минимальная версия CMake, необходимая для сборки проекта
cmake_minimum_required(VERSION 3.20)

# Название проекта и используемые языки (C++)
project(varm_server LANGUAGES CXX)

# Устанавливаем стандарт C++23 и делаем его обязательным
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Политика: при добавлении исполняемого файла в поддиректории
# CMAKE_CURRENT_SOURCE_DIR остается неизменным
cmake_policy(SET CMP0167 NEW)

# Подключаем дочерний модуль varm_server
add_subdirectory(varm_server)
