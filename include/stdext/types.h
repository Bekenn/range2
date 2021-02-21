//
//  file.h
//  stdext
//
//  Created by James Touton on 5/14/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_TYPES_INCLUDED
#define STDEXT_TYPES_INCLUDED
#pragma once

#include <stdext/detail/config.h>

#include <cstddef>
#include <cstdint>


namespace stdext
{
    // Types imported from cstddef
    using std::ptrdiff_t;
    using std::size_t;
    using std::max_align_t;
    using std::nullptr_t;

    // Types imported from cstdint
    using std::int8_t;
    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::intmax_t;
    using std::intptr_t;

    using std::uint8_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::uintmax_t;
    using std::uintptr_t;
}

#endif
