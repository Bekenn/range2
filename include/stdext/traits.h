//
//  traits.h
//  stdext
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_TRAITS_INCLUDED
#define STDEXT_TRAITS_INCLUDED
#pragma once

#include <stdext/types.h>

#include <type_traits>


#define STDEXT_REQUIRED(...) ::std::enable_if_t<(__VA_ARGS__), ::stdext::nullptr_t>
#define STDEXT_REQUIRES(...) STDEXT_REQUIRED(__VA_ARGS__) = nullptr

#define DECLARE_HAS_INNER_TYPE(Inner)                                   \
template <typename T> struct has_inner_type_##Inner                     \
{                                                                       \
    template <typename U> static true_type test(typename U::Inner*);    \
    template <typename U> static false_type test(...);                  \
    static constexpr bool value = decltype(test<T>(nullptr))::value;    \
}
#define HAS_INNER_TYPE_T(T, Inner) has_inner_type_##Inner<T>
#define HAS_INNER_TYPE(T, Inner) HAS_INNER_TYPE_T(T, Inner)::value

#define DECLARE_HAS_METHOD(MethodName)                                                                          \
template <typename T, typename... ArgTypes> struct has_method_##MethodName                                      \
{                                                                                                               \
    template <typename U> static true_type test(decltype(declval<U>().MethodName(declval<ArgTypes>()...))*);    \
    template <typename U> static false_type test(...);                                                          \
    static constexpr bool value = decltype(test<T>(nullptr))::value;                                            \
}
#define HAS_METHOD_T(T, MethodName, ...) has_method_##MethodName<T, ## __VA_ARGS__>
#define HAS_METHOD(T, MethodName, ...) HAS_METHOD_T(T, MethodName, ## __VA_ARGS__)::value

namespace stdext
{
    // declval
    template <typename T> std::add_rvalue_reference_t<T> declval();

    // Useful declarations
    using std::integral_constant;
    using std::bool_constant;
    using std::true_type;
    using std::false_type;

    // Debugging helpers
    template <typename T> struct error_type;
    template <auto V> struct error_value;

    // Test whether T1() == T2() is well-formed
    namespace _private
    {
        template <typename T1, typename T2>
        static true_type test_equality_comparable(decltype(
            declval<T1>() == declval<T2>()
            && declval<T2>() == declval<T1>()
            && declval<T1>() != declval<T2>()
            && declval<T2>() != declval<T1>())*);
        template <typename T1, typename T2>
        static false_type test_equality_comparable(...);
    }

    template <typename T1, typename T2>
    struct is_equality_comparable_with : bool_constant<decltype(_private::test_equality_comparable<T1, T2>(0))::value> { };
    template <typename T1, typename T2>
    constexpr bool is_equality_comparable_with_v = is_equality_comparable_with<T1, T2>::value;

    template <typename T> struct is_equality_comparable : is_equality_comparable_with<const T&, const T&> { };
    template <> struct is_equality_comparable<void> : false_type { };
    template <> struct is_equality_comparable<const void> : false_type { };
    template <> struct is_equality_comparable<volatile void> : false_type { };
    template <> struct is_equality_comparable<const volatile void> : false_type { };

    template <typename T> constexpr bool is_equality_comparable_v = is_equality_comparable<T>::value;

    // Conversion from type From to type possibly-const To, where To is const-qualified if From is const-qualified.
    template <typename From, typename To> struct preserve_const { using type = To; };
    template <typename From, typename To> struct preserve_const<const From, To> { using type = const To; };

    template <typename From, typename To> using preserve_const_t = typename preserve_const<From, To>::type;

    // Type trait returning one of [u]intN_t based on the size and signedness of T
    template <typename T> struct equivalent_sized_type;
    template <typename T> using equivalent_sized_type_t = typename equivalent_sized_type<T>::type;

    namespace _private
    {
        template <typename T, bool is_integral = std::is_integral_v<T>> struct equivalent_sized_type_base;
        template <size_t size, bool is_signed> struct make_sized_integral_type;
        template <size_t size, bool is_signed> using make_sized_integral_type_t = typename make_sized_integral_type<size, is_signed>::type;

        template <typename T>
        struct equivalent_sized_type_base<T, false>
        {
        };

        template <typename T>
        struct equivalent_sized_type_base<T, true>
        {
            using type = make_sized_integral_type_t<sizeof(T), std::is_signed_v<T>>;
        };

        template <size_t size, bool is_signed>
        struct make_sized_integral_type
        {
        };

        template <bool is_signed>
        struct make_sized_integral_type<1, is_signed>
        {
            using type = std::conditional_t<is_signed, int8_t, uint8_t>;
        };
        template <bool is_signed>
        struct make_sized_integral_type<2, is_signed>
        {
            using type = std::conditional_t<is_signed, int16_t, uint16_t>;
        };
        template <bool is_signed>
        struct make_sized_integral_type<4, is_signed>
        {
            using type = std::conditional_t<is_signed, int32_t, uint32_t>;
        };
        template <bool is_signed>
        struct make_sized_integral_type<8, is_signed>
        {
            using type = std::conditional_t<is_signed, int64_t, uint64_t>;
        };
    }

    template <typename T> struct equivalent_sized_type : _private::equivalent_sized_type_base<T> { };

    namespace _private
    {
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct value_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct position_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct difference_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct size_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct pointer_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct reference_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct iterator_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct sentinel_type_of { };
        template <typename T, template <typename> typename IsT, bool = IsT<T>::value> struct generator_type_of { };
    }

    template <typename T, template <typename> typename IsT> struct value_type : _private::value_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct position_type : _private::position_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct difference_type : _private::difference_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct size_type : _private::size_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct pointer_type : _private::pointer_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct reference_type : _private::reference_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct iterator_type : _private::iterator_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct sentinel_type : _private::sentinel_type_of<T, IsT> { };
    template <typename T, template <typename> typename IsT> struct generator_type : _private::generator_type_of<T, IsT> { };

    template <typename T, template <typename> typename IsT> using value_type_t = typename value_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using position_type_t = typename position_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using difference_type_t = typename difference_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using size_type_t = typename size_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using pointer_type_t = typename pointer_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using reference_type_t = typename reference_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using iterator_type_t = typename iterator_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using sentinel_type_t = typename sentinel_type<T, IsT>::type;
    template <typename T, template <typename> typename IsT> using generator_type_t = typename generator_type<T, IsT>::type;
}

#endif
