cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(iolib)
    set(IOLIB_SRC
        "src/stream.cpp"
        "src/string_view.cpp")
    set(IOLIB_INC
        "include/iolib/algorithm.h"
        "include/iolib/array_view.h"
        "include/iolib/concept.h"
        "include/iolib/optional.h"
        "include/iolib/range.h"
        "include/iolib/stream.h"
        "include/iolib/string_view.h")
    include_directories("include")
    add_library(iolib ${IOLIB_SRC} ${IOLIB_INC})
    add_executable(iotest "test/main.cpp")

    if(WIN32)
        target_compile_definitions(iolib PRIVATE _SCL_SECURE_NO_WARNINGS)
        target_compile_definitions(iotest PRIVATE _SCL_SECURE_NO_WARNINGS)
    endif()

    target_link_libraries(iotest iolib)
    if(MSVC)
        set_property(TARGET iolib APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
        set_property(TARGET iotest APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4814")
    endif()