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

#include <stdext/types.h>

#include <cstddef>


namespace stdext
{
    // Compile-time variadic boolean comparisons.
    template <bool... vs> struct const_and;
    template <bool... vs> constexpr auto const_and_v = const_and<vs...>::value;
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
    template <bool... vs> constexpr auto const_or_v = const_or<vs...>::value;
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
    template <auto v>
    struct constant
    {
        using type = decltype(v);
        static constexpr auto value = v;
    };

    // Identity metafunction.
    template <class T> struct identity_type;
    template <class T> using identity_type_t = typename identity_type<T>::type;
    template <class T>
    struct identity_type
    {
        using type = T;
    };

    // A list of types.
    template <class...> struct type_list { };

    // A list of values.
    template <auto...> struct value_list { };

    // Retrieve the first element from a list.
    template <class List> struct list_head;
    template <class List> using list_head_t = typename list_head<List>::type;
    template <class List> constexpr auto list_head_v = list_head<List>::value;
    template <class T, class... Ts>
    struct list_head<type_list<T, Ts...>>
    {
        using type = T;
    };
    template <auto v, auto... vs>
    struct list_head<value_list<v, vs...>>
    {
        static constexpr auto value = v;
    };

    // Retrieve the list of all elements except the first from a list.
    template <class List> struct list_tail;
    template <class List> using list_tail_t = typename list_head<List>::type;
    template <class T, class... Ts>
    struct list_tail<type_list<T, Ts...>>
    {
        using type = type_list<Ts...>;
    };
    template <auto v, auto... vs>
    struct list_tail<value_list<v, vs...>>
    {
        using type = value_list<vs...>;
    };

    // Retrieve the length of a list.
    template <class List> struct list_length;
    template <class List> constexpr auto list_length_v = list_length<List>::value;
    template <class... Ts>
    struct list_length<type_list<Ts...>>
    {
        static constexpr size_t value = sizeof...(Ts);
    };
    template <auto... vs>
    struct list_length<value_list<vs...>>
    {
        static constexpr size_t value = sizeof...(vs);
    };

    // Retrieve the nth element from a list.
    template <class List, size_t n> struct list_element;
    template <class List, size_t n> using list_element_t = typename list_element<List, n>::type;
    template <class List, size_t n> constexpr auto list_element_v = list_element<List, n>::value;
    template <class T, class... Ts, size_t n>
    struct list_element<type_list<T, Ts...>, n>
    {
        using type = list_element_t<type_list<Ts...>, n - 1>;
    };
    template <class T, class... Ts>
    struct list_element<type_list<T, Ts...>, 0>
    {
        using type = T;
    };
    template <auto v, auto... vs, size_t n>
    struct list_element<value_list<v, vs...>, n>
    {
        static constexpr auto value = list_element_v<value_list<vs...>, n - 1>;
    };
    template <auto v, auto... vs>
    struct list_element<value_list<v, vs...>, 0>
    {
        static constexpr auto value = v;
    };

    // Find the index of an element in the list; returns list_length<List> if the element can't be found.
    template <class List, class T> struct list_index_of;
    template <class List, class T> constexpr auto list_index_of_v = list_index_of<List, T>::value;
    template <class T0, class... Ts, class T>
    struct list_index_of<type_list<T0, Ts...>, T>
    {
        static constexpr auto value = list_index_of_v<type_list<Ts...>, T> + 1;
    };
    template <class T, class... Ts>
    struct list_index_of<type_list<T, Ts...>, T>
    {
        static constexpr size_t value = 0;
    };
    template <class... Ts, class T>
    struct list_index_of<type_list<Ts...>, T>
    {
        static constexpr auto value = sizeof...(Ts);
    };
    template <auto v0, auto... vs, auto v>
    struct list_index_of<value_list<v0, vs...>, constant<v>>
    {
        static constexpr auto value = list_index_of_v<value_list<vs...>, constant<v>> + 1;
    };
    template <auto v, auto... vs>
    struct list_index_of<value_list<v, vs...>, constant<v>>
    {
        static constexpr size_t value = 0;
    };
    template <auto... vs, auto v>
    struct list_index_of<value_list<vs...>, constant<v>>
    {
        static constexpr auto value = sizeof...(vs);
    };

    // Add a new element to the front of a list.
    template <class List, class T> struct list_prepend;
    template <class List, class T> using list_prepend_t = typename list_prepend<List, T>::type;
    template <class... Ts, class T>
    struct list_prepend<type_list<Ts...>, T>
    {
        using type = type_list<T, Ts...>;
    };
    template <auto... vs, auto v>
    struct list_prepend<value_list<vs...>, constant<v>>
    {
        using type = value_list<v, vs...>;
    };

    // Add a new element to the back of alist.
    template <class T, class List> struct list_append;
    template <class List, class T> using list_append_t = typename list_append<List, T>::type;
    template <class... Ts, class T>
    struct list_append<type_list<Ts...>, T>
    {
        using type = type_list<Ts..., T>;
    };
    template <auto... vs, auto v>
    struct list_append<value_list<vs...>, constant<v>>
    {
        using type = value_list<vs..., v>;
    };

    // Construct a list containing all of the elements of several other lists.
    template <class... Lists> struct list_concat;
    template <class... Lists> using list_concat_t = typename list_concat<Lists...>::type;
    template <class... Ts, class... Us, class... Lists>
    struct list_concat<type_list<Ts...>, type_list<Us...>, Lists...>
    {
        using type = typename list_concat<type_list<Ts..., Us...>, Lists...>::type;
    };
    template <class... Ts>
    struct list_concat<type_list<Ts...>>
    {
        using type = type_list<Ts...>;
    };
    template <auto... vs, auto... ws, class... Lists>
    struct list_concat<value_list<vs...>, value_list<ws...>, Lists...>
    {
        using type = typename list_concat<value_list<vs..., ws...>, Lists...>::type;
    };
    template <auto... vs>
    struct list_concat<value_list<vs...>>
    {
        using type = value_list<vs...>;
    };

    // Retrieve the list of the first n elements in a list.
    template <class List, size_t n> struct list_take;
    template <class List, size_t n> using list_take_t = typename list_take<List, n>::type;
    template <class T0, class... Ts, size_t n>
    struct list_take<type_list<T0, Ts...>, n>
    {
        using type = list_prepend_t<list_take_t<type_list<Ts...>, n - 1>, T0>;
    };
    template <class... Ts>
    struct list_take<type_list<Ts...>, 0>
    {
        using type = type_list<>;
    };
    template <auto v0, auto... vs, size_t n>
    struct list_take<value_list<v0, vs...>, n>
    {
        using type = list_prepend_t<list_take_t<value_list<vs...>, n - 1>, constant<v0>>;
    };
    template <auto... vs>
    struct list_take<value_list<vs...>, 0>
    {
        using type = value_list<>;
    };

    // Retrieve the list of all elements after the first n elements in a list.
    template <class List, size_t n> struct list_drop;
    template <class List, size_t n> using list_drop_t = typename list_drop<List, n>::type;
    template <class T0, class... Ts, size_t n>
    struct list_drop<type_list<T0, Ts...>, n>
    {
        using type = list_drop_t<type_list<Ts...>, n - 1>;
    };
    template <class... Ts>
    struct list_drop<type_list<Ts...>, 0>
    {
        using type = type_list<Ts...>;
    };
    template <auto v0, auto... vs, size_t n>
    struct list_drop<value_list<v0, vs...>, n>
    {
        using type = list_drop_t<value_list<vs...>, n - 1>;
    };
    template <auto... vs>
    struct list_drop<value_list<vs...>, 0>
    {
        using type = value_list<vs...>;
    };

    // Insert the given element into a list at the given location.
    template <class List, size_t n, class T> struct list_insert_element;
    template <class List, size_t n, class T> using list_insert_element_t = typename list_insert_element<List, n, T>::type;
    template <class... Ts, size_t n, class T>
    struct list_insert_element<type_list<Ts...>, n, T>
    {
        using type = list_concat_t<list_take_t<type_list<Ts...>, n>, type_list<T>, list_drop_t<type_list<Ts...>, n>>;
    };
    template <auto... vs, size_t n, auto v>
    struct list_insert_element<value_list<vs...>, n, constant<v>>
    {
        using type = list_concat_t<list_take_t<value_list<vs...>, n>, value_list<v>, list_drop_t<value_list<vs...>, n>>;
    };

    // Insert the given elements into a list at the given locations.
    template <size_t n, class T> struct list_insert_elements_arg { };
    template <class List, class... Args> struct list_insert_elements;
    template <class List, class... Args> using list_insert_elements_t = typename list_insert_elements<List, Args...>::type;
    template <class List, size_t n0, class T0, size_t... ns, class... Ts>
    struct list_insert_elements<List, list_insert_elements_arg<n0, T0>, list_insert_elements_arg<ns, Ts>...>
    {
        using type = list_insert_elements_t<list_insert_element_t<List, n0, T0>, list_insert_elements_arg<ns < n0 ? ns : ns + 1, Ts>...>;
    };
    template <class List>
    struct list_insert_elements<List>
    {
        using type = List;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by prepending a value onto its corresponding list.
    template <class Lists, class List> struct list_prepend_lists;
    template <class Lists, class List> using list_prepend_lists_t = typename list_prepend_lists<Lists, List>::type;
    template <class... Lists, class... Ts>
    struct list_prepend_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_prepend_t<Lists, Ts>...>;
    };
    template <class... Lists, auto... vs>
    struct list_prepend_lists<type_list<Lists...>, value_list<vs...>>
    {
        using type = type_list<list_prepend_t<Lists, constant<vs>>...>;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by appending a value onto its corresponding list.
    template <class Lists, class List> struct list_append_lists;
    template <class Lists, class List> using list_append_lists_t = typename list_append_lists<Lists, List>::type;
    template <class... Lists, class... Ts>
    struct list_append_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_append_t<Lists, Ts>...>;
    };
    template <class... Lists, auto... vs>
    struct list_append_lists<type_list<Lists...>, value_list<vs...>>
    {
        using type = type_list<list_append_t<Lists, constant<vs>>...>;
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

    // Given a template taking variadic type arguments and a list of types, conditionally invoke list_apply.
    template <bool condition, template <class...> class TrueTempl, class TrueList, template <class...> class FalseTempl, class FalseList> struct list_conditional_apply;
    template <bool condition, template <class...> class TrueTempl, class TrueList, template <class...> class FalseTempl, class FalseList> using list_conditional_apply_t = typename list_conditional_apply<condition, TrueTempl, TrueList, FalseTempl, FalseList>::type;
    template <bool condition, template <class...> class TrueTempl, class TrueList, template <class...> class FalseTempl, class FalseList>
    struct list_conditional_apply
    {
        using type = list_apply_t<FalseTempl, FalseList>;
    };
    template <template <class...> class TrueTempl, class TrueList, template <class...> class FalseTempl, class FalseList>
    struct list_conditional_apply<true, TrueTempl, TrueList, FalseTempl, FalseList>
    {
        using type = list_apply_t<TrueTempl, TrueList>;
    };

    // Given a list of types and a boolean type trait, returns whether the trait is true for all types.
    template <class List, template <class> class Trait> struct list_all_of;
    template <class List, template <class> class Trait> constexpr auto list_all_of_v = list_all_of<List, Trait>::value;
    template <class T0, class... Ts, template <class> class Trait>
    struct list_all_of<type_list<T0, Ts...>, Trait>
    {
        static constexpr auto value = Trait<T0>::value && list_all_of<type_list<Ts...>, Trait>::value;
    };
    template <template <class> class Trait>
    struct list_all_of<type_list<>, Trait>
    {
        static constexpr auto value = true;
    };

    // Given a list of types and a boolean type trait, returns whether the trait is true for any type.
    template <class List, template <class> class Trait> struct list_any_of;
    template <class List, template <class> class Trait> constexpr auto list_any_of_v = list_any_of<List, Trait>::value;
    template <class T0, class... Ts, template <class> class Trait>
    struct list_any_of<type_list<T0, Ts...>, Trait>
    {
        static constexpr auto value = Trait<T0>::value || list_any_of<type_list<Ts...>, Trait>::value;
    };
    template <template <class> class Trait>
    struct list_any_of<type_list<>, Trait>
    {
        static constexpr auto value = false;
    };

    // Given a list of types and a boolean type trait, returns whether the trait is true for no type.
    template <class List, template <class> class Trait> struct list_none_of;
    template <class List, template <class> class Trait> constexpr auto list_none_of_v = list_none_of<List, Trait>::value;
    template <class List, template <class> class Trait>
    struct list_none_of
    {
        static constexpr auto value = !list_any_of<List, Trait>::value;
    };

    // Given a list of types and a boolean type trait, returns the number of types for which the trait is true.
    template <class List, template <class> class Trait> struct list_count_fulfilling;
    template <class List, template <class> class Trait> constexpr auto list_count_fulfilling_v = list_count_fulfilling<List, Trait>::value;
    template <class T0, class... Ts, template <class> class Trait>
    struct list_count_fulfilling<type_list<T0, Ts...>, Trait>
    {
        static constexpr auto value = list_count_fulfilling<type_list<Ts...>, Trait>::value + (Trait<T0>::value ? 1 : 0);
    };
    template <template <class> class Trait>
    struct list_count_fulfilling<type_list<>, Trait>
    {
        static constexpr size_t value = 0;
    };

    // Given a list of types and a boolean type trait, returns the first type for which the trait is true.
    template <class List, template <class> class Trait> struct list_find_first_fulfilling;
    template <class List, template <class> class Trait> using list_find_first_fulfilling_t = typename list_find_first_fulfilling<List, Trait>::type;
    template <class T0, class... Ts, template <class> class Trait>
    struct list_find_first_fulfilling<type_list<T0, Ts...>, Trait>
        : std::conditional_t<Trait<T0>::value, identity_type<T0>, list_find_first_fulfilling<type_list<Ts...>, Trait>>
    {
    };
    template <template <class> class Trait>
    struct list_find_first_fulfilling<type_list<>, Trait>
    {
        // empty
    };

    // Construct a list of monotonically increasing constants with a given length and starting value.
    template <size_t length, class T = int, T start = 0> struct iota_list;
    template <size_t length, class T = int, T start = 0> using iota_list_t = typename iota_list<length, T, start>::type;
    template <class T, T start>
    struct iota_list<0, T, start>
    {
        using type = value_list<>;
    };
    template <size_t length, class T, T start>
    struct iota_list
    {
        using type = list_prepend_t<iota_list_t<length - 1, T, start + 1>, constant<start>>;
    };
}

#endif
