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
    template <auto V>
    struct constant
    {
        using type = decltype(V);
        static constexpr auto value = V;
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
    template <auto V, auto... Vs>
    struct list_head<value_list<V, Vs...>>
    {
        static constexpr auto value = V;
    };

    // Retrieve the list of all elements except the first from a list.
    template <typename List> struct list_tail;
    template <typename List> using list_tail_t = typename list_tail<List>::type;
    template <typename T, typename... Ts>
    struct list_tail<type_list<T, Ts...>>
    {
        using type = type_list<Ts...>;
    };
    template <auto V, auto... Vs>
    struct list_tail<value_list<V, Vs...>>
    {
        using type = value_list<Vs...>;
    };

    // Retrieve the length of a list.
    template <typename List> struct list_length;
    template <typename List> constexpr auto list_length_v = list_length<List>::value;
    template <typename... Ts>
    struct list_length<type_list<Ts...>>
    {
        static constexpr size_t value = sizeof...(Ts);
    };
    template <auto... Vs>
    struct list_length<value_list<Vs...>>
    {
        static constexpr size_t value = sizeof...(Vs);
    };

    // Retrieve the nth element from a list.
    template <typename List, size_t N> struct list_element;
    template <typename List, size_t N> using list_element_t = typename list_element<List, N>::type;
    template <typename List, size_t N> constexpr auto list_element_v = list_element<List, N>::value;
    template <typename T, typename... Ts, size_t N>
    struct list_element<type_list<T, Ts...>, N>
    {
        using type = list_element_t<type_list<Ts...>, N - 1>;
    };
    template <typename T, typename... Ts>
    struct list_element<type_list<T, Ts...>, 0>
    {
        using type = T;
    };
    template <auto V, auto... Vs, size_t N>
    struct list_element<value_list<V, Vs...>, N>
    {
        static constexpr auto value = list_element_v<value_list<Vs...>, N - 1>;
    };
    template <auto V, auto... Vs>
    struct list_element<value_list<V, Vs...>, 0>
    {
        static constexpr auto value = V;
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
    template <auto V0, auto... Vs, auto V>
    struct list_index_of<value_list<V0, Vs...>, constant<V>>
    {
        static constexpr size_t value = list_index_of_v<value_list<Vs...>, constant<V>> + 1;
    };
    template <auto V, auto... Vs>
    struct list_index_of<value_list<V, Vs...>, constant<V>>
    {
        static constexpr size_t value = 0;
    };
    template <auto V>
    struct list_index_of<value_list<>, constant<V>>
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
    template <auto... Vs, auto V>
    struct list_prepend<value_list<Vs...>, constant<V>>
    {
        using type = value_list<V, Vs...>;
    };

    // Add a new element to the back of alist.
    template <typename T, typename List> struct list_append;
    template <typename List, typename T> using list_append_t = typename list_append<List, T>::type;
    template <typename... Ts, typename T>
    struct list_append<type_list<Ts...>, T>
    {
        using type = type_list<Ts..., T>;
    };
    template <auto... Vs, auto V>
    struct list_append<value_list<Vs...>, constant<V>>
    {
        using type = value_list<Vs..., V>;
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
    template <auto... Vs, auto... Ws, typename... Lists>
    struct list_concat<value_list<Vs...>, value_list<Ws...>, Lists...>
    {
        using type = typename list_concat<value_list<Vs..., Ws...>, Lists...>::type;
    };
    template <auto... Vs>
    struct list_concat<value_list<Vs...>>
    {
        using type = value_list<Vs...>;
    };

    // Retrieve the list of the first N elements in a list.
    template <typename List, size_t N> struct list_take;
    template <typename List, size_t N> using list_take_t = typename list_take<List, N>::type;
    template <typename T0, typename... Ts, size_t N>
    struct list_take<type_list<T0, Ts...>, N>
    {
        using type = list_prepend_t<list_take_t<type_list<Ts...>, N - 1>, T0>;
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
    template <auto V0, auto... Vs, size_t N>
    struct list_take<value_list<V0, Vs...>, N>
    {
        using type = list_prepend_t<list_take_t<value_list<Vs...>, N - 1>, constant<V0>>;
    };
    template <auto V0, auto... Vs>
    struct list_take<value_list<V0, Vs...>, 0>
    {
        using type = value_list<>;
    };
    template <>
    struct list_take<value_list<>, 0>
    {
        using type = value_list<>;
    };

    // Retrieve the list of all elements after the first N elements in a list.
    template <typename List, size_t N> struct list_drop;
    template <typename List, size_t N> using list_drop_t = typename list_drop<List, N>::type;
    template <typename T0, typename... Ts, size_t N>
    struct list_drop<type_list<T0, Ts...>, N>
    {
        using type = list_drop_t<type_list<Ts...>, N - 1>;
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
    template <auto V0, auto... Vs, size_t N>
    struct list_drop<value_list<V0, Vs...>, N>
    {
        using type = list_drop_t<value_list<Vs...>, N - 1>;
    };
    template <auto V0, auto... Vs>
    struct list_drop<value_list<V0, Vs...>, 0>
    {
        using type = value_list<V0, Vs...>;
    };
    template <>
    struct list_drop<value_list<>, 0>
    {
        using type = value_list<>;
    };

    // Insert the given element into a list at the given location.
    template <typename List, size_t N, typename T> struct list_insert_element;
    template <typename List, size_t N, typename T> using list_insert_element_t = typename list_insert_element<List, N, T>::type;
    template <typename... Ts, size_t N, typename T>
    struct list_insert_element<type_list<Ts...>, N, T>
    {
        using type = list_concat_t<list_take_t<type_list<Ts...>, N>, type_list<T>, list_drop_t<type_list<Ts...>, N>>;
    };
    template <auto... Vs, size_t N, auto V>
    struct list_insert_element<value_list<Vs...>, N, constant<V>>
    {
        using type = list_concat_t<list_take_t<value_list<Vs...>, N>, value_list<V>, list_drop_t<value_list<Vs...>, N>>;
    };

    // Insert the given elements into a list at the given locations.
    template <size_t N, typename T> struct list_insert_elements_arg { };
    template <typename List, typename... Args> struct list_insert_elements;
    template <typename List, typename... Args> using list_insert_elements_t = typename list_insert_elements<List, Args...>::type;
    template <typename List, size_t N0, typename T0, size_t... Ns, typename... Ts>
    struct list_insert_elements<List, list_insert_elements_arg<N0, T0>, list_insert_elements_arg<Ns, Ts>...>
    {
        using type = list_insert_elements_t<list_insert_element_t<List, N0, T0>, list_insert_elements_arg<Ns < N0 ? Ns : Ns + 1, Ts>...>;
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
    template <typename... Lists, auto... Vs>
    struct list_prepend_lists<type_list<Lists...>, value_list<Vs...>>
    {
        using type = type_list<list_prepend_t<Lists, constant<Vs>>...>;
    };

    // Given a list of lists and a list of values, construct a list of lists, each constructed by appending a value onto its corresponding list.
    template <typename Lists, typename List> struct list_append_lists;
    template <typename Lists, typename List> using list_append_lists_t = typename list_append_lists<Lists, List>::type;
    template <typename... Lists, typename... Ts>
    struct list_append_lists<type_list<Lists...>, type_list<Ts...>>
    {
        using type = type_list<list_append_t<Lists, Ts>...>;
    };
    template <typename... Lists, auto... Vs>
    struct list_append_lists<type_list<Lists...>, value_list<Vs...>>
    {
        using type = type_list<list_append_t<Lists, constant<Vs>>...>;
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
        template <typename Lists, size_t Length> struct list_transpose;
        template <typename Lists, size_t Length> using list_transpose_t = typename list_transpose<Lists, Length>::type;
        template <typename... Lists, size_t Length>
        struct list_transpose<type_list<Lists...>, Length>
        {
            static_assert((... && (list_length_v<Lists> == Length)));
            using type = list_concat_t<type_list<list_concat_t<list_take_t<Lists, 1>...>>, list_transpose_t<type_list<list_drop_t<Lists, 1>...>, Length - 1>>;
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
    template <bool Condition, template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList> struct list_conditional_apply;
    template <bool Condition, template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList> using list_conditional_apply_t = typename list_conditional_apply<Condition, TrueTempl, TrueList, FalseTempl, FalseList>::type;
    template <bool Condition, template <typename...> typename TrueTempl, typename TrueList, template <typename...> typename FalseTempl, typename FalseList>
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
    template <size_t Length, typename T = int, T Start = 0> struct iota_list;
    template <size_t Length, typename T = int, T Start = 0> using iota_list_t = typename iota_list<Length, T, Start>::type;
    template <typename T, T Start>
    struct iota_list<0, T, Start>
    {
        using type = value_list<>;
    };
    template <size_t Length, typename T, T Start>
    struct iota_list
    {
        using type = list_prepend_t<iota_list_t<Length - 1, T, Start + 1>, constant<Start>>;
    };

    // Construct a list of monotonically increasing indices with a given length and starting value
    template <size_t Length, size_t Start = 0> using index_list = iota_list<Length, size_t, Start>;
    template <size_t Length, size_t Start = 0> using index_list_t = iota_list_t<Length, size_t, Start>;
}

#endif
