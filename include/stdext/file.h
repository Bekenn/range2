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

#include <stdext/platform.h>
#include <stdext/stream.h>
#include <stdext/types.h>

#include <system_error>


namespace stdext
{
#if STDEXT_PLATFORM_WINDOWS
    using file_handle_t = void*;
#else
    using file_handle_t = int;
#endif

    enum class file_open_flags
    {
        none = 0,
        create = 1,
        create_exclusive = 3,
        truncate = 4
    };

    struct utf8_path_encoding { };

    class file_input_stream;
    class file_output_stream;
    class file_stream;

    namespace detail
    {
        class file_stream_base : public seekable
        {
        protected:
            file_stream_base();
            file_stream_base(const file_stream_base&) = delete;
            file_stream_base& operator = (const file_stream_base&) = delete;
            file_stream_base(file_stream_base&& other);
            file_stream_base& operator = (file_stream_base&& other);
            ~file_stream_base() override;

            explicit file_stream_base(file_handle_t handle);

        public:
            bool is_open() const noexcept;
            void close() noexcept;

        public:
            stream_position position() const override;
            stream_position end_position() const override;
            void set_position(stream_position position) override;

        protected:
            template <class Stream> friend class file_input_stream_base;
            template <class Stream> friend class file_output_stream_base;

            file_handle_t handle;
        };

        template <class Stream>
        class file_input_stream_base : public input_stream
        {
        private:
            size_t do_read(uint8_t* buffer, size_t size) override;
            size_t do_skip(size_t size) override;

        private:
            Stream& self() noexcept { return static_cast<Stream&>(*this); }
            const Stream& self() const noexcept { return static_cast<const Stream&>(*this); }
        };

        template <class Stream>
        class file_output_stream_base : public output_stream
        {
        private:
            size_t do_write(const uint8_t* buffer, size_t size) override;

        private:
            Stream& self() noexcept { return static_cast<Stream&>(*this); }
            const Stream& self() const noexcept { return static_cast<const Stream&>(*this); }
        };
    }

    class file_input_stream : public detail::file_stream_base, public detail::file_input_stream_base<file_input_stream>
    {
    public:
        file_input_stream() = default;
        file_input_stream(file_input_stream&&) = default;
        file_input_stream& operator = (file_input_stream&&) = default;
        ~file_input_stream() override;

        explicit file_input_stream(const path_char* path);
        file_input_stream(const char* path, utf8_path_encoding);

    public:
        ::std::error_code open(const path_char* path);
        ::std::error_code open(const char* path, utf8_path_encoding);
    };

    class file_output_stream : public detail::file_stream_base, public detail::file_output_stream_base<file_output_stream>
    {
    private:
        static constexpr flags<file_open_flags> default_flags = { file_open_flags::create, file_open_flags::truncate };

    public:
        file_output_stream() = default;
        file_output_stream(file_output_stream&&) = default;
        file_output_stream& operator = (file_output_stream&&) = default;
        ~file_output_stream() override;

        explicit file_output_stream(const path_char* path, flags<file_open_flags> flags = default_flags);
        file_output_stream(const char* path, utf8_path_encoding, flags<file_open_flags> flags = default_flags);

    public:
        ::std::error_code open(const path_char* path, flags<file_open_flags> flags = default_flags);
        ::std::error_code open(const char* path, utf8_path_encoding, flags<file_open_flags> flags = default_flags);
    };

    class file_stream : public detail::file_stream_base, public detail::file_input_stream_base<file_stream>, public detail::file_output_stream_base<file_stream>, public stream
    {
    private:
        static constexpr flags<file_open_flags> default_flags = { file_open_flags::create };

    public:
        file_stream() = default;
        file_stream(file_stream&&) = default;
        file_stream& operator = (file_stream&&) = default;
        ~file_stream() override;

        explicit file_stream(const path_char* path, flags<file_open_flags> flags = default_flags);
        file_stream(const char* path, utf8_path_encoding, flags<file_open_flags> flags = default_flags);

    public:
        ::std::error_code open(const path_char* path, flags<file_open_flags> flags = default_flags);
        ::std::error_code open(const char* path, utf8_path_encoding, flags<file_open_flags> flags = default_flags);
    };
}

#endif
