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

#include <cstddef>
#include <cstdint>


namespace stdext
{
    // public classes
    class input_stream;
    class output_stream;
    class stream;
    class seekable;
    class peekable;

    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class input_stream_iterator;
    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class output_stream_iterator;

    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class stream_generator;
    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
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

    class stream_error : public ::std::runtime_error
    {
        using ::std::runtime_error::runtime_error;
    };

    class input_stream
    {
    public:
        virtual ~input_stream();

    public:
        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        POD read()
        {
            POD value;
            if (do_read(reinterpret_cast<uint8_t*>(&value), sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
            return value;
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        size_t read(POD* buffer, size_t count)
        {
            auto size = do_read(reinterpret_cast<uint8_t*>(buffer), count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, size_t length, REQUIRES(::std::is_pod<POD>::value)>
        size_t read(POD (&buffer)[length])
        {
            return read(buffer, length);
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        size_t skip(size_t count = 1)
        {
            auto size = do_skip(count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

    private:
        virtual size_t do_read(uint8_t* buffer, size_t size) = 0;
        virtual size_t do_skip(size_t size) = 0;
    };


    class output_stream
    {
    public:
        virtual ~output_stream();

    public:
        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        void write(const POD& value)
        {
            if (do_write(reinterpret_cast<const uint8_t*>(&value), sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        size_t write(const POD* buffer, size_t count)
        {
            auto size = do_write(reinterpret_cast<const uint8_t*>(buffer), count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, size_t length, REQUIRES(::std::is_pod<POD>::value)>
        size_t write(const POD (&buffer)[length])
        {
            return write(buffer, length);
        }

    private:
        virtual size_t do_write(const uint8_t* buffer, size_t size) = 0;
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
        virtual stream_position seek(seek_from from, stream_offset offset) = 0;

        stream_position position() { return seek(seek_from::current, 0); }
        virtual void set_position(stream_position position) = 0;
    };


    class peekable
    {
    public:
        virtual ~peekable();

    public:
        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        POD peek()
        {
            POD value;
            if (do_peek(&value, sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
            return value;
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        size_t peek(POD* buffer, size_t count)
        {
            auto size = do_peek(buffer, count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, size_t length, REQUIRES(::std::is_pod<POD>::value)>
        size_t peek(POD (&buffer)[length])
        {
            return peek(buffer, length);
        }

    private:
        virtual size_t do_peek(void* buffer, size_t size) = 0;
    };


    class direct_readable
    {
    public:
        virtual ~direct_readable();

    public:
        virtual size_t direct_read(std::function<size_t (const uint8_t* buffer, size_t size)> read) = 0;
    };

    class direct_writable
    {
    public:
        virtual ~direct_writable();

    public:
        virtual size_t direct_write(std::function<size_t (uint8_t* buffer, size_t size)> write) = 0;
    };

    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class input_stream_iterator
    {
    public:
        using value_type = POD;
        using difference_type = ::std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = ::std::input_iterator_tag;

    private:
        static constexpr bool noexcept_swappable()
        {
            return noexcept(swap(declval<value_type&>(), declval<value_type&>()));
        }

    public:
        input_stream_iterator() : stream(nullptr) { }
        explicit input_stream_iterator(input_stream& stream) : stream(&stream) { ++*this; }

    public:
        friend bool operator == (const input_stream_iterator& a, const input_stream_iterator& b) noexcept
        {
            if (a.stream != b.stream)
                return false;
            if (a.stream == nullptr)
                return true;
            return a.value == b.value;
        }
        friend bool operator != (const input_stream_iterator& a, const input_stream_iterator& b) noexcept
        {
            return !(a == b);
        }
        friend void swap(input_stream_iterator& a, input_stream_iterator& b)
            noexcept(input_stream_iterator::noexcept_swappable())
        {
            swap(a.stream, b.stream);
            swap(a.value, b.value);
        }

    public:
        reference operator * () const { return value; }
        pointer operator -> () const { return &value; }
        input_stream_iterator& operator ++ () { if (stream->read(&value, 1) == 0) stream = nullptr; return *this; }
        iterator_proxy<input_stream_iterator> operator ++ (int) { iterator_proxy<input_stream_iterator> proxy(move(value)); ++*this; return proxy; }

    private:
        input_stream* stream;
        POD value;
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class output_stream_iterator
    {
    public:
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using iterator_category = ::std::output_iterator_tag;

    public:
        explicit output_stream_iterator(output_stream& stream) : stream(&stream) { }

    public:
        output_stream_iterator& operator * () { return *this; }
        void operator = (const POD& value) { stream->write(value); }
        output_stream_iterator& operator ++ () { return *this; }
        output_stream_iterator& operator ++ (int) { return *this; }

        friend void swap(output_stream_iterator& a, output_stream_iterator& b) noexcept
        {
            swap(a.stream, b.stream);
        }

    private:
        output_stream* stream;
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class stream_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = POD;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        stream_generator() noexcept : stream(), value() { }
        explicit stream_generator(input_stream& stream) noexcept : stream(&stream)
        {
            next();
        }

    public:
        friend bool operator == (const stream_generator& a, const stream_generator& b) noexcept
        {
            return a.stream == b.stream && a.value == b.value;
        }
        friend bool operator != (const stream_generator& a, const stream_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(stream_generator& a, stream_generator& b) noexcept
        {
            swap(a.stream, b.stream);
            swap(a.value, b.value);
        }

    public:
        reference operator * () const noexcept { return value; }
        pointer operator -> () const noexcept { return &value; }
        stream_generator& operator ++ () { next(); return *this; }
        iterator_proxy<stream_generator> operator ++ (int)
        {
            iterator_proxy<stream_generator> proxy(value);
            ++*this;
            return proxy;
        }

        explicit operator bool () const noexcept { return stream != nullptr; }

    private:
        void next()
        {
            if (stream->read(&value, 1) == 0)
            {
                stream = nullptr;
                value = POD();
            }
        }

    private:
        input_stream* stream;
        POD value;
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class stream_consumer
    {
    public:
        using value_type = POD;

    public:
        stream_consumer() noexcept : stream() { }
        explicit stream_consumer(output_stream& stream) noexcept : stream(&stream) { }

    public:
        friend bool operator == (const stream_consumer& a, const stream_consumer& b) noexcept
        {
            return a.stream == b.stream;
        }
        friend bool operator != (const stream_consumer& a, const stream_consumer& b) noexcept
        {
            return !(a == b);
        }

    public:
        bool operator () (const value_type& value)
        {
            return stream->write(&value, 1) != 0;
        }

    private:
        output_stream* stream;
    };


    template <class CharT, class Traits>
    class basic_string_stream_consumer
    {
    public:
        basic_string_stream_consumer() noexcept : stream() { }
        explicit basic_string_stream_consumer(output_stream& stream) noexcept : stream(&stream) { }

    public:
        friend bool operator == (const basic_string_stream_consumer& a, const basic_string_stream_consumer& b) noexcept
        {
            return a.stream = b.stream;
        }
        friend bool operator != (const basic_string_stream_consumer& a, const basic_string_stream_consumer& b) noexcept
        {
            return !(a == b);
        }

    public:
        bool operator () (CharT value)
        {
            return stream->write(&value, 1) != 0;
        }

        bool operator () (basic_string_view<CharT, Traits> value)
        {
            auto count = stream->write(value.data(), value.size());
            if (count == 0)
                return false;
            if (count != value.size())
                throw stream_error("premature end of stream");
            return true;
        }

    private:
        output_stream* stream;
    };


    template <class Pointer>
    class memory_stream_base : public seekable
    {
    private:
        using _void = preserve_const_t<::std::remove_pointer_t<Pointer>, void>;

    private:
        memory_stream_base() = default; // not used
    protected:
        memory_stream_base(_void* buffer, size_t size) noexcept
            : current(static_cast<Pointer>(buffer)), first(current), last(first + size)
        {
        }
        ~memory_stream_base() override;

    public:
        void reset(_void* buffer, size_t size) noexcept
        {
            first = current = static_cast<Pointer>(buffer);
            last = first + size;
        }

    public:
        stream_position seek(seek_from from, stream_offset offset) override
        {
            Pointer location;

            switch (from)
            {
            case seek_from::begin:
                if (offset < 0)
                    throw ::std::invalid_argument("negative offset for seek_from::begin");
                location = first + offset;
                break;

            case seek_from::current:
                location = current + offset;
                break;

            case seek_from::end:
                if (offset > 0)
                    throw ::std::invalid_argument("positive offset for seek_from::end");
                location = last + offset;
                break;

            default:
                throw ::std::invalid_argument("invalid from value");
            }

            if (location < first || location > last)
                throw ::std::invalid_argument("offset out of range");

            current = location;
            return current - first;
        }

        void set_position(stream_position position) override
        {
            if (position > size_t(last - first))
                throw ::std::invalid_argument("position out of range");

            current = first + position;
        }

    private:
        template <class Stream> friend class memory_input_stream_base;
        template <class Stream> friend class memory_output_stream_base;
        Pointer current;
        Pointer first;
        Pointer last;
    };

    template <> memory_stream_base<const uint8_t*>::~memory_stream_base();
    template <> memory_stream_base<uint8_t*>::~memory_stream_base();


    template <class Stream>
    class memory_input_stream_base : public peekable, public direct_readable
    {
    protected:
        memory_input_stream_base() = default;
        ~memory_input_stream_base() override = default;

    public:
        size_t direct_read(std::function<size_t(const uint8_t* buffer, size_t size)> read) override
        {
            auto size = read(self().current, size_t(self().last - self().current));
            self().current += size;
            return size;
        }

    protected:
        size_t read_impl(uint8_t* buffer, size_t size)
        {
            size = ::std::min(size, size_t(self().last - self().current));
            ::std::copy_n(self().current, size, buffer);
            self().current += size;
            return size;
        }

        size_t skip_impl(size_t size)
        {
            size = ::std::min(size, size_t(self().last - self().current));
            self().current += size;
            return size;
        }

    private:
        size_t do_peek(void* buffer, size_t size) override
        {
            size = ::std::min(size, size_t(self().last - self().current));
            ::std::copy(self().current, self().current + size, static_cast<uint8_t*>(buffer));
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
        size_t direct_write(std::function<size_t(uint8_t* buffer, size_t size)> write) override
        {
            auto size = write(self().current, size_t(self().last - self().current));
            self().current += size;
            return size;
        }

    protected:
        size_t write_impl(const uint8_t* buffer, size_t size)
        {
            size = ::std::min(size, size_t(self().last - self().current));
            self().current = ::std::copy_n(buffer, size, self().current);
            return size;
        }

    private:
        Stream& self() noexcept { return static_cast<Stream&>(*this); }
    };


    class memory_input_stream : public memory_stream_base<const uint8_t*>, public memory_input_stream_base<memory_input_stream>, public input_stream
    {
    public:
        memory_input_stream(const void* buffer, size_t size) noexcept
            : memory_stream_base<const uint8_t*>(buffer, size)
        {
        }

        ~memory_input_stream() override;

    private:
        size_t do_read(uint8_t* buffer, size_t size) override
        {
            return read_impl(buffer, size);
        }

        size_t do_skip(size_t size) override
        {
            return skip_impl(size);
        }
    };


    class memory_output_stream : public memory_stream_base<uint8_t*>, public memory_output_stream_base<memory_output_stream>, public output_stream
    {
    public:
        memory_output_stream(void* buffer, size_t size) noexcept
            : memory_stream_base<uint8_t*>(buffer, size)
        {
        }

        ~memory_output_stream() override;

    private:
        size_t do_write(const uint8_t* buffer, size_t size) override
        {
            return write_impl(buffer, size);
        }
    };

    class memory_stream : public memory_stream_base<uint8_t*>, public memory_input_stream_base<memory_stream>, public memory_output_stream_base<memory_stream>, public stream
    {
    public:
        memory_stream(void* buffer, size_t size) noexcept
            : memory_stream_base<uint8_t*>(buffer, size)
        {
        }

        ~memory_stream() override;

    private:
        size_t do_read(uint8_t* buffer, size_t size) override
        {
            return read_impl(buffer, size);
        }

        size_t do_write(const uint8_t* buffer, size_t size) override
        {
            return write_impl(buffer, size);
        }

        size_t do_skip(size_t size) override
        {
            return skip_impl(size);
        }
    };
}

#endif
