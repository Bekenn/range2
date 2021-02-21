//
//  flags.h
//  stdext
//
//  Created by James Touton on 2/19/2021.
//  Copyright © 2021 James Touton. All rights reserved.
//

#ifndef STDEXT_FLAGS_INCLUDED
#define STDEXT_FLAGS_INCLUDED
#pragma once

#include <stdext/detail/config.h>

#include <type_traits>


namespace stdext
{
    template <typename T>
    class flags
    {
    private:
        using U = std::underlying_type_t<T>;

    public:
        constexpr flags() noexcept : value() { }
        constexpr flags(T value) noexcept : value(value) { }
        template <typename... TT>
        constexpr flags(T value, TT... values) noexcept : value(static_cast<T>(static_cast<U>(value) | static_cast<U>(flags(values...)))) { }
        constexpr flags& operator = (T value) noexcept { this->value = value; return *this; }

    public:
        friend constexpr bool operator == (flags a, flags b) noexcept { return a.value == b.value; }
        friend constexpr bool operator != (flags a, flags b) noexcept { return !(a == b); }
        friend constexpr bool operator == (flags a, T b) noexcept { return a.value == b; }
        friend constexpr bool operator != (flags a, T b) noexcept { return !(a == b); }
        friend constexpr bool operator == (T a, flags b) noexcept { return a == b.value; }
        friend constexpr bool operator != (T a, flags b) noexcept { return !(a == b); }

    public:
        constexpr operator T () const noexcept { return value; }
        explicit constexpr operator U () const noexcept { return static_cast<U>(value); }

        constexpr flags& clear() noexcept
        {
            value = static_cast<T>(0);
        }

        constexpr flags& set(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) | static_cast<U>(mask));
            return *this;
        }

        constexpr flags& reset(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) & ~static_cast<U>(mask));
            return *this;
        }

        constexpr flags& flip(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) ^ static_cast<U>(mask));
            return *this;
        }

        constexpr flags& keep(flags mask) noexcept
        {
            value = static_cast<T>(static_cast<U>(value) & static_cast<U>(mask));
            return *this;
        }

        constexpr bool test_any(flags mask) const noexcept
        {
            return (static_cast<U>(value) & static_cast<U>(mask)) != 0;
        }

        constexpr bool test_all(flags mask) const noexcept
        {
            return static_cast<T>(static_cast<U>(value) & static_cast<U>(mask)) == mask;
        }

        constexpr bool test(flags mask, flags value) const noexcept
        {
            return static_cast<T>(static_cast<U>(this->value) & static_cast<U>(mask)) == value;
        }

    private:
        T value;
    };

    template <typename T, typename... Ts>
    constexpr flags<T> make_flags(T value, Ts... values) noexcept
    {
        return { value, values... };
    }
}
#endif
