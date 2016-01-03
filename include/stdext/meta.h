//
//  meta.h
//  stdext
//
//  Created by James Touton on 12/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_META_INCLUDED
#define STDEXT_META_INCLUDED
#pragma once

#include <cstddef>

namespace stdext
{
    // Compile-time variadic boolean comparisons.
    template <bool... vs> struct const_and;
    template <>
    struct const_and<>
    {
        static constexpr bool value = true;
    };
    template <bool v, bool... vs>
    struct const_and<v, vs...>
    {
        static constexpr bool value = v && const_and<vs...>::value;
    };

    template <bool... vs> struct const_or;
    template <>
    struct const_or<>
    {
        static constexpr bool value = false;
    };
    template <bool v, bool... vs>
    struct const_or<v, vs...>
    {
        static constexpr bool value = v || const_or<vs...>::value;
    };

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

    // Retrieve the nth element from a list.
    template <class List, ::std::size_t n> struct list_element;
    template <class List, ::std::size_t n> using list_element_t = typename list_element<List, n>::type;
    template <class T, class... Ts>
    struct list_element<type_list<T, Ts...>, 0>
    {
        using type = T;
    };
    template <class T, class... Ts, ::std::size_t n>
    struct list_element<type_list<T, Ts...>, n>
    {
        using type = list_element_t<type_list<Ts...>, n - 1>;
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
    template <class... Listss> using list_concat_lists_t = typename list_concat_lists<Listss...>::type;
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
    struct list_transpose<type_list<type_list<Ts...>, Lists...>>
    {
        using type = list_concat_lists_t<type_list<type_list<Ts>...>, list_transpose_t<type_list<Lists...>>>;
    };

    // Given several lists, construct a list of lists, each constructed with corresponding elements from all lists.
    template <class... Lists> struct list_zip;
    template <class... Lists> using list_zip_t = typename list_zip<Lists...>::type;
    template <class... Lists>
    struct list_zip
    {
        using type = list_transpose_t<type_list<Lists...>>;
    };

    // Given a template taking variadic type arguments, instantiate that template with types from a list.
    template <template <class...> class Templ, class List> struct list_apply;
    template <template <class...> class Templ, class List> using list_apply_t = typename list_apply<Templ, List>::type;
    template <template <class...> class Templ, class... Ts>
    struct list_apply<Templ, type_list<Ts...>>
    {
        using type = Templ<Ts...>;
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
