cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(SOURCES src/main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(streamtest ${SOURCES})
target_link_libraries(streamtest stdext Catch2::Catch2)

if(WIN32)
    target_compile_definitions(streamtest PRIVATE _SCL_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    set_property(TARGET streamtest APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
endif()
