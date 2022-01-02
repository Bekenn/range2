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
    template <typename T, typename IndexList = stdext::index_list_t<std::tuple_size_v<T>>> struct tuple_element_list;
    template <typename T> using tuple_element_list_t = typename tuple_element_list<T>::type;
    template <typename T, size_t... Indices>
    struct tuple_element_list<T, stdext::value_list<Indices...>>
    {
        using type = stdext::type_list<std::tuple_element_t<Indices, T>...>;
    };

    namespace _private
    {
        template <typename... Tuples> struct tuples_size;
        template <typename... Tuples> constexpr size_t tuples_size_v = tuples_size<Tuples...>::value;
        template <typename Tuple>
        struct tuples_size<Tuple>
        {
            static constexpr size_t value = std::tuple_size<Tuple>::value;
        };
        template <typename Tuple, typename... Tuples>
        struct tuples_size<Tuple, Tuples...>
        {
            static constexpr std::enable_if_t<std::tuple_size_v<Tuple> == tuples_size_v<Tuples...>, size_t>
                value = std::tuple_size_v<Tuple>;
        };

        template <typename T>
        constexpr std::conditional_t<std::is_reference_v<T>, std::reference_wrapper<std::remove_reference_t<T>>, T&>
            wrap(std::remove_reference_t<T>& arg) noexcept
        {
            return arg;
        }

        template <size_t N, typename... Tuples, size_t... tuple_indices>
        constexpr auto zip_element(const std::tuple<Tuples&...>& tuples, value_list<tuple_indices...>)
        {
            using element_type_list = type_list<std::tuple_element_t<N, Tuples>...>;
            return std::make_tuple(wrap<list_element_t<element_type_list, tuple_indices>>(std::get<N>(std::get<tuple_indices>(tuples)))...);
        }

        template <typename Tuples, size_t... ElementIndices>
        constexpr auto zip(const Tuples& tuples, value_list<ElementIndices...>)
        {
            return std::make_tuple(zip_element<ElementIndices>(tuples, index_list_t<std::tuple_size_v<Tuples>>())...);
        }
    }

    template <typename... Tuples>
    constexpr auto zip(const Tuples&... tuples)
    {
        return _private::zip(std::tuple<const Tuples&...>(tuples...), index_list_t<_private::tuples_size<Tuples...>::value>());
    }
}

#endif
