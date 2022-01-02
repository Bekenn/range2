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

#include <stdext/traits.h>

#include <cstddef>


namespace stdext
{
    // A constant value.
    template <auto v>
    struct constant
    {
        using type = decltype(v);
        static constexpr auto value = v;
    };

    // Identity metafunction.
    template <typename T> struct identity_type;
    template <typename T> using identity_type_t = typename identity_type<T>::type;
    template <typename T>
    struct identity_type
    {
        using type = T;
    };

    // A list of types.
    template <typename...> struct type_list { };

    // A list of values.
    template <auto...> struct value_list { };

    // Retrieve the first element from a list.
    template <typename List> struct list_head;
    template <typename List> using list_head_t = typename list_head<List>::type;
    template <typename List> constexpr auto list_head_v = list_head<List>::value;
    template <typename T, typename... Ts>
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
    template <typename List> struct list_tail;
    template <typename List> using list_tail_t = typename list_tail<List>::type;
    template <typename T, typename... Ts>
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
    template <typename List> struct list_length;
    template <typename List> constexpr auto list_length_v = list_length<List>::value;
    template <typename... Ts>
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
    template <typename List, size_t n> struct list_element;
    template <typename List, size_t n> using list_element_t = typename list_element<List, n>::type;
    template <typename List, size_t n> constexpr auto list_element_v = list_element<List, n>::value;
    template <typename T, typename... Ts, size_t n>
    struct list_element<type_list<T, Ts...>, n>
    {
        using type = list_element_t<type_list<Ts...>, n - 1>;
    };
    template <typename T, typename... Ts>
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
    template <typename List, typename T> struct list_index_of;
    template <typename List, typename T> constexpr auto list_index_of_v = list_index_of<List, T>::value;
    template <typename T0, typename... Ts, typename T>
    struct list_index_of<type_list<T0, Ts...>, T>
    {
        static constexpr size_t value = list_index_of_v<type_list<Ts...>, T> + 1;
    };
    template <typename T, typename... Ts>
    struct list_index_of<type_list<T, Ts...>, T>
    {
        static constexpr size_t value = 0;
    };
    template <typename T>
    struct list_index_of<type_list<>, T>
    {
        static constexpr size_t value = 0;
    };
    template <auto v0, auto... vs, auto v>
    struct list_index_of<value_list<v0, vs...>, constant<v>>
    {
        static constexpr size_t value = list_index_of_v<value_list<vs...>, constant<v>> + 1;
    };
    template <auto v, auto... vs>
    struct list_index_of<value_list<v, vs...>, constant<v>>
    {
        static constexpr size_t value = 0;
    };
    template <auto v>
    struct list_index_of<value_list<>, constant<v>>
    {
        static constexpr size_t value = 0;
    };

    // Add a new element to the front of a list.
    template <typename List, typename T> struct list_prepend;
    template <typename List, typename T> using list_prepend_t = typename list_prepend<List, T>::type;
    template <typename... Ts, typename T>
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
    template <typename T, typename List> struct list_append;
    template <typename List, typename T> using list_append_t = typename list_append<List, T>::type;
    template <typename... Ts, typename T>
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
    template <typename... Lists> struct list_concat;
    template <typename... Lists> using list_concat_t = typename list_concat<Lists...>::type;
    template <typename... Ts, typename... Us, typename... Lists>
    struct list_concat<type_list<Ts...>, type_list<Us...>, Lists...>
    {
        using type = typename list_concat<type_list<Ts..., Us...>, Lists...>::type;
    };
    template <typename... Ts>
    struct list_concat<type_list<Ts...>>
    {
        using type = type_list<Ts...>;
    };
    template <auto... vs, auto... ws, typename... Lists>
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
    template <typename List, size_t n> struct list_take;
    template <typename List, size_t n> using list_take_t = typename list_take<List, n>::type;
    template <typename T0, typename... Ts, size_t n>
    struct list_take<type_list<T0, Ts...>, n>
    {
        using type = list_prepend_t<list_take_t<type_list<Ts...>, n - 1>, T0>;
    };
    template <typename T0, typename... Ts>
    struct list_take<type_list<T0, Ts...>, 0>
    {
        using type = type_list<>;
    };
    template <>
    struct list_take<type_list<>, 0>
    {
        using type = type_list<>;
    };
    template <auto v0, auto... vs, size_t n>
    struct list_take<value_list<v0, vs...>, n>
    {
        using type = list_prepend_t<list_take_t<value_list<vs...>, n - 1>, constant<v0>>;
    };
    template <auto v0, auto... vs>
    struct list_take<value_list<v0, vs...>, 0>
    {
        using type = value_list<>;
    };
    template <>
    struct list_take<value_list<>, 0>
    {
        using type = value_list<>;
    };

    // Retrieve the list of all elements after the first n elements in a list.
    template <typename List, size_t n> struct list_drop;
    template <typename List, size_t n> using list_drop_t = typename list_drop<List, n>::type;
    template <typename T0, typename... Ts, size_t n>
    struct list_drop<type_list<T0, Ts...>, n>
    {
        using type = list_drop_t<type_list<Ts...>, n - 1>;
    };
    template <typename T0, typename... Ts>
    struct list_drop<type_list<T0, Ts...>, 0>
    {
        using type = type_list<T0, Ts...>;
    };
    template <>
    struct list_drop<type_list<>, 0>
    {
        using type = type_list<>;
    };
    template <auto v0, auto... vs, size_t n>
    struct list_drop<value_list<v0, vs...>, n>
    {
        using type = list_drop_t<value_list<vs...>, n - 1>;
    };
    template <auto v0, auto... vs>
    struct list_drop<value_list<v0, vs...>, 0>
    {
        using type = value_list<v0, vs...>;
    };
    template <>
    struct list_drop<value_list<>, 0>
    {
        using type = value_list<>;
    };

    // Insert the given element into a list at the given location.
    template <typename List, size_t n, typename T> struct list_insert_element;
    template <typename List, size_t n, typename T> using list_insert_element_t = typename list_insert_element<List, n, T>::type;
    template <typename... Ts, size_t n, typename T>
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
    template <size_t n, typename T> struct list_insert_elements_arg { };
    template <typename List, typename... Args> struct list_insert_elements;
    template <typename List, typename... Args> using list_insert_elements_t = typename list_insert_elements<List, Args...>::type;
    template <typename List, size_t n0, typename T0, size_t... ns, typename... Ts>
    struct list_insert_elements<List, list_insert_elements_arg<n0, T0>, list_insert_elements_arg<ns, Ts>...>
    {
        using type = list_insert_elements_t<list_insert_element_t<List, n0, T0>, list_insert_elements_arg<ns < n0 ? ns : ns + 1, Ts>...>;
    };
    template <typename List>
    struct list_insert_elements<List>
    {
        using type = List;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by prepending a value onto its corresponding list.
    template <typename Lists, typename List> struct list_prepend_lists;
    template <typename Lists, typename List> using list_prepend_lists_t = typename list_prepend_lists<Lists, List>::type;
    template <typename... Lists, typename... Ts>
    struct list_prepend_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_prepend_t<Lists, Ts>...>;
    };
    template <typename... Lists, auto... vs>
    struct list_prepend_lists<type_list<Lists...>, value_list<vs...>>
    {
        using type = type_list<list_prepend_t<Lists, constant<vs>>...>;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by appending a value onto its corresponding list.
    template <typename Lists, typename List> struct list_append_lists;
    template <typename Lists, typename List> using list_append_lists_t = typename list_append_lists<Lists, List>::type;
    template <typename... Lists, typename... Ts>
    struct list_append_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_append_t<Lists, Ts>...>;
    };
    template <typename... Lists, auto... vs>
    struct list_append_lists<type_list<Lists...>, value_list<vs...>>
    {
        using type = type_list<list_append_t<Lists, constant<vs>>...>;
    };

    // Given several lists of lists, construct a list of lists, each constructed by concatenating corresponding lists.
    template <typename... Listss> struct list_concat_lists;
    template <typename... Listss> using list_concat_lists_t = typename list_concat_lists<Listss...>::type;
    template <typename... Lists>
    struct list_concat_lists<type_list<Lists...>>
    {
        using type = type_list<Lists...>;
    };
    template <typename... Lists1, typename... Lists2, typename... Listss>
    struct list_concat_lists<type_list<Lists1...>, type_list<Lists2...>, Listss...>
    {
        using type = list_concat_lists_t<type_list<list_concat_t<Lists1, Lists2>...>, Listss...>;
    };

    // Given a list of lists, construct a list of lists, each constructed with corresponding elements from all lists.
    template <typename Lists> struct list_transpose;
    template <typename Lists> using list_transpose_t = typename list_transpose<Lists>::type;

    namespace _private
    {
        template <typename Lists, size_t length> struct list_transpose;
        template <typename Lists, size_t length> using list_transpose_t = typename list_transpose<Lists, length>::type;
        template <typename... Lists, size_t length>
        struct list_transpose<type_list<Lists...>, length>
        {
            static_assert((... && (list_length_v<Lists> == length)));
            using type = list_concat_t<type_list<list_concat_t<list_take_t<Lists, 1>...>>, list_transpose_t<type_list<list_drop_t<Lists, 1>...>, length - 1>>;
        };
        template <typename... Lists>
        struct list_transpose<type_list<Lists...>, 1>
        {
            static_assert((... && (list_length_v<Lists> == 1)));
            using type = type_list<list_concat_t<Lists...>>;
        };
        template <typename List>
        struct list_transpose<type_list<List>, 0>
        {
            static_assert(list_length_v<List> == 0);
            using type = type_list<List>;
        };
    }

    template <typename List, typename... Lists>
    struct list_transpose<type_list<List, Lists...>>
    {
        using type = _private::list_transpose_t<type_list<List, Lists...>, list_length_v<List>>;
    };

    // Given several lists, construct a list of lists, each constructed with corresponding elements from all lists.
    template <typename... Lists> struct list_zip;
    template <typename... Lists> using list_zip_t = typename list_zip<Lists...>::type;
    template <typename... Lists>
    struct list_zip
    {
        using type = list_transpose_t<type_list<Lists...>>;
    };

    // Given a template taking variadic type arguments, instantiate that template with types from a list.
    template <template <typename...> typename Templ, typename List> struct list_apply;
    template <template <typename...> typename Templ, typename List> using list_apply_t = typename list_apply<Templ, List>::type;
    template <template <typename...> typename Templ, typename... Ts>
    struct list_apply<Templ, type_list<Ts...>>
    {
        using type = Templ<Ts...>;
    };

    // Given a template taking variadic type arguments and a list of types, conditionally invoke list_apply.
    template <bool condition, template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList> struct list_conditional_apply;
    template <bool condition, template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList> using list_conditional_apply_t = typename list_conditional_apply<condition, TrueTempl, TrueList, FalseTempl, FalseList>::type;
    template <bool condition, template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList>
    struct list_conditional_apply
    {
        using type = list_apply_t<FalseTempl, FalseList>;
    };
    template <template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList>
    struct list_conditional_apply<true, TrueTempl, TrueList, FalseTempl, FalseList>
    {
        using type = list_apply_t<TrueTempl, TrueList>;
    };

    // Given a list of types and a boolean type trait, returns whether the trait is true for all types.
    template <typename List, template <typename> typename Trait> struct list_all_of;
    template <typename List, template <typename> typename Trait> constexpr auto list_all_of_v = list_all_of<List, Trait>::value;
    template <typename T0, typename... Ts, template <typename> typename Trait>
    struct list_all_of<type_list<T0, Ts...>, Trait>
    {
        static constexpr auto value = Trait<T0>::value && list_all_of<type_list<Ts...>, Trait>::value;
    };
    template <template <typename> typename Trait>
    struct list_all_of<type_list<>, Trait>
    {
        static constexpr auto value = true;
    };

    // Given a list of types and a boolean type trait, returns whether the trait is true for any type.
    template <typename List, template <typename> typename Trait> struct list_any_of;
    template <typename List, template <typename> typename Trait> constexpr auto list_any_of_v = list_any_of<List, Trait>::value;
    template <typename T0, typename... Ts, template <typename> typename Trait>
    struct list_any_of<type_list<T0, Ts...>, Trait>
    {
        static constexpr auto value = Trait<T0>::value || list_any_of<type_list<Ts...>, Trait>::value;
    };
    template <template <typename> typename Trait>
    struct list_any_of<type_list<>, Trait>
    {
        static constexpr auto value = false;
    };

    // Given a list of types and a boolean type trait, returns whether the trait is true for no type.
    template <typename List, template <typename> typename Trait> struct list_none_of;
    template <typename List, template <typename> typename Trait> constexpr auto list_none_of_v = list_none_of<List, Trait>::value;
    template <typename List, template <typename> typename Trait>
    struct list_none_of
    {
        static constexpr auto value = !list_any_of<List, Trait>::value;
    };

    // Given a list of types and a boolean type trait, returns the number of types for which the trait is true.
    template <typename List, template <typename> typename Trait> struct list_count_fulfilling;
    template <typename List, template <typename> typename Trait> constexpr auto list_count_fulfilling_v = list_count_fulfilling<List, Trait>::value;
    template <typename T0, typename... Ts, template <typename> typename Trait>
    struct list_count_fulfilling<type_list<T0, Ts...>, Trait>
    {
        static constexpr auto value = list_count_fulfilling<type_list<Ts...>, Trait>::value + (Trait<T0>::value ? 1 : 0);
    };
    template <template <typename> typename Trait>
    struct list_count_fulfilling<type_list<>, Trait>
    {
        static constexpr size_t value = 0;
    };

    // Given a list of types and a boolean type trait, returns the first type for which the trait is true.
    template <typename List, template <typename> typename Trait> struct list_find_first_fulfilling;
    template <typename List, template <typename> typename Trait> using list_find_first_fulfilling_t = typename list_find_first_fulfilling<List, Trait>::type;
    template <typename T0, typename... Ts, template <typename> typename Trait>
    struct list_find_first_fulfilling<type_list<T0, Ts...>, Trait>
        : std::conditional_t<Trait<T0>::value, identity_type<T0>, list_find_first_fulfilling<type_list<Ts...>, Trait>>
    {
    };
    template <template <typename> typename Trait>
    struct list_find_first_fulfilling<type_list<>, Trait>
    {
        // empty
    };

    // Construct a list of monotonically increasing constants with a given length and starting value.
    template <size_t length, typename T = int, T start = 0> struct iota_list;
    template <size_t length, typename T = int, T start = 0> using iota_list_t = typename iota_list<length, T, start>::type;
    template <typename T, T start>
    struct iota_list<0, T, start>
    {
        using type = value_list<>;
    };
    template <size_t length, typename T, T start>
    struct iota_list
    {
        using type = list_prepend_t<iota_list_t<length - 1, T, start + 1>, constant<start>>;
    };

    // Construct a list of monotonically increasing indices with a given length and starting value
    template <size_t length, size_t start = 0> using index_list = iota_list<length, size_t, start>;
    template <size_t length, size_t start = 0> using index_list_t = iota_list_t<length, size_t, start>;
}

#endif
