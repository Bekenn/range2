cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

add_executable(utftest "src/main.cpp" "UTF-8-test.txt")
target_link_libraries(utftest stdext)
add_custom_command(TARGET utftest POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/UTF-8-test.txt $<TARGET_PROPERTY:utftest,BINARY_DIR>)

if(WIN32)
    target_compile_definitions(utftest
                               PRIVATE _SCL_SECURE_NO_WARNINGS)
endif()
