cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(STDEXT_STANDALONE_BUILD FALSE)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STDEXT_STANDALONE_BUILD TRUE)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    project(stdext)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
    if(MSVC)
        add_compile_options(/MP /W4 /WX)
    endif()
endif()

add_library(stdext)
target_include_directories(stdext PUBLIC include)
target_compile_features(stdext PUBLIC cxx_std_17)
set_target_properties(stdext PROPERTIES CXX_EXTENSIONS OFF)

if(WIN32)
    target_compile_definitions(stdext PRIVATE UNICODE)
endif()

if(MSVC)
    target_compile_definitions(stdext
        PUBLIC _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS
        PRIVATE _UNICODE
    )
    # The XP toolset is incompatible with /permissive-.
    if(NOT CMAKE_VS_PLATFORM_TOOLSET MATCHES [[_xp(,.*)?$]])
        target_compile_options(stdext PRIVATE /permissive-)
    endif()

    set(NATVIS msvc/stdext.natvis)
else()
    target_compile_options(stdext PRIVATE -Wno-switch)
    set(NATVIS "")
endif()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE POSIX_SOURCES CONFIGURE_DEPENDS src/posix/*)
file(GLOB_RECURSE WINDOWS_SOURCES CONFIGURE_DEPENDS src/windows/*)

unset(SOURCES_DISABLED)
if(WIN32)
    list(APPEND SOURCES_DISABLED ${POSIX_SOURCES})
elseif(UNIX)
    list(APPEND SOURCES_DISABLED ${WINDOWS_SOURCES})
endif()

target_sources(stdext PRIVATE ${SOURCES} ${HEADERS} ${NATVIS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS} ${NATVIS})
set_source_files_properties(${SOURCES_DISABLED} PROPERTIES LANGUAGE "")

if(STDEXT_STANDALONE_BUILD)
    enable_testing()
    add_subdirectory(test)
endif()
