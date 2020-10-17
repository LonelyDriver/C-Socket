cmake_minimum_required(VERSION 3.5)
project(TCP_Infrastructur CXX)

if(WIN32)
set(COMPILE_OPTIONS)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

else()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror)
endif()

option(build_examples "Building examples" OFF)

find_package(Threads)

add_subdirectory(Logging)

add_library(tcp_connection SHARED src/socket.cpp)
target_include_directories(tcp_connection PUBLIC include)
target_link_libraries(tcp_connection logger)
target_compile_options(tcp_connection PRIVATE ${COMPILE_OPTIONS})

if(build_examples)
    add_executable(tcp_examples examples/tcp_examples.cpp)
    target_include_directories(tcp_examples PUBLIC include)
    target_link_libraries(tcp_examples tcp_connection)
    target_compile_options(tcp_examples PRIVATE ${COMPILE_OPTIONS})
endif(build_examples)
