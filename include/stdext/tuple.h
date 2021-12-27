//
//  tuple.h
//  stdext
//
//  Created by James Touton on 12/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_TUPLE_INCLUDED
#define STDEXT_TUPLE_INCLUDED
#pragma once

#include <stdext/meta.h>

#include <tuple>

#include <cstddef>


namespace stdext
{
    template <class Tuple> struct tuple_element_list;
    template <class Tuple> using tuple_element_list_t = typename tuple_element_list<Tuple>::type;
    template <class... Ts>
    struct tuple_element_list<std::tuple<Ts...>>
    {
        using type = type_list<Ts...>;
    };

    namespace _private
    {
        template <class... Tuples> struct tuples_size;
        template <class Tuple>
        struct tuples_size<Tuple>
        {
            static constexpr size_t value = std::tuple_size<Tuple>::value;
        };
        template <class Tuple, class... Tuples>
        struct tuples_size<Tuple, Tuples...>
        {
            static constexpr std::enable_if_t<std::tuple_size<Tuple>::value == tuples_size<Tuples...>::value, size_t>
                value = std::tuple_size<Tuple>::value;
        };

        template <class T>
        std::conditional_t<std::is_reference<T>::value, std::reference_wrapper<std::remove_reference_t<T>>, T&>
            wrap(std::remove_reference_t<T>& arg)
        {
            return arg;
        }

        template <size_t n, class... Tuples, size_t... tuple_indices>
        auto zip_element(const std::tuple<Tuples&...>& tuples, value_list<tuple_indices...>)
        {
            using element_type_list = type_list<std::tuple_element_t<n, Tuples>...>;
            return std::make_tuple(wrap<list_element_t<element_type_list, tuple_indices>>(std::get<n>(std::get<tuple_indices>(tuples)))...);
        }

        template <class Tuples, size_t... element_indices>
        auto zip(const Tuples& tuples, value_list<element_indices...>)
        {
            return std::make_tuple(zip_element<element_indices>(tuples, iota_list_t<std::tuple_size<Tuples>::value, size_t>())...);
        }
    }

    template <class... Tuples>
    auto zip(const Tuples&... tuples)
    {
        return _private::zip(std::tuple<Tuples&...>(tuples...), iota_list<_private::tuples_size<Tuples...>::value, size_t>());
    }
}

#endif
