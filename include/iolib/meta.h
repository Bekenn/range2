//
//  meta.h
//  iolib
//
//  Created by James Touton on 12/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_META_INCLUDED
#define IOLIB_META_INCLUDED
#pragma once

#include <cstddef>

namespace iolib
{
    // A constant value.
    template <class T, T v>
    struct constant
    {
        using type = T;
        static constexpr T value = v;
    };

    // A list of types.
    template <class...> struct type_list { };

    // Retrieve the first element from a list.
    template <class List> struct list_head;
    template <class List> using list_head_t = typename list_head<List>::type;
    template <class T, class... Ts>
    struct list_head<type_list<T, Ts...>>
    {
        using type = T;
    };

    // Retrieve the list of all elements except the first from a list.
    template <class List> struct list_tail;
    template <class List> using list_tail_t = typename list_head<List>::type;
    template <class T, class... Ts>
    struct list_tail<type_list<T, Ts...>>
    {
        using type = type_list<Ts...>;
    };

    // Add a new element to the front of a list.
    template <class List, class T> struct list_prepend;
    template <class List, class T> using list_prepend_t = typename list_prepend<List, T>::type;
    template <class... Ts, class T>
    struct list_prepend<type_list<Ts...>, T>
    {
        using type = type_list<T, Ts...>;
    };

    // Add a new element to the back of alist.
    template <class T, class List> struct list_append;
    template <class List, class T> using list_append_t = typename list_append<List, T>::type;
    template <class... Ts, class T>
    struct list_append<type_list<Ts...>, T>
    {
        using type = type_list<Ts..., T>;
    };

    // Construct a list containing all of the elements of several other lists.
    template <class... Lists> struct list_concat;
    template <class... Lists> using list_concat_t = typename list_concat<Lists...>::type;
    template <class... Ts>
    struct list_concat<type_list<Ts...>>
    {
        using type = type_list<Ts...>;
    };
    template <class... Ts, class... Us, class... Lists>
    struct list_concat<type_list<Ts...>, type_list<Us...>, Lists...>
    {
        using type = typename list_concat<type_list<Ts..., Us...>, Lists...>::type;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by prepending a value onto its corresponding list.
    template <class Lists, class List> struct list_prepend_lists;
    template <class Lists, class List> using list_prepend_lists_t = typename list_prepend_lists<Lists, List>::type;
    template <class... Lists, class... Ts>
    struct list_prepend_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_prepend_t<Lists, Ts>...>;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by appending a value onto its corresponding list.
    template <class Lists, class List> struct list_append_lists;
    template <class Lists, class List> using list_append_lists_t = typename list_append_lists<Lists, List>::type;
    template <class... Lists, class... Ts>
    struct list_append_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_append_t<Lists, Ts>...>;
    };

    // Given several lists of lists, construct a list of lists, each constructed by concatenating corresponding lists.
    template <class... Listss> struct list_concat_lists;
    template <class... Listss> using list_concat_lists_t = typename list_concat_lists::type;
    template <class... Lists>
    struct list_concat_lists<type_list<Lists...>>
    {
        using type = type_list<Lists...>;
    };
    template <class... Lists1, class... Lists2, class... Listss>
    struct list_concat_lists<type_list<Lists1...>, type_list<Lists2...>, Listss...>
    {
        using type = list_concat_lists_t<type_list<list_concat_t<Lists1, Lists2>...>, Listss...>;
    };

    // Given a list of lists, construct a list of lists, each constructed with corresponding elements from all lists.
    template <class Lists> struct list_transpose;
    template <class Lists> using list_transpose_t = typename list_transpose<Lists>::type;
    template <>
    struct list_transpose<type_list<>>
    {
        using type = type_list<>;
    };
    template <class... Ts, class... Lists>
    struct list_transpose<type_list<type_list<Ts...>, Lists...>
    {
        using type = list_concat_lists_t<type_list<type_list<Ts>...>, list_transpose_t<type_list<Lists...>>>;
    };

    // Construct a list of monotonically increasing constants with a given length and starting value.
    template <::std::size_t length, class T = int, T start = 0> struct iota_list;
    template <::std::size_t length, class T = int, T start = 0> using iota_list_t = typename iota_list<length, T, start>::type;
    template <class T, T start>
    struct iota_list<0, T, start>
    {
        using type = type_list<>;
    };
    template <::std::size_t length, class T, T start>
    struct iota_list
    {
        using type = list_prepend_t<constant<T, start>, iota_list_t<length - 1, T, start + 1>>;
    };
}

#endif
