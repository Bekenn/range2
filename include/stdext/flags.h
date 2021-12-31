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

#include <stdext/traits.h>


namespace stdext
{
    template <typename E>
    class flags
    {
        static_assert(std::is_enum_v<E>);

    public:
        using enum_type = E;
        using underlying_type = std::underlying_type_t<enum_type>;

    public:
        constexpr flags() noexcept : _value() { }
        constexpr flags(enum_type value) noexcept : _value(value) { }
        constexpr flags& operator = (enum_type value) noexcept { _value = value; return *this; }
        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr flags(Ts... values) noexcept : _value(enum_type((underlying_type() | ... | underlying_type(enum_type(values))))) { }

        explicit constexpr flags(underlying_type value) noexcept : _value(enum_type(value)) { }

    public:
        friend constexpr bool operator == (flags a, flags b) noexcept { return a._value == b._value; }
        friend constexpr bool operator != (flags a, flags b) noexcept { return a._value != b._value; }
        friend constexpr bool operator == (flags a, enum_type b) noexcept { return a._value == b; }
        friend constexpr bool operator != (flags a, enum_type b) noexcept { return a._value != b; }
        friend constexpr bool operator == (enum_type a, flags b) noexcept { return a == b._value; }
        friend constexpr bool operator != (enum_type a, flags b) noexcept { return a != b._value; }

    public:
        constexpr operator enum_type () const noexcept { return _value; }
        explicit constexpr operator underlying_type () const noexcept { return underlying_type(_value); }

        constexpr enum_type value() const noexcept
        {
            return _value;
        }

        constexpr flags& clear() noexcept
        {
            _value = { };
            return *this;
        }

        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr flags& set(Ts... values) noexcept
        {
            underlying_type all_values = (underlying_type() | ... | underlying_type(enum_type(values)));
            _value = enum_type(underlying_type(_value) | all_values);
            return *this;
        }

        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr flags& reset(Ts... values) noexcept
        {
            underlying_type all_values = (underlying_type() | ... | underlying_type(enum_type(values)));
            _value = enum_type(underlying_type(_value) & ~all_values);
            return *this;
        }

        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr flags& toggle(Ts... values) noexcept
        {
            underlying_type all_values = (underlying_type() | ... | underlying_type(enum_type(values)));
            _value = enum_type(underlying_type(_value) ^ all_values);
            return *this;
        }

        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr flags& keep(Ts... values) noexcept
        {
            underlying_type all_values = (underlying_type() | ... | underlying_type(enum_type(values)));
            _value = enum_type(underlying_type(_value) & all_values);
            return *this;
        }

        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr bool test_any(Ts... values) const noexcept
        {
            underlying_type all_values = (underlying_type() | ... | underlying_type(enum_type(values)));
            return (underlying_type(_value) & all_values) != underlying_type();
        }

        template <typename... Ts, STDEXT_REQUIRES((... && std::is_convertible_v<Ts, enum_type>))>
        constexpr bool test_all(Ts... values) const noexcept
        {
            underlying_type all_values = (underlying_type() | ... | underlying_type(enum_type(values)));
            return (underlying_type(_value) & all_values) == all_values;
        }

        constexpr bool test(flags mask, flags values) const noexcept
        {
            return (underlying_type(_value) & underlying_type(mask)) == (underlying_type(values) & underlying_type(mask));
        }

    private:
        enum_type _value;
    };
}

#endif
