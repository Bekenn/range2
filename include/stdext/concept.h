//
//  concept.h
//  stdext
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_CONCEPT_INCLUDED
#define STDEXT_CONCEPT_INCLUDED
#pragma once

#include "types.h"
#include "utility.h"

#include <type_traits>


#define REQUIRED(...) ::std::enable_if_t<(__VA_ARGS__), nullptr_t>
#define REQUIRES(...) REQUIRED(__VA_ARGS__) = nullptr

#define DECLARE_HAS_INNER_TYPE(Inner)                                \
template <class T> struct has_inner_type_##Inner                     \
{                                                                    \
    template <class U> static true_type test(typename U::Inner*);    \
    template <class U> static false_type test(...);                  \
    static constexpr bool value = decltype(test<T>(nullptr))::value; \
}
#define HAS_INNER_TYPE_T(T, Inner) has_inner_type_##Inner<T>
#define HAS_INNER_TYPE(T, Inner) HAS_INNER_TYPE_T(T, Inner)::value

#define DECLARE_HAS_METHOD(MethodName)                                                                     \
template <class T, class... ArgTypes> struct has_method_##MethodName                                       \
{                                                                                                          \
    template <class U> static true_type test(decltype(declval<U>().MethodName(declval<ArgTypes>()...))*);  \
    template <class U> static false_type test(...);                                                        \
    static constexpr bool value = decltype(test<T>(nullptr))::value;                                       \
}
#define HAS_METHOD_T(T, MethodName, ...) has_method_##MethodName<T, ## __VA_ARGS__>
#define HAS_METHOD(T, MethodName, ...) HAS_METHOD_T(T, MethodName, ## __VA_ARGS__)::value

namespace stdext
{
    // Useful declarations
    using ::std::true_type;
    using ::std::false_type;

    // Debugging helpers
    template <class T> struct error_type;
    template <class T, T v> struct error_value;

    template <class, class R = void> struct is_callable;
    template <class Function, class... ArgTypes>
    struct is_callable<Function(ArgTypes...), void>
    {
        template <class T> static true_type test(decltype(declval<T>()(declval<ArgTypes>()...))*);
        template <class T> static false_type test(...);
        static constexpr bool value = decltype(test<Function>(nullptr))::value;
    };
    template <class Function, class... ArgTypes, class R>
    struct is_callable<Function(ArgTypes...), R>
    {
        template <class T> static void test_ret(R);
        template <class T> static true_type test(decltype(test_ret<T>(declval<T>()(declval<ArgTypes>()...)))*);
        template <class T> static false_type test(...);
        static constexpr bool value = decltype(test<Function>(nullptr))::value;
    };
    template <class F, class R = void>
    constexpr bool is_callable_v = is_callable<F, R>::value;

    template <class T1, class T2>
    struct is_equality_comparable
    {
        template <class U1, class U2>
        static true_type test(decltype(declval<U1>() == declval<U2>()
            && declval<U1>() != declval<U2>())*);
        template <class U1, class U2>
        static false_type test(...);
        static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
    };
    template <class T1, class T2>
    constexpr bool is_equality_comparable_v = is_equality_comparable<T1, T2>::value;

    template <class From, class To>
    struct preserve_const
    {
        using type = To;
    };
    template <class From, class To>
    struct preserve_const<const From, To>
    {
        using type = const To;
    };
    template <class From, class To>
    using preserve_const_t = typename preserve_const<From, To>::type;

    namespace detail
    {
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct value_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct position_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct difference_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct size_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct pointer_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct reference_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct iterator_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct sentinel_type_of { };
        template <class T, template <class> class IsT, bool = IsT<T>::value> struct generator_type_of { };
    }

    template <class T, template <class> class IsT> struct value_type : detail::value_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct position_type : detail::position_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct difference_type : detail::difference_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct size_type : detail::size_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct pointer_type : detail::pointer_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct reference_type : detail::reference_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct iterator_type : detail::iterator_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct sentinel_type : detail::sentinel_type_of<T, IsT> { };
    template <class T, template <class> class IsT> struct generator_type : detail::generator_type_of<T, IsT> { };

    template <class T, template <class> class IsT> using value_type_t = typename value_type<T, IsT>::type;
    template <class T, template <class> class IsT> using position_type_t = typename position_type<T, IsT>::type;
    template <class T, template <class> class IsT> using difference_type_t = typename difference_type<T, IsT>::type;
    template <class T, template <class> class IsT> using size_type_t = typename size_type<T, IsT>::type;
    template <class T, template <class> class IsT> using pointer_type_t = typename pointer_type<T, IsT>::type;
    template <class T, template <class> class IsT> using reference_type_t = typename reference_type<T, IsT>::type;
    template <class T, template <class> class IsT> using iterator_type_t = typename iterator_type<T, IsT>::type;
    template <class T, template <class> class IsT> using sentinel_type_t = typename sentinel_type<T, IsT>::type;
    template <class T, template <class> class IsT> using generator_type_t = typename generator_type<T, IsT>::type;
}

#endif
