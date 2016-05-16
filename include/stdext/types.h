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

#include "detail/config.h"

#include <type_traits>

#include <cstddef>
#include <cstdint>


#if 0 // Use the flags template instead.
// Handy-dandy macro for adding flag operations for enums
#define FLAGS_ENUM(T)                                                   \
constexpr T operator ~ (T v) noexcept                                   \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return static_cast<T>(~static_cast<U>(v));                          \
}                                                                       \
constexpr T operator | (T a, T b) noexcept                              \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return static_cast<T>(static_cast<U>(a) | static_cast<U>(b));       \
}                                                                       \
constexpr T operator & (T a, T b) noexcept                              \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return static_cast<T>(static_cast<U>(a) & static_cast<U>(b));       \
}                                                                       \
constexpr T operator ^ (T a, T b) noexcept                              \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return static_cast<T>(static_cast<U>(a) ^ static_cast<U>(b));       \
}                                                                       \
constexpr T& operator |= (T& a, T b) noexcept                           \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return a = static_cast<T>(static_cast<U>(a) | static_cast<U>(b));   \
}                                                                       \
constexpr T& operator &= (T& a, T b) noexcept                           \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return a = static_cast<T>(static_cast<U>(a) & static_cast<U>(b));   \
}                                                                       \
constexpr T& operator ^= (T& a, T b) noexcept                           \
{                                                                       \
    using U = ::std::underlying_type_t<T>;                              \
    return a = static_cast<T>(static_cast<U>(a) ^ static_cast<U>(b));   \
}
#endif

namespace stdext
{
    // Types imported from cstddef
    using ::std::ptrdiff_t;
    using ::std::size_t;
    using ::std::max_align_t;
    using ::std::nullptr_t;

    // Types imported from cstdint
    using ::std::int8_t;
    using ::std::int16_t;
    using ::std::int32_t;
    using ::std::int64_t;
    using ::std::intmax_t;
    using ::std::intptr_t;

    using ::std::uint8_t;
    using ::std::uint16_t;
    using ::std::uint32_t;
    using ::std::uint64_t;
    using ::std::uintmax_t;
    using ::std::uintptr_t;

    template <class T>
    class flags
    {
    private:
        using U = ::std::underlying_type_t<T>;

    public:
        constexpr flags() noexcept : value() { }
        constexpr flags(T value) noexcept : value(value) { }
        template <typename... TT>
        constexpr flags(T value, TT... values) noexcept : value(static_cast<T>(static_cast<U>(value) | static_cast<U>(flags(values...)))) { }
        CONSTEXPR14 flags& operator = (T value) noexcept { this->value = value; return *this; }

    public:
        friend constexpr bool operator == (flags a, flags b) noexcept { return a.value == b.value; }
        friend constexpr bool operator != (flags a, flags b) noexcept { return !(a == b); }
        friend void swap(flags& a, flags& b) noexcept { using ::std::swap; swap(a.value, b.value); }

    public:
        constexpr operator T () const noexcept { return value; }
        explicit constexpr operator U () const noexcept { return static_cast<U>(value); }

        CONSTEXPR14 flags& clear() noexcept
        {
            value = static_cast<T>(0);
        }

        CONSTEXPR14 flags& set(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) | static_cast<U>(mask));
            return *this;
        }

        CONSTEXPR14 flags& reset(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) & ~static_cast<U>(mask));
            return *this;
        }

        CONSTEXPR14 flags& flip(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) ^ static_cast<U>(mask));
            return *this;
        }

        CONSTEXPR14 flags& keep(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) & static_cast<U>(mask));
            return *this;
        }

        constexpr bool test_any(flags mask) const noexcept
        {
            return static_cast<U>(value) & static_cast<U>(mask) != 0;
        }

        constexpr bool test_all(flags mask) const noexcept
        {
            return static_cast<T>(static_cast<U>(value) & static_cast<U>(mask)) == mask;
        }

    private:
        T value;
    };
}

#endif
