cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(STDEXT_ONLY_BUILD FALSE)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STDEXT_ONLY_BUILD TRUE)
    project(stdext)
endif()

add_library(stdext "")
target_include_directories(stdext PUBLIC include)
target_compile_features(stdext PUBLIC cxx_std_17)
set_target_properties(stdext PROPERTIES CXX_EXTENSIONS OFF)

if(WIN32)
    target_compile_definitions(stdext PRIVATE _UNICODE UNICODE)
endif()

if(MSVC)
    target_compile_definitions(stdext PUBLIC _SCL_SECURE_NO_WARNINGS)
else()
    target_compile_options(stdext PRIVATE -Wno-switch)
endif()

file(GLOB_RECURSE HEADERS include/*)
file(GLOB_RECURSE SOURCES src/*)
file(GLOB_RECURSE POSIX_SOURCES src/posix/*)
file(GLOB_RECURSE WINDOWS_SOURCES src/windows/*)

unset(SOURCES_DISABLED)
if(WIN32)
    list(APPEND SOURCES_DISABLED ${POSIX_SOURCES})
elseif(UNIX)
    list(APPEND SOURCES_DISABLED ${WINDOWS_SOURCES})
endif()

target_sources(stdext PRIVATE ${SOURCES} ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX src FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX include FILES ${HEADERS})
set_source_files_properties(${SOURCES_DISABLED} PROPERTIES LANGUAGE "")

if(STDEXT_ONLY_BUILD)
    add_subdirectory(tests)
endif()
