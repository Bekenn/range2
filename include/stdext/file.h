//
//  file.h
//  stdext
//
//  Created by James Touton on 5/14/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_FILE_INCLUDED
#define STDEXT_FILE_INCLUDED
#pragma once

#include "stream.h"


namespace stdext
{
#ifdef _WIN32
    using path_char = wchar_t;
#define PATH_STR(s) L ## s
#else
    using pach_char = char;
#define PATH_STR(s) s
#endif

    class file_input_stream : public input_stream, public seekable
    {
    public:
#ifdef _WIN32
        using native_handle_t = void*;
#else
        using native_handle_t = int;
#endif

    public:
        explicit file_input_stream(const path_char* path);
        ~file_input_stream() override;

    public:
        void seek(seek_from from, ::std::ptrdiff_t offset) override;

    private:
        ::std::size_t do_read(void* buffer, ::std::size_t size) override;
        ::std::size_t do_skip(::std::size_t size) override;

    private:
        native_handle_t handle;
    };
}

#endif
