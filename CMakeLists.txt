cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(STDEXT_SRC
    "src/stream.cpp"
    "src/unicode.cpp")
set(STDEXT_INC
    "include/stdext/algorithm.h"
    "include/stdext/array_view.h"
    "include/stdext/concept.h"
    "include/stdext/consumer.h"
    "include/stdext/file.h"
    "include/stdext/generator.h"
    "include/stdext/iterator.h"
    "include/stdext/meta.h"
    "include/stdext/optional.h"
    "include/stdext/range.h"
    "include/stdext/stream.h"
    "include/stdext/string.h"
    "include/stdext/string_view.h"
    "include/stdext/tuple.h"
    "include/stdext/unicode.h"
    "include/stdext/utility.h")

if(WIN32)
    list(APPEND STDEXT_SRC
         "src/windows/file.cpp")
    list(APPEND STDEXT_INC
         "src/windows/platform.h")
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STDEXT_ONLY_BUILD TRUE)
    project(stdext)
endif()

add_library(stdext ${STDEXT_SRC} ${STDEXT_INC})
target_include_directories(stdext PUBLIC "include")

if(WIN32)
    target_compile_definitions(stdext
                               PUBLIC _UNICODE
                               PRIVATE _SCL_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    set_property(TARGET stdext APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
endif()

if(STDEXT_ONLY_BUILD)
    add_subdirectory(tests)
endif()
