cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_FOLDER Tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

find_package(Catch2 CONFIG REQUIRED)
include(Catch)

add_subdirectory(stream)
add_subdirectory(utf)
