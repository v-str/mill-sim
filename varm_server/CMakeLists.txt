cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(OpenSSL REQUIRED)

add_executable(varm_server
    sources/main.cpp
)

target_include_directories(varm_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(varm_server PRIVATE
    Boost::program_options
    OpenSSL::SSL
    OpenSSL::Crypto
)
