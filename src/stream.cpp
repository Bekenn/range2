//
//  stream.cpp
//  iolib
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#include <iolib/stream.h>

namespace iolib
{
    stream_base::~stream_base() = default;
    input_stream::~input_stream() = default;
    output_stream::~output_stream() = default;
    seekable::~seekable() = default;
    peekable::~peekable() = default;
    memory_input_stream::~memory_input_stream() = default;
    memory_output_stream::~memory_output_stream() = default;
}
