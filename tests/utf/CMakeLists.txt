cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(SOURCES src/main.cpp)
set(RESOURCES res/UTF-8-test.txt res/UTF-8-post.txt res/UTF-16-post.txt res/UTF-32-post.txt)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${RESOURCES})

add_executable(utftest ${SOURCES} ${RESOURCES})
target_link_libraries(utftest stdext Catch2::Catch2)
add_custom_command(TARGET utftest POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_COMMAND} -E copy_if_different ${RESOURCES} $<TARGET_FILE_DIR:utftest>
)

if(WIN32)
    target_compile_definitions(utftest PRIVATE _SCL_SECURE_NO_WARNINGS)
endif()

catch_discover_tests(utftest WORKING_DIRECTORY $<TARGET_FILE_DIR:utftest>)
