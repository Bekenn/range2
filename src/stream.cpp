//
//  stream.cpp
//  stdext
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#include <stdext/stream.h>

namespace stdext
{
    input_stream::~input_stream() = default;
    output_stream::~output_stream() = default;
    stream::~stream() = default;
    seekable::~seekable() = default;
    peekable::~peekable() = default;
    direct_readable::~direct_readable() = default;
    direct_writable::~direct_writable() = default;
    template <> memory_stream_base<const byte*>::~memory_stream_base() = default;
    template <> memory_stream_base<byte*>::~memory_stream_base() = default;
    memory_input_stream::~memory_input_stream() = default;
    memory_output_stream::~memory_output_stream() = default;
    memory_stream::~memory_stream() = default;
    substream::~substream() = default;

    stream_position seekable::seek(seek_from from, stream_offset offset)
    {
        stream_position p;

        switch (from)
        {
        case seek_from::begin:
            p = offset;
            break;
        case seek_from::current:
            p = position() + offset;
            break;
        case seek_from::end:
            p = end_position() + offset;
            break;
        default:
            unreachable();
        }

        set_position(p);
        return p;
    }

    extern string_stream_consumer& strout()
    {
        static string_stream_consumer strout(out());
        return strout;
    }

    extern string_stream_consumer& strerr()
    {
        static string_stream_consumer strerr(err());
        return strerr;
    }

    extern wstring_stream_consumer& wstrout()
    {
        static wstring_stream_consumer wstrout(out());
        return wstrout;
    }

    extern wstring_stream_consumer& wstrerr()
    {
        static wstring_stream_consumer wstrerr(err());
        return wstrerr;
    }

    extern u16string_stream_consumer& u16strout()
    {
        static u16string_stream_consumer u16strout(out());
        return u16strout;
    }

    extern u16string_stream_consumer& u16strerr()
    {
        static u16string_stream_consumer u16strerr(err());
        return u16strerr;
    }

    extern u32string_stream_consumer& u32strout()
    {
        static u32string_stream_consumer u32strout(out());
        return u32strout;
    }

    extern u32string_stream_consumer& u32strerr()
    {
        static u32string_stream_consumer u32strerr(err());
        return u32strerr;
    }
}
