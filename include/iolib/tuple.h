//
//  tuple.h
//  iolib
//
//  Created by James Touton on 12/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_TUPLE_INCLUDED
#define IOLIB_TUPLE_INCLUDED
#pragma once

#include "meta.h"

#include <tuple>

#include <cstddef>


namespace iolib
{
    namespace detail
    {
        template <class Function, class... Ts, ::std::size_t... indices>
        decltype(auto) apply(Function&& func, const ::std::tuple<Ts...>& args, type_list<constant<size_t, indices>...>)
        {
            return func(::std::forward<Ts>(::std::get<indices>(args))...);
        }
    }

    template <class Function, class... Ts>
    decltype(auto) apply(Function&& func, const ::std::tuple<Ts...>& args)
    {
        return detail::apply(::std::forward<Function>(func), args, iota_list<sizeof...(Ts), ::std::size_t>());
    }

    namespace detail
    {
        template <class... Tuples, ::std::size_t... inner_indices, ::std::size_t... outer_indices>
        auto zip(const ::std::tuple<Tuples&...>& tuples, type_list<constant<::std::size_t, inner_indices>...>, type_list<constant<::std::size_t, outer_indices>)
        {
            return ::std::make_tuple(::std::make_tuple(::std::get<inner_indices>(::std::get<outer_indices>(tuples)...))...);
        }
    }

    template <class... Tuples>
    auto zip(const Tuples&... tuples)
    {
    }
}

#endif
