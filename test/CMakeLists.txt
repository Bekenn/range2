cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

find_package(Catch2 CONFIG REQUIRED)
include(Catch)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS res/*)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${RESOURCES})

add_executable(test-stdext ${SOURCES} ${RESOURCES})
target_link_libraries(test-stdext PRIVATE stdext Catch2::Catch2)
add_custom_command(TARGET test-stdext POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_COMMAND} -E copy_if_different ${RESOURCES} $<TARGET_FILE_DIR:test-stdext>
)

if(WIN32)
    target_compile_definitions(test-stdext PRIVATE _SCL_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    target_compile_options(test-stdext PRIVATE /permissive-)
endif()

catch_discover_tests(test-stdext WORKING_DIRECTORY $<TARGET_FILE_DIR:test-stdext>)
