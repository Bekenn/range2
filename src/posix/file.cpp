#include <stdext/file.h>
#include <stdext/unicode.h>

#include <algorithm>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


namespace stdext
{
    namespace
    {
        class std_input_stream : public detail::file_input_stream_base<std_input_stream>
        {
        public:
            explicit std_input_stream(int fd) : handle(fd) { }

        public:
            bool is_open() const noexcept { return true; }

        private:
            template <class Stream> friend class detail::file_input_stream_base;
            file_handle_t handle;
        };

        class std_output_stream : public detail::file_output_stream_base<std_output_stream>
        {
        public:
            explicit std_output_stream(int fd) : handle(fd) { }

        public:
            bool is_open() const noexcept { return true; }

        private:
            template <class Stream> friend class detail::file_output_stream_base;
            file_handle_t handle;
        };

        int creation_disposition(flags<file_open_flags> flags);
    }

    namespace detail
    {
        file_stream_base::file_stream_base() : handle(-1)
        {
        }

        file_stream_base::file_stream_base(file_stream_base&& other) : handle(move(other.handle))
        {
            other.handle = -1;
        }

        file_stream_base& file_stream_base::operator = (file_stream_base&& other)
        {
            if (is_open())
                close();

            handle = move(other.handle);
            other.handle = -1;
            return *this;
        }

        file_stream_base::~file_stream_base()
        {
            ::close(handle);
        }

        file_stream_base::file_stream_base(file_handle_t handle) : handle(handle)
        {
            if (handle == -1)
                throw std::system_error(errno, std::generic_category());
        }

        bool file_stream_base::is_open() const noexcept
        {
            return handle != -1;
        }

        void file_stream_base::close() noexcept
        {
            assert(is_open());

            ::close(handle);
            handle = -1;
        }

        stream_position file_stream_base::position() const
        {
            assert(is_open());

            auto pos = ::lseek(handle, 0, SEEK_CUR);
            if (pos == -1)
                throw std::system_error(errno, std::generic_category());

            return pos;
        }

        stream_position file_stream_base::end_position() const
        {
            assert(is_open());

            struct stat st;
            if (::fstat(handle, &st) == -1)
                throw std::system_error(errno, std::generic_category());

            return st.st_size;
        }

        void file_stream_base::set_position(stream_position position)
        {
            assert(is_open());

            auto pos = ::lseek(handle, position, SEEK_SET);
            if (pos == -1)
                throw std::system_error(errno, std::generic_category());
        }

        template <class Stream>
        size_t file_input_stream_base<Stream>::do_read(uint8_t* buffer, size_t size)
        {
            assert(self().is_open());

            auto bytes = ::read(self().handle, buffer, size);
            if (bytes == -1)
                throw std::system_error(errno, std::generic_category());

            return bytes;
        }

        template <class Stream>
        size_t file_input_stream_base<Stream>::do_skip(size_t size)
        {
            assert(self().is_open());

            struct stat st;
            if (fstat(self().handle, &st) == -1)
                throw std::system_error(errno, std::generic_category());

            auto distance = lseek(self().handle, 0, SEEK_CUR);
            if (distance == -1)
                throw std::system_error(errno, std::generic_category());

            distance = std::min(size, size_t(st.st_size - distance));
            if (lseek(self().handle, distance, SEEK_CUR) == -1)
                throw std::system_error(errno, std::generic_category());

            return static_cast<size_t>(distance);
        }

        template <class Stream>
        size_t file_output_stream_base<Stream>::do_write(const uint8_t* buffer, size_t size)
        {
            assert(self().is_open());

            auto bytes = ::write(self().handle, buffer, size);
            if (bytes == -1)
                throw std::system_error(errno, std::generic_category());

            return bytes;
        }
    }

    file_input_stream::file_input_stream(const path_char* path)
        : file_stream_base(::open(path, O_RDONLY))
    {
    }

    file_input_stream::file_input_stream(const char* path, utf8_path_encoding)
        : file_input_stream(path)
    {
    }

    file_input_stream::~file_input_stream() = default;

    std::error_code file_input_stream::open(const path_char* path)
    {
        assert(!is_open());

        handle = ::open(path, O_RDONLY);
        if (handle == -1)
            return { errno, std::generic_category() };

        return { };
    }

    std::error_code file_input_stream::open(const char* path, utf8_path_encoding)
    {
        return open(path);
    }

    file_output_stream::file_output_stream(const path_char* path, flags<file_open_flags> flags)
        : file_stream_base(::open(path, O_WRONLY | creation_disposition(flags), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))
    {
    }

    file_output_stream::file_output_stream(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
        : file_output_stream(path, flags)  // TODO: Exception on encoding error
    {
    }

    file_output_stream::~file_output_stream() = default;

    std::error_code file_output_stream::open(const path_char* path, flags<file_open_flags> flags)
    {
        assert(!is_open());

        handle = ::open(path, O_WRONLY | creation_disposition(flags), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (handle == -1)
            return { errno, std::generic_category() };

        return { };
    }

    std::error_code file_output_stream::open(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
    {
        return open(path, flags);
    }

    file_stream::file_stream(const path_char* path, flags<file_open_flags> flags)
        : file_stream_base(::open(path, O_RDWR | creation_disposition(flags), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))
    {
    }

    file_stream::file_stream(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
        : file_stream(path, flags)
    {
    }

    file_stream::~file_stream() = default;

    std::error_code file_stream::open(const path_char* path, flags<file_open_flags> flags)
    {
        assert(!is_open());

        handle = ::open(path, O_RDWR | creation_disposition(flags), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (handle == -1)
            return { errno, std::generic_category() };

        return { };
    }

    std::error_code file_stream::open(const char* path, utf8_path_encoding, flags<file_open_flags> flags)
    {
        return open(path, flags);
    }

    input_stream& in()
    {
        static std_input_stream in(STDIN_FILENO);
        return in;
    }

    output_stream& out()
    {
        static std_output_stream out(STDOUT_FILENO);
        return out;
    }

    output_stream& err()
    {
        static std_output_stream err(STDERR_FILENO);
        return err;
    }

    namespace
    {
        int creation_disposition(flags<file_open_flags> flags)
        {
            // The static_cast is here to work around a Visual Studio 2015 bug.
            switch (static_cast<file_open_flags>(flags))
            {
            case file_open_flags::none:
                return 0;
            case file_open_flags::create:
                return O_CREAT;
            case file_open_flags::create_exclusive:
                return O_CREAT | O_EXCL;
            case file_open_flags::truncate:
                return O_TRUNC;
            case make_flags(file_open_flags::create, file_open_flags::truncate):
                return O_CREAT | O_TRUNC;
            }

            return -1;
        }
    }
}
