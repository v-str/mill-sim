# Политика: сохранение CMAKE_CURRENT_SOURCE_DIR при добавлении целей
cmake_policy(SET CMP0167 NEW)

# Поиск системной библиотеки Boost (компонент program_options)
find_package(Boost REQUIRED COMPONENTS program_options)
# Поиск системной библиотеки OpenSSL
find_package(OpenSSL REQUIRED)

# Список исходных файлов
set(SOURCES
    sources/main.cpp
    sources/client_connection/tdm.cpp
    sources/client_connection/udm.cpp
    sources/client_connection/bdm.cpp
    sources/client_connection/edm.cpp
    sources/command_center/ucc.cpp
)

# Список заголовочных файлов
set(HEADERS
    include/client_connection/tdm.hpp
    include/client_connection/udm.hpp
    include/client_connection/bdm.hpp
    include/client_connection/edm.hpp
    include/command_center/ucc.hpp
)

# Создаём исполняемый файл vmill_server
add_executable(vmill_server
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(vmill_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линкуем исполняемый файл с внешними библиотеками
target_link_libraries(vmill_server PRIVATE
    Boost::program_options
    OpenSSL::SSL
    OpenSSL::Crypto
)
