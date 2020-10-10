//
//  generator.h
//  stdext
//
//  Created by James Touton on 10/31/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_FORMAT_INCLUDED
#define STDEXT_FORMAT_INCLUDED
#pragma once

#include <stdext/consumer.h>
#include <stdext/generator.h>
#include <stdext/meta.h>
#include <stdext/string.h>
#include <stdext/string_view.h>

#include <algorithm>
#include <array>
#include <functional>
#include <stdexcept>
#include <utility>

#include <cctype>
#include <cstdlib>


namespace stdext
{
    template <class Consumer, class... Args, STDEXT_REQUIRES(is_consumer_v<std::decay_t<Consumer>(char)>)>
    bool format(Consumer&& out, string_view fmt, Args&&... args);

    template <class... Args>
    std::string format_string(string_view fmt, Args&&... args);

    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_integral_v<Arg>)>
    bool format_arg(Consumer& out, string_view fmt, Arg arg);

    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg);
#if STDEXT_HAS_C_UNICODE
    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, u16string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg);
    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, u32string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg);
#endif
    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, wstring_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg);

    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_invocable_r_v<bool, Arg, Consumer&>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg);
    template <class Consumer, class Arg, STDEXT_REQUIRES(is_consumer_v<Consumer(char)> && std::is_invocable_r_v<bool, Arg, Consumer&, string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg);

    class format_error : public std::logic_error
    {
        using logic_error::logic_error;
    };

    namespace detail
    {
        template <class Consumer, class Arg>
        bool format_dispatch(Consumer& out, string_view fmt, Arg&& arg)
        {
            return format_arg(out, fmt, forward<Arg>(arg));
        }

        template <size_t Index, class Consumer, class DispatchTuple, class ArgsTuple, class... Args>
        bool format_dispatch(Consumer& out, string_view fmt, const DispatchTuple& dispatch, const ArgsTuple& args, type_list<Args...>)
        {
            return std::get<Index>(dispatch)(out, fmt, forward<list_element_t<type_list<Args...>, Index>>(std::get<Index>(args)));
        }

        template <size_t... Indices, class Consumer, class DispatchTuple, class ArgsTuple, class... Args>
        bool format_dispatch(size_t index, std::index_sequence<Indices...>, Consumer& out, string_view fmt, const DispatchTuple& dispatch, const ArgsTuple& args, type_list<Args...>)
        {
            using array_type = std::array<bool (*)(Consumer&, string_view, const DispatchTuple&, const ArgsTuple&, type_list<Args...>), sizeof...(Indices)>;
            constexpr array_type dispatch_table = {{ &format_dispatch<Indices>... }};
            if (index >= sizeof...(Indices))
                throw format_error("Argument index out of range");
            return (*dispatch_table[index])(out, fmt, dispatch, args, type_list<Args...>());
        }
    }

    template <class Consumer, class... Args, STDEXT_REQUIRED(is_consumer_v<std::decay_t<Consumer>(char)>)>
    bool format(Consumer&& out, string_view fmt, Args&&... args)
    {
        std::tuple<Args&...> arglist = { args... };
        auto dispatch = std::make_tuple(std::ref(static_cast<bool (&)(Consumer& out, string_view fmt, Args&& arg)>(detail::format_dispatch))...);
        for (auto i = fmt.begin(); i != fmt.end(); )
        {
            if (*i != '$')
            {
                if (!out(*i))
                    return false;
                ++i;
                continue;
            }

            ++i;
            if (*i == '{')
            {
                ++i;
                if (!isdigit(*i))
                    throw format_error("Invalid format string");
                char* p;
                auto n = strtoul(&*i, &p, 10);
                i += p - &*i;
                string_view f;
                if (*i == ':')
                {
                    ++i;
                    auto e = std::find(i, fmt.end(), '}');
                    if (e == fmt.end())
                        throw format_error("Invalid format string");
                    f = fmt.substr(i - fmt.begin(), e - i);
                    i = e;
                }
                if (*i != '}')
                    throw format_error("Invalid format string");
                if (!detail::format_dispatch(n, std::make_index_sequence<sizeof...(Args)>(), out, f, dispatch, arglist, type_list<Args...>()))
                    return false;
                ++i;
            }
            else if (isdigit(*i))
            {
                char* p;
                auto n = strtoul(&*i, &p, 10);
                if (!detail::format_dispatch(n, std::make_index_sequence<sizeof...(Args)>(), out, "", dispatch, arglist, type_list<Args...>()))
                    return false;
                i += p - &*i;
            }
            else if (*i == '$')
            {
                if (!out(*i))
                    return false;
                ++i;
            }
            else
                throw format_error("Invalid format string");
        }

        return true;
    }

    template <class... Args>
    std::string format_string(string_view fmt, Args&&... args)
    {
        stringbuf buf;
        bool result = format(buf, fmt, forward<Args>(args)...);
        assert(result);
        discard(result);
        return buf.extract();
    }

    namespace detail
    {
        enum class format_options
        {
            none                = 0x00,
            left_justified      = 0x01,
            show_sign           = 0x02,
            pad_sign            = 0x04,
            alternative_form    = 0x08,
            zero_pad            = 0x10,
        };

        enum class format_type
        {
            _char,
#if STDEXT_HAS_C_UNICODE
            _char16,
            _char32,
#endif
            _wchar,
            _signed,
            _unsigned
        };

        flags<format_options> parse_format_options(string_view& fmt);
        bool format_integer(const std::function<bool (char ch)>& out, string_view fmt, uintmax_t uval, format_type type);
    }

    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_integral_v<Arg>)>
    bool format_arg(Consumer& out, string_view fmt, Arg arg)
    {
        auto type = std::is_same_v<Arg, char> ? detail::format_type::_char
#if STDEXT_HAS_C_UNICODE
            : std::is_same_v<Arg, char16_t> ? detail::format_type::_char16
            : std::is_same_v<Arg, char32_t> ? detail::format_type::_char32
#endif
            : std::is_same_v<Arg, wchar_t> ? detail::format_type::_wchar
            : std::is_signed_v<Arg> ? detail::format_type::_signed
            : detail::format_type::_unsigned;
        return detail::format_integer(std::ref(out), fmt, arg, type);
    }

    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        if (!fmt.empty())
            throw format_error("Unrecognized format for string_view");

        auto sv = string_view(forward<Arg>(arg));
        if constexpr (is_consumer_v<std::decay_t<Consumer>(string_view)>)
            return out(sv);
        else
            return sv >> out;
    }

#if STDEXT_HAS_C_UNICODE
    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, u16string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        if (!fmt.empty())
            throw format_error("Unrecognized format for u16string_view");

        auto sv = u16string_view(forward<Arg>(arg));
        auto generator = make_generator(sv);
        return generator >> to_multibyte() >> out;
    }

    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, u32string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        if (!fmt.empty())
            throw format_error("Unrecognized format for u32string_view");

        auto sv = u32string_view(forward<Arg>(arg));
        auto generator = make_generator(sv);
        return generator >> to_multibyte() >> out;
    }
#endif

    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, wstring_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        if (!fmt.empty())
            throw format_error("Unrecognized format for wstring_view");

        auto sv = wstring_view(forward<Arg>(arg));
        auto generator = make_generator(sv);
        return generator >> to_multibyte() >> out;
    }

    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_invocable_r_v<bool, Arg, Consumer&>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        if (!fmt.empty())
            throw format_error("Unrecognized format for invocable");
        return std::invoke(forward<Arg>(arg), out);
    }

    template <class Consumer, class Arg, STDEXT_REQUIRED(is_consumer_v<Consumer(char)> && std::is_invocable_r_v<bool, Arg, Consumer&, string_view>)>
    bool format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        return std::invoke(forward<Arg>(arg), out, fmt);
    }
}

#endif
