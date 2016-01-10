cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(stdext)
    set(STDEXT_SRC
        "src/stream.cpp"
        "src/string_view.cpp")
    set(STDEXT_INC
        "include/stdext/algorithm.h"
        "include/stdext/array_view.h"
        "include/stdext/concept.h"
        "include/stdext/generator.h"
        "include/stdext/iterator.h"
		"include/stdext/meta.h"
        "include/stdext/optional.h"
        "include/stdext/range.h"
        "include/stdext/stream.h"
        "include/stdext/string_view.h"
		"include/stdext/tuple.h"
		"include/stdext/unicode.h"
        "include/stdext/utility.h")
    include_directories("include")
    add_library(stdext ${STDEXT_SRC} ${STDEXT_INC})
    add_executable(streamtest "test/main.cpp")

    if(WIN32)
        target_compile_definitions(stdext PRIVATE _SCL_SECURE_NO_WARNINGS)
        target_compile_definitions(streamtest PRIVATE _SCL_SECURE_NO_WARNINGS)
    endif()

    target_link_libraries(streamtest stdext)
    if(MSVC)
        set_property(TARGET stdext APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
        set_property(TARGET streamtest APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
    endif()