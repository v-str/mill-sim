# Политика: сохранение CMAKE_CURRENT_SOURCE_DIR при добавлении целей
cmake_policy(SET CMP0167 NEW)

# Поиск системной библиотеки Boost (компонент program_options)
find_package(Boost REQUIRED COMPONENTS program_options)
# Поиск системной библиотеки OpenSSL
find_package(OpenSSL REQUIRED)

# Список исходных файлов
set(SOURCES
    sources/main.cpp
)

# Список заголовочных файлов (пока пуст)
set(HEADERS
)

# Создаём исполняемый файл varm_server из перечисленных исходников
add_executable(varm_server
    ${SOURCES}
)

# Добавляем директорию с заголовками в путь include
target_include_directories(varm_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

# Линкуем исполняемый файл с внешними библиотеками
target_link_libraries(varm_server PRIVATE
    Boost::program_options
    OpenSSL::SSL
    OpenSSL::Crypto
)
