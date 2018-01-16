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
#include <stdext/meta.h>
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
    template <class Consumer, class... Args, REQUIRES(is_consumer_v<std::decay_t<Consumer>(char)>)>
    Consumer&& format(Consumer&& out, string_view fmt, Args&&... args);

    template <class Consumer, class Arg, REQUIRES(is_consumer_v<Consumer(char)> && std::is_integral_v<Arg>)>
    void format_arg(Consumer& out, string_view fmt, Arg arg);
    template <class Consumer, class Arg, REQUIRES(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, string_view>)>
    void format_arg(Consumer& out, string_view fmt, Arg&& arg);

    class format_error : std::logic_error
    {
        using logic_error::logic_error;
    };

    namespace detail
    {
        template <class Consumer, class Arg>
        void format_dispatch(Consumer& out, string_view fmt, Arg&& arg)
        {
            format_arg(out, fmt, std::forward<Arg>(arg));
        }

        template <size_t Index, class Consumer, class DispatchTuple, class ArgsTuple, class... Args>
        void format_dispatch(Consumer& out, string_view fmt, const DispatchTuple& dispatch, const ArgsTuple& args, type_list<Args...>)
        {
            std::get<Index>(dispatch)(out, fmt, std::forward<list_element_t<type_list<Args...>, Index>>(std::get<Index>(args)));
        }

        template <size_t... Indices, class Consumer, class DispatchTuple, class ArgsTuple, class... Args>
        void format_dispatch(size_t index, std::index_sequence<Indices...>, Consumer& out, string_view fmt, const DispatchTuple& dispatch, const ArgsTuple& args, type_list<Args...>)
        {
            using array_type = std::array<void (*)(Consumer&, string_view, const DispatchTuple&, const ArgsTuple&, type_list<Args...>), sizeof...(Indices)>;
            constexpr array_type dispatch_table = {{ &format_dispatch<Indices>... }};
            if (index >= sizeof...(Indices))
                throw format_error("Argument index out of range");
            (*dispatch_table[index])(out, fmt, dispatch, args, type_list<Args...>());
        }
    }

    template <class Consumer, class... Args, REQUIRED(is_consumer_v<std::decay_t<Consumer>(char)>)>
    Consumer&& format(Consumer&& out, string_view fmt, Args&&... args)
    {
        std::tuple<Args&...> arglist = { args... };
        auto dispatch = std::make_tuple(std::ref(static_cast<void(&)(Consumer& out, string_view fmt, Args&& arg)>(detail::format_dispatch))...);
        for (auto i = fmt.begin(); i != fmt.end(); )
        {
            if (*i != '$')
            {
                out(*i);
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
                detail::format_dispatch(n, std::make_index_sequence<sizeof...(Args)>(), out, f, dispatch, arglist, type_list<Args...>());
                ++i;
            }
            else if (isdigit(*i))
            {
                char* p;
                auto n = strtoul(&*i, &p, 10);
                detail::format_dispatch(n, std::make_index_sequence<sizeof...(Args)>(), out, "", dispatch, arglist, type_list<Args...>());
                i += p - &*i;
            }
            else
                throw format_error("Invalid format string");
        }

        return std::forward<Consumer>(out);
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
        FLAGS_ENUM(format_options)

        flags<format_options> parse_format_options(string_view& fmt);
        void format_integer(const std::function<bool (char ch)>& out, string_view fmt, uintmax_t uval, bool negative);
    }

    template <class Consumer, class Arg, REQUIRED(is_consumer_v<Consumer(char)> && std::is_integral_v<Arg>)>
    void format_arg(Consumer& out, string_view fmt, Arg arg)
    {
        return detail::format_integer(out, fmt, arg < 0 ? -arg : arg, arg < 0);
    }

    template <class Consumer, class Arg, REQUIRED(is_consumer_v<Consumer(char)> && std::is_convertible_v<std::decay_t<Arg>, string_view>)>
    void format_arg(Consumer& out, string_view fmt, Arg&& arg)
    {
        auto sv = string_view(std::forward<Arg>(arg));
        if (fmt.length() != 0)
            throw format_error("Unrecognized format for string_view");
        if constexpr (is_consumer_v<std::decay_t<Consumer>(string_view)>)
            out(sv);
        else
            sv >> out;
    }
}

#endif
