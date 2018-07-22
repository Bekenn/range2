cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

add_executable(streamtest "src/main.cpp")
target_link_libraries(streamtest stdext)

if(WIN32)
    target_compile_definitions(streamtest PRIVATE _SCL_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    set_property(TARGET streamtest APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
endif()
