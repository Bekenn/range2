cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(STDEXT_SRC
    src/stream.cpp
    src/unicode.cpp)
set(STDEXT_INC
    include/stdext/detail/config.h
    include/stdext/algorithm.h
    include/stdext/array_view.h
    include/stdext/consumer.h
    include/stdext/file.h
    include/stdext/generator.h
    include/stdext/iterator.h
    include/stdext/meta.h
    include/stdext/optional.h
    include/stdext/range.h
    include/stdext/stream.h
    include/stdext/string.h
    include/stdext/string_view.h
    include/stdext/traits.h
    include/stdext/tuple.h
    include/stdext/types.h
    include/stdext/unicode.h
    include/stdext/utility.h)

if(WIN32)
    list(APPEND STDEXT_SRC
        src/windows/file.cpp)
    list(APPEND STDEXT_INC
        src/windows/platform.h)
endif()

foreach(file ${STDEXT_SRC} ${STDEXT_INC})
    get_filename_component(dir ${file} DIRECTORY)
    string(REPLACE "/" "\\" group ${dir})
    source_group(${group} FILES ${file})
endforeach()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STDEXT_ONLY_BUILD TRUE)
    project(stdext)
endif()

add_library(stdext ${STDEXT_SRC} ${STDEXT_INC})
target_include_directories(stdext PUBLIC include)

if(WIN32)
    target_compile_definitions(stdext
                               PUBLIC _UNICODE
                               PUBLIC _SCL_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    target_compile_options(stdext PRIVATE /wd4814)
endif()

set_target_properties(stdext PROPERTIES CXX_EXTENSIONS OFF)
if(NOT MSVC)
    target_compile_options(stdext PUBLIC -std=c++1z PRIVATE -Wno-switch)
endif()

if(STDEXT_ONLY_BUILD)
    add_subdirectory(tests)
endif()
