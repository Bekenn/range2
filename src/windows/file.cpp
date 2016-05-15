#include <stdext/file.h>

#include "platform.h"

#include <algorithm>
#include <system_error>


namespace stdext
{
    file_input_stream::file_input_stream(const path_char* path)
        : handle(::CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr))
    {
        if (handle == INVALID_HANDLE_VALUE)
            throw std::system_error(::GetLastError(), std::system_category());
    }

    file_input_stream::~file_input_stream()
    {
        ::CloseHandle(handle);
    }

    void file_input_stream::seek(seek_from from, std::ptrdiff_t offset)
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

    std::size_t file_input_stream::do_read(void* buffer, std::size_t size)
    {
        constexpr DWORD granularity = 0x1000;

        auto p = static_cast<std::uint8_t*>(buffer);
        std::size_t bytes = 0;
        while (size != 0)
        {
            DWORD count = size > MAXDWORD ? MAXDWORD & ~(granularity - 1) : size;
            DWORD dwbytes;
            if (!::ReadFile(handle, p, count, &dwbytes, nullptr))
                throw std::system_error(::GetLastError(), std::system_category());
            if (dwbytes == 0)
                break;
            size -= dwbytes;
            bytes += dwbytes;
            p += dwbytes;
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

        distance.QuadPart = ::std::min(static_cast<LONGLONG>(size), file_size.QuadPart - distance.QuadPart);
        if (!::SetFilePointerEx(handle, distance, nullptr, FILE_CURRENT))
            throw std::system_error(::GetLastError(), std::system_category());

        return static_cast<std::size_t>(distance.QuadPart);
    }
}
