//
//  stream.h
//  stdext
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_STREAM_INCLUDED
#define STDEXT_STREAM_INCLUDED
#pragma once

#include <stdext/generator.h>
#include <stdext/string_view.h>

#include <algorithm>
#include <functional>
#include <stdexcept>


namespace stdext
{
    // public classes
    class input_stream;
    class output_stream;
    class stream;
    class seekable;
    class peekable;

    template <class POD>
    class input_stream_iterator;
    template <class POD>
    class output_stream_iterator;

    template <class POD>
    class stream_generator;
    template <class POD>
    class stream_consumer;

    template <class CharT, class Traits = std::char_traits<CharT>>
    class basic_string_stream_consumer;
    using string_stream_consumer = basic_string_stream_consumer<char>;
    using wstring_stream_consumer = basic_string_stream_consumer<wchar_t>;
    using u16string_stream_consumer = basic_string_stream_consumer<char16_t>;
    using u32string_stream_consumer = basic_string_stream_consumer<char32_t>;

    template <class Pointer> class memory_stream_base;
    template <class Stream> class memory_input_stream_base;
    template <class Stream> class memory_output_stream_base;
    class memory_input_stream;
    class memory_output_stream;
    class memory_stream;

    class substream;

    input_stream& in();
    output_stream& out();
    output_stream& err();

    string_stream_consumer& strout();
    string_stream_consumer& strerr();
    wstring_stream_consumer& wstrout();
    wstring_stream_consumer& wstrerr();
    u16string_stream_consumer& u16strout();
    u16string_stream_consumer& u16strerr();
    u32string_stream_consumer& u32strout();
    u32string_stream_consumer& u32strerr();

    class stream_error : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    class input_stream
    {
    public:
        virtual ~input_stream();

    public:
        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] POD read()
        {
            POD value;
            if (do_read(reinterpret_cast<std::byte*>(&value), sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
            return value;
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t read(POD* buffer, size_t count)
        {
            auto size = do_read(reinterpret_cast<std::byte*>(buffer), count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, size_t length, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t read(POD (&buffer)[length])
        {
            return read(buffer, length);
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void read_all(POD* buffer, size_t count)
        {
            auto size = do_read(reinterpret_cast<std::byte*>(buffer), count * sizeof(POD));
            if (size != count * sizeof(POD))
                throw stream_error("premature end of stream");
        }

        template <class POD, size_t length, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void read_all(POD (&buffer)[length])
        {
            read_all(buffer, length);
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void skip()
        {
            auto size = do_skip(sizeof(POD));
            if (size != sizeof(POD))
                throw stream_error("premature end of stream");
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t skip(size_t count)
        {
            auto size = do_skip(count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void skip_all(size_t count)
        {
            auto size = do_skip(count * sizeof(POD));
            if (size != count * sizeof(POD))
                throw stream_error("premature end of stream");
        }

    private:
        [[nodiscard]] virtual size_t do_read(std::byte* buffer, size_t size) = 0;
        [[nodiscard]] virtual size_t do_skip(size_t size) = 0;
    };


    class output_stream
    {
    public:
        virtual ~output_stream();

    public:
        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void write(const POD& value)
        {
            if (do_write(reinterpret_cast<const std::byte*>(&value), sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t write(const POD* buffer, size_t count)
        {
            auto size = do_write(reinterpret_cast<const std::byte*>(buffer), count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, size_t length, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t write(const POD (&buffer)[length])
        {
            return write(buffer, length);
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void write_all(const POD* buffer, size_t count)
        {
            auto size = do_write(reinterpret_cast<const std::byte*>(buffer), count * sizeof(POD));
            if (size != count * sizeof(POD))
                throw stream_error("premature end of stream");
        }

        template <class POD, size_t length, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        void write_all(const POD(&buffer)[length])
        {
            write_all(buffer, length);
        }

    private:
        [[nodiscard]] virtual size_t do_write(const std::byte* buffer, size_t size) = 0;
    };

    class stream : public input_stream, public output_stream
    {
    public:
        ~stream() override;
    };

    using stream_position = uintmax_t;
    using stream_offset = intmax_t;

    enum class seek_from
    {
        begin,
        current,
        end
    };

    class seekable
    {
    public:
        virtual ~seekable();

    public:
        virtual stream_position position() const = 0;
        virtual stream_position end_position() const = 0;

        virtual void set_position(stream_position position) = 0;

        stream_position seek(seek_from from, stream_offset offset);
    };


    class peekable
    {
    public:
        virtual ~peekable();

    public:
        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] POD peek()
        {
            POD value;
            if (do_peek(reinterpret_cast<std::byte*>(&value), sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
            return value;
        }

        template <class POD, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t peek(POD* buffer, size_t count)
        {
            auto size = do_peek(reinterpret_cast<std::byte*>(buffer), count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, size_t length, STDEXT_REQUIRES(std::is_trivially_copyable_v<POD>)>
        [[nodiscard]] size_t peek(POD (&buffer)[length])
        {
            return peek(buffer, length);
        }

    private:
        [[nodiscard]] virtual size_t do_peek(std::byte* buffer, size_t size) = 0;
    };


    class direct_readable
    {
    public:
        virtual ~direct_readable();

    public:
        [[nodiscard]] virtual size_t direct_read(std::function<size_t (const std::byte* buffer, size_t size)> read) = 0;
    };

    class direct_writable
    {
    public:
        virtual ~direct_writable();

    public:
        [[nodiscard]] virtual size_t direct_write(std::function<size_t (std::byte* buffer, size_t size)> write) = 0;
    };

    template <class POD>
    class input_stream_iterator
    {
        static_assert(std::is_default_constructible_v<POD>);
        static_assert(std::is_copy_constructible_v<POD>);
        static_assert(std::is_copy_assignable_v<POD>);
        static_assert(std::is_trivially_copyable_v<POD>);

    public:
        using value_type = POD;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::input_iterator_tag;

    private:
        static constexpr bool noexcept_swappable = noexcept(swap(declval<value_type&>(), declval<value_type&>()));

    public:
        input_stream_iterator() : _stream(nullptr) { }
        explicit input_stream_iterator(input_stream& stream) : _stream(&stream) { ++*this; }

    public:
        friend bool operator == (const input_stream_iterator& a, const input_stream_iterator& b) noexcept
        {
            if (a._stream != b._stream)
                return false;
            if (a._stream == nullptr)
                return true;
            return a._value == b._value;
        }
        friend bool operator != (const input_stream_iterator& a, const input_stream_iterator& b) noexcept
        {
            return !(a == b);
        }
        friend void swap(input_stream_iterator& a, input_stream_iterator& b)
            noexcept(input_stream_iterator::noexcept_swappable)
        {
            swap(a._stream, b._stream);
            swap(a._value, b._value);
        }

    public:
        reference operator * () const { return _value; }
        pointer operator -> () const { return &_value; }
        input_stream_iterator& operator ++ () { if (_stream->read(&_value, 1) == 0) _stream = nullptr; return *this; }
        iterator_proxy<input_stream_iterator> operator ++ (int) { iterator_proxy<input_stream_iterator> proxy(stdext::move(_value)); ++*this; return proxy; }

    private:
        input_stream* _stream;
        POD _value;
    };


    template <class POD>
    class output_stream_iterator
    {
        static_assert(std::is_trivially_copyable_v<POD>);

    public:
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using iterator_category = std::output_iterator_tag;

    public:
        explicit output_stream_iterator(output_stream& stream) : _stream(&stream) { }

    public:
        output_stream_iterator& operator * () { return *this; }
        void operator = (const POD& value) { _stream->write(value); }
        output_stream_iterator& operator ++ () { return *this; }
        output_stream_iterator& operator ++ (int) { return *this; }

        friend void swap(output_stream_iterator& a, output_stream_iterator& b) noexcept
        {
            swap(a._stream, b._stream);
        }

    private:
        output_stream* _stream;
    };


    template <class POD>
    class stream_generator
    {
        static_assert(std::is_default_constructible_v<POD>);
        static_assert(std::is_copy_constructible_v<POD>);
        static_assert(std::is_copy_assignable_v<POD>);
        static_assert(std::is_trivially_copyable_v<POD>);

    public:
        using iterator_category = generator_tag;
        using value_type = POD;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        stream_generator() noexcept : _stream(), _value() { }
        explicit stream_generator(input_stream& stream) noexcept : _stream(&stream)
        {
            next();
        }

    public:
        friend bool operator == (const stream_generator& a, const stream_generator& b) noexcept
        {
            return a._stream == b._stream && a._value == b._value;
        }
        friend bool operator != (const stream_generator& a, const stream_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(stream_generator& a, stream_generator& b) noexcept
        {
            swap(a._stream, b._stream);
            swap(a._value, b._value);
        }

    public:
        reference operator * () const noexcept { return _value; }
        pointer operator -> () const noexcept { return &_value; }
        stream_generator& operator ++ () { next(); return *this; }
        iterator_proxy<stream_generator> operator ++ (int)
        {
            iterator_proxy<stream_generator> proxy(_value);
            ++*this;
            return proxy;
        }

        explicit operator bool () const noexcept { return _stream != nullptr; }

    private:
        void next()
        {
            if (_stream->read(&_value, 1) == 0)
            {
                _stream = nullptr;
                _value = POD();
            }
        }

    private:
        input_stream* _stream;
        POD _value;
    };


    template <class POD>
    class stream_consumer
    {
        static_assert(std::is_trivially_copyable_v<POD>);

    public:
        using value_type = POD;

    public:
        stream_consumer() noexcept : _stream() { }
        explicit stream_consumer(output_stream& stream) noexcept : _stream(&stream) { }

    public:
        friend bool operator == (const stream_consumer& a, const stream_consumer& b) noexcept
        {
            return a._stream == b._stream;
        }
        friend bool operator != (const stream_consumer& a, const stream_consumer& b) noexcept
        {
            return !(a == b);
        }

    public:
        [[nodiscard]] bool operator () (const value_type& value)
        {
            return _stream->write(&value, 1) != 0;
        }

    private:
        output_stream* _stream;
    };


    template <class CharT, class Traits>
    class basic_string_stream_consumer
    {
    public:
        basic_string_stream_consumer() noexcept : _stream() { }
        explicit basic_string_stream_consumer(output_stream& stream) noexcept : _stream(&stream) { }

    public:
        friend bool operator == (const basic_string_stream_consumer& a, const basic_string_stream_consumer& b) noexcept
        {
            return a._stream = b._stream;
        }
        friend bool operator != (const basic_string_stream_consumer& a, const basic_string_stream_consumer& b) noexcept
        {
            return !(a == b);
        }

    public:
        [[nodiscard]] bool operator () (CharT value)
        {
            return _stream->write(&value, 1) != 0;
        }

        [[nodiscard]] bool operator () (basic_string_view<CharT, Traits> value)
        {
            auto count = _stream->write(value.data(), value.size());
            if (count == 0)
                return false;
            if (count != value.size())
                throw stream_error("premature end of stream");
            return true;
        }

    private:
        output_stream* _stream;
    };


    template <class Pointer>
    class memory_stream_base : public seekable
    {
    private:
        using byte = preserve_const_t<std::remove_pointer_t<Pointer>, std::byte>;

    protected:
        memory_stream_base() = default;
        explicit memory_stream_base(byte* buffer, size_t size) noexcept
            : _current(static_cast<Pointer>(buffer)), _first(_current), _last(_first + size)
        {
        }
        ~memory_stream_base() override;

    public:
        void reset() noexcept
        {
            _first = _current = _last = nullptr;
        }

        void reset(byte* buffer, size_t size) noexcept
        {
            _first = _current = static_cast<Pointer>(buffer);
            _last = _first + size;
        }

    public:
        stream_position position() const final
        {
            return _current - _first;
        }

        stream_position end_position() const final
        {
            return _last - _first;
        }

        void set_position(stream_position position) final
        {
            if (position > size_t(_last - _first))
                throw std::invalid_argument("position out of range");

            _current = _first + position;
        }

    private:
        template <class Stream> friend class memory_input_stream_base;
        template <class Stream> friend class memory_output_stream_base;
        Pointer _current = nullptr;
        Pointer _first = nullptr;
        Pointer _last = nullptr;
    };

    template <> memory_stream_base<const std::byte*>::~memory_stream_base();
    template <> memory_stream_base<std::byte*>::~memory_stream_base();


    template <class Stream>
    class memory_input_stream_base : public peekable, public direct_readable
    {
    protected:
        memory_input_stream_base() = default;
        ~memory_input_stream_base() override = default;

    public:
        [[nodiscard]] size_t direct_read(std::function<size_t(const std::byte* buffer, size_t size)> read) final
        {
            auto size = read(self()._current, size_t(self()._last - self()._current));
            self()._current += size;
            return size;
        }

    protected:
        [[nodiscard]] size_t read_impl(std::byte* buffer, size_t size)
        {
            size = std::min(size, size_t(self()._last - self()._current));
            std::copy_n(self()._current, size, buffer);
            self()._current += size;
            return size;
        }

        [[nodiscard]] size_t skip_impl(size_t size)
        {
            size = std::min(size, size_t(self()._last - self()._current));
            self()._current += size;
            return size;
        }

    private:
        [[nodiscard]] size_t do_peek(std::byte* buffer, size_t size) final
        {
            size = std::min(size, size_t(self()._last - self()._current));
            std::copy(self()._current, self()._current + size, buffer);
            return size;
        }

    private:
        Stream& self() noexcept { return static_cast<Stream&>(*this); }
    };


    template <class Stream>
    class memory_output_stream_base : public direct_writable
    {
    protected:
        memory_output_stream_base() = default;
        ~memory_output_stream_base() override = default;

    public:
        [[nodiscard]] size_t direct_write(std::function<size_t(std::byte* buffer, size_t size)> write) final
        {
            auto size = write(self()._current, size_t(self()._last - self()._current));
            self()._current += size;
            return size;
        }

    protected:
        [[nodiscard]] size_t write_impl(const std::byte* buffer, size_t size)
        {
            size = std::min(size, size_t(self()._last - self()._current));
            self()._current = std::copy_n(buffer, size, self()._current);
            return size;
        }

    private:
        Stream& self() noexcept { return static_cast<Stream&>(*this); }
    };


    class memory_input_stream : public memory_stream_base<const std::byte*>, public memory_input_stream_base<memory_input_stream>, public input_stream
    {
    public:
        memory_input_stream() = default;

        explicit memory_input_stream(const std::byte* buffer, size_t size) noexcept
            : memory_stream_base<const std::byte*>(buffer, size)
        {
        }

        ~memory_input_stream() override;

    private:
        [[nodiscard]] size_t do_read(std::byte* buffer, size_t size) final
        {
            return read_impl(buffer, size);
        }

        [[nodiscard]] size_t do_skip(size_t size) final
        {
            return skip_impl(size);
        }
    };


    class memory_output_stream : public memory_stream_base<std::byte*>, public memory_output_stream_base<memory_output_stream>, public output_stream
    {
    public:
        memory_output_stream() = default;

        explicit memory_output_stream(std::byte* buffer, size_t size) noexcept
            : memory_stream_base<std::byte*>(buffer, size)
        {
        }

        ~memory_output_stream() override;

    private:
        [[nodiscard]] size_t do_write(const std::byte* buffer, size_t size) final
        {
            return write_impl(buffer, size);
        }
    };

    class memory_stream : public memory_stream_base<std::byte*>, public memory_input_stream_base<memory_stream>, public memory_output_stream_base<memory_stream>, public stream
    {
    public:
        memory_stream() = default;

        explicit memory_stream(std::byte* buffer, size_t size) noexcept
            : memory_stream_base<std::byte*>(buffer, size)
        {
        }

        ~memory_stream() override;

    private:
        [[nodiscard]] size_t do_read(std::byte* buffer, size_t size) final
        {
            return read_impl(buffer, size);
        }

        [[nodiscard]] size_t do_write(const std::byte* buffer, size_t size) final
        {
            return write_impl(buffer, size);
        }

        [[nodiscard]] size_t do_skip(size_t size) final
        {
            return skip_impl(size);
        }
    };

    class substream : public input_stream
    {
    public:
        substream() = default;
        substream(const substream&) = delete;
        substream& operator = (const substream&) = delete;

        substream(substream&& other) noexcept
            : _stream(stdext::exchange(other._stream, nullptr)), _max_extent(stdext::exchange(other._max_extent, 0))
        {
        }

        substream& operator = (substream&& other) noexcept
        {
            _stream = stdext::exchange(other._stream, nullptr);
            _max_extent = stdext::exchange(other._max_extent, 0);
            return *this;
        }

        explicit substream(input_stream& stream, size_t max_extent) noexcept
            : _stream(&stream), _max_extent(max_extent)
        {
        }

        ~substream() override;

    public:
        bool is_attached() const noexcept { return _stream != nullptr; }

        void attach(input_stream& stream, size_t max_extent) noexcept
        {
            _stream = &stream;
            _max_extent = max_extent;
        }

        void detach() noexcept
        {
            _stream = nullptr;
            _max_extent = 0;
        }

    private:
        [[nodiscard]] size_t do_read(std::byte* buffer, size_t size) final
        {
            assert(is_attached());
            auto bytes = _stream->read(buffer, min(size, _max_extent));
            _max_extent -= bytes;
            return bytes;
        }

        [[nodiscard]] size_t do_skip(size_t size) final
        {
            assert(is_attached());
            auto bytes = _stream->skip<std::byte>(min(size, _max_extent));
            _max_extent -= bytes;
            return bytes;
        }

    private:
        input_stream* _stream = nullptr;
        size_t _max_extent = 0;
    };
}

#endif
