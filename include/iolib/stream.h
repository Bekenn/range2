//
//  stream.h
//  iolib
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_STREAM_INCLUDED
#define IOLIB_STREAM_INCLUDED
#pragma once

#include <iolib/range.h>

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>

#include <cstddef>
#include <cstdint>


namespace iolib
{
    // public classes
    class input_stream;
    class output_stream;
    class seekable;
    class peekable;

    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class input_stream_iterator;
    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class output_stream_iterator;

    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class input_stream_range;
    template <class POD, REQUIRES(::std::is_pod<POD>::value)>
    class output_stream_range;

    class memory_input_stream;
    class memory_output_stream;

    class stream_error : public ::std::runtime_error
    {
        using ::std::runtime_error::runtime_error;
    };

    // Streams are not copyable or movable.
    class stream_base
    {
    public:
        stream_base() = default;
        stream_base(const stream_base&) = delete;
        stream_base& operator = (const stream_base&) = delete;
        stream_base(stream_base&&) = delete;
        stream_base& operator = (stream_base&&) = delete;
        virtual ~stream_base();
    };


    class input_stream : public stream_base
    {
    public:
        ~input_stream() override;

    public:
        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        POD read()
        {
            POD value;
            if (do_read(&value, sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
            return value;
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        ::std::size_t read(POD* buffer, ::std::size_t count)
        {
            auto size = do_read(buffer, count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, ::std::size_t length, REQUIRES(::std::is_pod<POD>::value)>
        ::std::size_t read(POD (&buffer)[length])
        {
            return read(buffer, length);
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        ::std::size_t skip(::std::size_t count = 1)
        {
            auto size = do_skip(count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

    private:
        virtual ::std::size_t do_read(void* buffer, ::std::size_t size) = 0;
        virtual ::std::size_t do_skip(::std::size_t size) = 0;
    };


    class output_stream : public stream_base
    {
    public:
        ~output_stream() override;

    public:
        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        void write(const POD& value)
        {
            if (do_write(&value, sizeof(POD)) != sizeof(POD))
                throw stream_error("premature end of stream");
        }

        template <class POD, REQUIRES(::std::is_pod<POD>::value)>
        ::std::size_t write(const POD* buffer, ::std::size_t count)
        {
            auto size = do_write(buffer, count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, ::std::size_t length, REQUIRES(::std::is_pod<POD>::value)>
        ::std::size_t write(const POD (&buffer)[length])
        {
            return write(buffer, length);
        }

    private:
        virtual ::std::size_t do_write(const void* buffer, ::std::size_t size) = 0;
    };


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
        virtual void seek(seek_from from, ::std::ptrdiff_t offset) = 0;
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
        ::std::size_t peek(POD* buffer, ::std::size_t count)
        {
            auto size = do_peek(buffer, count * sizeof(POD));
            if (size % sizeof(POD) != 0)
                throw stream_error("premature end of stream");
            return size / sizeof(POD);
        }

        template <class POD, ::std::size_t length, REQUIRES(::std::is_pod<POD>::value)>
        ::std::size_t peek(POD (&buffer)[length])
        {
            return peek(buffer, length);
        }

    private:
        virtual ::std::size_t do_peek(void* buffer, ::std::size_t size) = 0;
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class input_stream_iterator : public ::std::iterator<::std::input_iterator_tag, const POD>
    {
    public:
        using typename ::std::iterator<::std::input_iterator_tag, const POD>::value_type;
        using typename ::std::iterator<::std::input_iterator_tag, const POD>::difference_type;
        using typename ::std::iterator<::std::input_iterator_tag, const POD>::pointer;
        using typename ::std::iterator<::std::input_iterator_tag, const POD>::reference;
        using typename ::std::iterator<::std::input_iterator_tag, const POD>::iterator_category;

        struct value_proxy
        {
            value_type value;
            reference operator * () { return value; }
        };

    public:
        input_stream_iterator() : stream(nullptr) { }
        explicit input_stream_iterator(input_stream& stream) : stream(&stream) { ++*this; }

    public:
        reference operator * () const { return value; }
        pointer operator -> () const { return &value; }
        input_stream_iterator& operator ++ () { if (stream->read(&value, 1) == 0) stream = nullptr; return *this; }
        value_proxy operator ++ (int) { value_proxy proxy = { value }; ++*this; return proxy; }

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
            using ::std::swap;
            swap(a.stream, b.stream);
            swap(a.value, b.value);
        }

    private:
        static constexpr bool noexcept_swappable()
        {
            using ::std::swap;
            return noexcept(swap(::std::declval<value_type>(), ::std::declval<value_type>()));
        }

    private:
        input_stream* stream;
        POD value;
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class output_stream_iterator : ::std::iterator<::std::output_iterator_tag, void, void, void, void>
    {
    public:
        using typename ::std::iterator<::std::output_iterator_tag, void, void, void, void>::iterator_category;

    public:
        explicit output_stream_iterator(output_stream& stream) : stream(&stream) { }

    public:
        output_stream_iterator& operator * () { return *this; }
        void operator = (const POD& value) { stream->write(value); }
        output_stream_iterator& operator ++ () { return *this; }
        output_stream_iterator& operator ++ (int) { return *this; }

        friend void swap(output_stream_iterator& a, output_stream_iterator& b) noexcept
        {
            using ::std::swap;
            swap(a.stream, b.stream);
        }

    private:
        output_stream* stream;
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class input_stream_range : public iterator_range<input_stream_iterator<POD>>
    {
    public:
        using iterator = typename iterator_range<input_stream_iterator<POD>>::iterator;

    public:
        input_stream_range() = default;
        input_stream_range(input_stream& stream)
            : iterator_range<iterator>(iterator(stream), iterator())
        {
        }
    };


    template <class POD, REQUIRED(::std::is_pod<POD>::value)>
    class output_stream_range : public iterator_range<output_stream_iterator<POD>>
    {
    public:
        using iterator = typename iterator_range<output_stream_iterator<POD>>::iterator;

    public:
        output_stream_range() = default;
        output_stream_range(output_stream& stream)
            : iterator_range<iterator>(iterator(stream), iterator())
        {
        }
    };


    template <class Stream>
    class memory_stream_base : public seekable
    {
    public:
        ~memory_stream_base() override = default;

    public:
        void seek(seek_from from, ::std::ptrdiff_t offset) override
        {
            switch (from)
            {
                case seek_from::begin:
                    if (offset < 0)
                        throw ::std::invalid_argument("negative offset for seek_from::begin");
                    if (offset > self().last - self().first)
                        throw ::std::invalid_argument("offset out of range");
                    self().current = self().first + offset;
                    break;

                case seek_from::current:
                    if (offset < self().first - self().current || offset > self().last - self().current)
                        throw ::std::invalid_argument("offset out of range");
                    self().current += offset;
                    break;

                case seek_from::end:
                    if (offset > 0)
                        throw ::std::invalid_argument("positive offset for seek_from::end");
                    if (offset < self().first - self().last)
                        throw ::std::invalid_argument("offset out of range");
                    self().current = self().last + offset;
                    break;
            }
        }

    private:
        Stream& self() { return static_cast<Stream&>(*this); }
    };


    class memory_input_stream : public input_stream, public memory_stream_base<memory_input_stream>, public peekable
    {
    public:
        memory_input_stream(const void* buffer, ::std::size_t size) noexcept
            : current(static_cast<const ::std::uint8_t*>(buffer)), first(current), last(first + size)
        {
        }

        ~memory_input_stream() override;

    public:
        void reset(const void* buffer, ::std::size_t size) noexcept
        {
            first = current = static_cast<const uint8_t*>(buffer);
            last = first + size;
        }

    private:
        ::std::size_t do_read(void* buffer, ::std::size_t size) override
        {
            size = ::std::min(size, ::std::size_t(last - current));
            ::std::copy(current, current + size, static_cast<::std::uint8_t*>(buffer));
            current += size;
            return size;
        }

        ::std::size_t do_skip(::std::size_t size) override
        {
            size = ::std::min(size, ::std::size_t(last - current));
            current += size;
            return size;
        }
        
        ::std::size_t do_peek(void* buffer, ::std::size_t size) override
        {
            size = ::std::min(size, ::std::size_t(last - current));
            ::std::copy(current, current + size, static_cast<::std::uint8_t*>(buffer));
            return size;
        }

    private:
        friend class memory_stream_base<memory_input_stream>;
        const ::std::uint8_t* current;
        const ::std::uint8_t* first;
        const ::std::uint8_t* last;
    };


    class memory_output_stream : public output_stream, public memory_stream_base<memory_output_stream>
    {
    public:
        memory_output_stream(void* buffer, ::std::size_t size) noexcept
            : current(static_cast<::std::uint8_t*>(buffer)), first(current), last(first + size)
        {
        }

        ~memory_output_stream() override;

    public:
        void reset(void* buffer, ::std::size_t size) noexcept
        {
            first = current = static_cast<uint8_t*>(buffer);
            last = first + size;
        }

    private:
        ::std::size_t do_write(const void* buffer, ::std::size_t size) override
        {
            size = ::std::min(size, ::std::size_t(last - current));
            auto p = static_cast<const ::std::uint8_t*>(buffer);
            current = ::std::copy(p, p + size, current);
            return size;
        }

    private:
        friend class memory_stream_base<memory_output_stream>;
        ::std::uint8_t* current;
        ::std::uint8_t* first;
        ::std::uint8_t* last;
    };
}

#endif
