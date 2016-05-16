#include <stdext/file.h>

#include "platform.h"

#include <algorithm>
#include <system_error>


namespace stdext
{
    namespace
    {
        DWORD creation_disposition(flags<file_open_flags> flags);
    }

    namespace detail
    {
        file_stream_base::file_stream_base(file_handle_t handle) : handle(handle)
        {
            if (handle == INVALID_HANDLE_VALUE)
                throw std::system_error(::GetLastError(), std::system_category());
        }

        file_stream_base::~file_stream_base()
        {
            ::CloseHandle(handle);
        }

        void file_stream_base::seek(seek_from from, std::ptrdiff_t offset)
        {
            LARGE_INTEGER distance;
            distance.QuadPart = offset;

            DWORD method;
            switch (from)
            {
            case seek_from::begin:
                method = FILE_BEGIN;
                break;

            case seek_from::current:
                method = FILE_CURRENT;
                break;

            case seek_from::end:
                method = FILE_END;
                break;
            }

            if (!::SetFilePointerEx(handle, distance, nullptr, method))
                throw std::system_error(::GetLastError(), std::system_category());
        }
    }

    file_input_stream::file_input_stream(const path_char* path)
        : file_stream_base(::CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr))
    {
    }

    file_input_stream::~file_input_stream() = default;

    std::size_t file_input_stream::do_read(void* buffer, std::size_t size)
    {
        constexpr DWORD granularity = 0x1000;

        auto p = static_cast<std::uint8_t*>(buffer);
        std::size_t bytes = 0;
        while (size != 0)
        {
            DWORD chunk_size = size > MAXDWORD ? MAXDWORD & ~(granularity - 1) : size;
            DWORD chunk_bytes;
            if (!::ReadFile(handle, p, chunk_size, &chunk_bytes, nullptr))
                throw std::system_error(::GetLastError(), std::system_category());
            if (chunk_bytes == 0)
                break;
            size -= chunk_bytes;
            bytes += chunk_bytes;
            p += chunk_bytes;
        }

        return bytes;
    }

    std::size_t file_input_stream::do_skip(std::size_t size)
    {
        LARGE_INTEGER file_size;
        if (!::GetFileSizeEx(handle, &file_size))
            throw std::system_error(::GetLastError(), std::system_category());

        LARGE_INTEGER distance;
        if (!::SetFilePointerEx(handle, LARGE_INTEGER(), &distance, FILE_CURRENT))
            throw std::system_error(::GetLastError(), std::system_category());

        distance.QuadPart = std::min(static_cast<LONGLONG>(size), file_size.QuadPart - distance.QuadPart);
        if (!::SetFilePointerEx(handle, distance, nullptr, FILE_CURRENT))
            throw std::system_error(::GetLastError(), std::system_category());

        return static_cast<std::size_t>(distance.QuadPart);
    }

    file_output_stream::file_output_stream(const path_char* path, flags<file_open_flags> flags)
        : file_stream_base(::CreateFile(path, GENERIC_WRITE, 0, nullptr, creation_disposition(flags), FILE_ATTRIBUTE_NORMAL, nullptr))
    {
    }

    file_output_stream::~file_output_stream() = default;

    std::size_t file_output_stream::do_write(const void* buffer, std::size_t size)
    {
        constexpr DWORD granularity = 0x1000;

        auto p = static_cast<const uint8_t*>(buffer);
        std::size_t bytes = 0;
        while (size != 0)
        {
            DWORD chunk_size = size > MAXDWORD ? MAXDWORD & ~(granularity - 1) : size;
            DWORD chunk_bytes;
            if (!::WriteFile(handle, p, chunk_size, &chunk_bytes, nullptr))
                throw std::system_error(::GetLastError(), std::system_category());
            if (chunk_bytes < chunk_size)
                return bytes + chunk_bytes;
            size -= chunk_bytes;
            bytes += chunk_bytes;
            p += chunk_bytes;
        }

        return bytes;
    }

    namespace
    {
        DWORD creation_disposition(flags<file_open_flags> flags)
        {
            switch (flags)
            {
            case file_open_flags::none:
                return OPEN_EXISTING;
            case file_open_flags::create:
                return OPEN_ALWAYS;
            case file_open_flags::truncate:
                return TRUNCATE_EXISTING;
            case stdext::flags<file_open_flags>(file_open_flags::create, file_open_flags::truncate):
                return CREATE_ALWAYS;
            }

            return -1;
        }
    }
}
