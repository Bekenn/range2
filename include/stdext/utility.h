//
//  utility.h
//  stdext
//
//  Created by James Touton on 12/14/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_UTILITY_INCLUDED
#define STDEXT_UTILITY_INCLUDED
#pragma once

#include <stdext/types.h>

#include <utility>


#define STDEXT_CONCAT_(a, b) a ## b
#define STDEXT_CONCAT(a, b) STDEXT_CONCAT_(a, b)

namespace stdext
{
    // Useful declarations
    using std::swap;
    using std::move;
    using std::move_if_noexcept;
    using std::forward;
    using std::exchange;
    using std::declval;

    // Discards a value; can be used to silence warnings about unused entities.
    template <class... Ts>
    constexpr void discard(Ts&&...) noexcept
    {
    }

    // Returns the length of an array.
    template <class T, size_t size>
    constexpr size_t lengthof(T (&)[size]) noexcept
    {
        return size;
    }

    // Run-time invocation of unreachable results in undefined behavior.
    [[noreturn]] inline void unreachable() noexcept
    {
#ifndef NDEBUG
        std::abort();
#endif
#if STDEXT_COMPILER_GCC
        __builtin_unreachable();
#endif
    }
}

#endif
