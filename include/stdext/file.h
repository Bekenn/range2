//
//  file.h
//  stdext
//
//  Created by James Touton on 5/14/16.
//  Copyright � 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_FILE_INCLUDED
#define STDEXT_FILE_INCLUDED
#pragma once

#include "stream.h"
#include "types.h"


namespace stdext
{
#ifdef _WIN32
    using path_char = wchar_t;
    using file_handle_t = void*;
#define PATH_STR(s) L ## s
#else
    using pach_char = char;
    using file_handle_t = int;
#define PATH_STR(s) s
#endif

    enum class file_open_flags
    {
        none = 0,
        create = 1,
        truncate = 2
    };

    class file_input_stream;
    class file_output_stream;

    namespace detail
    {
        class file_stream_base : public seekable
        {
        public:
            explicit file_stream_base(file_handle_t handle);
            ~file_stream_base() override;

        public:
            void seek(seek_from from, ::std::ptrdiff_t offset) override;

        protected:
            file_handle_t handle;
        };
    }

    class file_input_stream : public detail::file_stream_base, public input_stream
    {
    public:
        explicit file_input_stream(const path_char* path);
        ~file_input_stream() override;

    private:
        ::std::size_t do_read(void* buffer, ::std::size_t size) override;
        ::std::size_t do_skip(::std::size_t size) override;
    };

    class file_output_stream : public detail::file_stream_base, public output_stream
    {
    public:
        file_output_stream(const path_char* path, flags<file_open_flags> flags);
        ~file_output_stream() override;

    private:
        virtual ::std::size_t do_write(const void* buffer, ::std::size_t size) override;
    };
}

#endif
