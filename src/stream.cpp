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
    template <> memory_stream_base<const uint8_t*>::~memory_stream_base() = default;
    template <> memory_stream_base<uint8_t*>::~memory_stream_base() = default;
    memory_input_stream::~memory_input_stream() = default;
    memory_output_stream::~memory_output_stream() = default;
    memory_stream::~memory_stream() = default;
}
