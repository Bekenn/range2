#include <stdext/file.h>
#include <stdext/unicode.h>

#include "platform.h"

#include <algorithm>
#include <system_error>


namespace stdext
{
    namespace
    {
        class std_input_stream : public detail::file_input_stream_base<std_input_stream>
        {
        public:
            explicit std_input_stream(HANDLE handle) : handle(handle) { }

        public:
            bool is_open() const noexcept { return true; }

        private:
            template <class Stream> friend class detail::file_input_stream_base;
            file_handle_t handle;
        };

        class std_output_stream : public detail::file_output_stream_base<std_output_stream>
        {
        public:
            explicit std_output_stream(HANDLE handle) : handle(handle) { }

        public:
            bool is_open() const noexcept { return true; }

        private:
            template <class Stream> friend class detail::file_output_stream_base;
            file_handle_t handle;
        };

        DWORD creation_disposition(flags<file_open_flags> flags);
    }

    namespace detail
    {
        file_stream_base::file_stream_base() : handle(INVALID_HANDLE_VALUE)
        {
        }

        file_stream_base::file_stream_base(file_stream_base&& other) : handle(move(other.handle))
        {
            other.handle = INVALID_HANDLE_VALUE;
        }

        file_stream_base& file_stream_base::operator = (file_stream_base&& other)
        {
            if (is_open())
                close();

            handle = move(other.handle);
            other.handle = INVALID_HANDLE_VALUE;
            return *this;
        }

        file_stream_base::~file_stream_base()
        {
            ::CloseHandle(handle);
        }

        file_stream_base::file_stream_base(file_handle_t handle) : handle(handle)
        {
            if (handle == INVALID_HANDLE_VALUE)
                throw std::system_error(::GetLastError(), std::system_category());
        }

        bool file_stream_base::is_open() const noexcept
        {
            return handle != INVALID_HANDLE_VALUE;
        }

        void file_stream_base::close() noexcept
        {
            assert(is_open());

            ::CloseHandle(handle);
            handle = INVALID_HANDLE_VALUE;
        }

        stream_position file_stream_base::position() const
        {
            assert(is_open());

            LARGE_INTEGER pos;
            if (!::SetFilePointerEx(handle, {}, &pos, FILE_CURRENT))
                throw std::system_error(::GetLastError(), std::system_category());

            return pos.QuadPart;
        }

        stream_position file_stream_base::end_position() const
        {
            assert(is_open());

            LARGE_INTEGER size;
            if (!::GetFileSizeEx(handle, &size))
                throw std::system_error(::GetLastError(), std::system_category());

            return size.QuadPart;
        }

        void file_stream_base::set_position(stream_position position)
        {
            assert(is_open());

            LARGE_INTEGER pos;
            pos.QuadPart = position;
            if (!::SetFilePointerEx(handle, pos, nullptr, FILE_BEGIN))
                throw std::system_error(::GetLastError(), std::system_category());
        }

        template <class Stream>
        size_t file_input_stream_base<Stream>::do_read(uint8_t* buffer, size_t size)
        {
            assert(self().is_open());

            constexpr DWORD granularity = 0x1000;

            auto p = buffer;
            size_t bytes = 0;
            while (size != 0)
            {
                DWORD chunk_size = size > MAXDWORD ? MAXDWORD & ~(granularity - 1) : DWORD(size);
                DWORD chunk_bytes;
                if (!::ReadFile(self().handle, p, chunk_size, &chunk_bytes, nullptr))
                    throw std::system_error(::GetLastError(), std::system_category());
                if (chunk_bytes == 0)
                    break;
                size -= chunk_bytes;
                bytes += chunk_bytes;
                p += chunk_bytes;
            }

            return bytes;
        }

        template <class Stream>
        size_t file_input_stream_base<Stream>::do_skip(size_t size)
        {
            assert(self().is_open());

            LARGE_INTEGER file_size;
            if (!::GetFileSizeEx(self().handle, &file_size))
                throw std::system_error(::GetLastError(), std::system_category());

            LARGE_INTEGER distance;
            if (!::SetFilePointerEx(self().handle, LARGE_INTEGER(), &distance, FILE_CURRENT))
                throw std::system_error(::GetLastError(), std::system_category());

            distance.QuadPart = std::min(static_cast<LONGLONG>(size), file_size.QuadPart - distance.QuadPart);
            if (!::SetFilePointerEx(self().handle, distance, nullptr, FILE_CURRENT))
                throw std::system_error(::GetLastError(), std::system_category());

            return static_cast<size_t>(distance.QuadPart);
        }

        template <class Stream>
        size_t file_output_stream_base<Stream>::do_write(const uint8_t* buffer, size_t size)
        {
            assert(self().is_open());

            constexpr DWORD granularity = 0x1000;

            auto p = buffer;
            size_t bytes = 0;
            while (size != 0)
            {
                DWORD chunk_size = size > MAXDWORD ? MAXDWORD & ~(granularity - 1) : DWORD(size);
                DWORD chunk_bytes;
                if (!::WriteFile(self().handle, p, chunk_size, &chunk_bytes, nullptr))
                    throw std::system_error(::GetLastError(), std::system_category());
                if (chunk_bytes < chunk_size)
                    return bytes + chunk_bytes;
                size -= chunk_bytes;
                bytes += chunk_bytes;
                p += chunk_bytes;
            }

            return bytes;
        }
    }

    file_input_stream::file_input_stream(const path_char* path)
        : file_stream_base(::CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr))
    {
    }

    file_input_stream::file_input_stream(const char* path, utf8_path_encoding)
        : file_input_stream(reinterpret_cast<const path_char*>(to_u16string(path).second.c_str()))  // TODO: Exception on encoding error
    {
    }

    file_input_stream::~file_input_stream() = default;

    std::error_code file_input_stream::open(const path_char* path)
    {
        assert(!is_open());

        handle = ::CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (handle == INVALID_HANDLE_VALUE)
            return { int(::GetLastError()), std::system_category() };

        return { };
    }

    std::error_code file_input_stream::open(const char* path, utf8_path_encoding)
    {
        assert(!is_open());

        auto path_str = to_u16string(path);
        if (path_str.first == utf_result::error)
            return { ERROR_NO_UNICODE_TRANSLATION, std::system_category() };

        return open(reinterpret_cast<const path_char*>(path_str.second.c_str()));
    }

    file_output_stream::file_output_stream(const path_char* path, flags<file_open_flags> flags)
        : file_stream_base(::CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, creation_disposition(flags), FILE_ATTRIBUTE_NORMAL, nullptr))
    {
    }

    file_output_stream::file_output_stream(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
        : file_output_stream(reinterpret_cast<const path_char*>(to_u16string(path).second.c_str()), flags)  // TODO: Exception on encoding error
    {
    }

    file_output_stream::~file_output_stream() = default;

    std::error_code file_output_stream::open(const path_char* path, flags<file_open_flags> flags)
    {
        assert(!is_open());

        handle = ::CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, creation_disposition(flags), FILE_ATTRIBUTE_NORMAL, nullptr);
        if (handle == INVALID_HANDLE_VALUE)
            return { int(::GetLastError()), std::system_category() };

        return { };
    }

    std::error_code file_output_stream::open(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
    {
        assert(!is_open());

        auto path_str = to_u16string(path);
        if (path_str.first == utf_result::error)
            return { ERROR_NO_UNICODE_TRANSLATION, std::system_category() };

        return open(reinterpret_cast<const path_char*>(path_str.second.c_str()), flags);
    }

    file_stream::file_stream(const path_char* path, flags<file_open_flags> flags)
        : file_stream_base(::CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, creation_disposition(flags), FILE_ATTRIBUTE_NORMAL, nullptr))
    {
    }

    file_stream::file_stream(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
        : file_stream(reinterpret_cast<const path_char*>(to_u16string(path).second.c_str()), flags)  // TODO: Exception on encoding error
    {
    }

    file_stream::~file_stream() = default;

    std::error_code file_stream::open(const path_char* path, flags<file_open_flags> flags)
    {
        assert(!is_open());

        handle = ::CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, creation_disposition(flags), FILE_ATTRIBUTE_NORMAL, nullptr);
        if (handle == INVALID_HANDLE_VALUE)
            return { int(::GetLastError()), std::system_category() };

        return { };
    }

    std::error_code file_stream::open(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
    {
        assert(!is_open());

        auto path_str = to_u16string(path);
        if (path_str.first == utf_result::error)
            return { ERROR_NO_UNICODE_TRANSLATION, std::system_category() };

        return open(reinterpret_cast<const path_char*>(path_str.second.c_str()), flags);
    }

    input_stream& in()
    {
        static std_input_stream in(::GetStdHandle(STD_INPUT_HANDLE));
        return in;
    }

    output_stream& out()
    {
        static std_output_stream out(::GetStdHandle(STD_OUTPUT_HANDLE));
        return out;
    }

    output_stream& err()
    {
        static std_output_stream err(::GetStdHandle(STD_ERROR_HANDLE));
        return err;
    }

    namespace
    {
        DWORD creation_disposition(flags<file_open_flags> flags)
        {
            // The static_cast is here to work around a Visual Studio 2015 bug.
            switch (static_cast<file_open_flags>(flags))
            {
            case file_open_flags::none:
                return OPEN_EXISTING;
            case file_open_flags::create:
                return OPEN_ALWAYS;
            case file_open_flags::create_exclusive:
                return CREATE_NEW;
            case file_open_flags::truncate:
                return TRUNCATE_EXISTING;
            case make_flags(file_open_flags::create, file_open_flags::truncate):
                return CREATE_ALWAYS;
            }

            return -1;
        }
    }
}
