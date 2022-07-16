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

#include <stdext/preprocessor.h>
#include <stdext/types.h>

#include <type_traits>


#define STDEXT_REQUIRED(...) ::std::enable_if_t<(__VA_ARGS__), ::stdext::nullptr_t>
#define STDEXT_REQUIRES(...) STDEXT_REQUIRED(__VA_ARGS__) = nullptr

#define STDEXT_DECLARE_HAS_INNER_TYPE(Inner)                                    \
template <typename T> struct has_inner_type_##Inner                             \
{                                                                               \
    template <typename U> static ::stdext::true_type test(typename U::Inner*);  \
    template <typename U> static ::stdext::false_type test(...);                \
    static constexpr bool value = decltype(test<T>(nullptr))::value;            \
}
#define STDEXT_HAS_INNER_TYPE(T, Inner) has_inner_type_##Inner<STDEXT_PP_DEPAREN(T)>
#define STDEXT_HAS_INNER_TYPE_V(T, Inner) STDEXT_HAS_INNER_TYPE(T, Inner)::value

#define STDEXT_DECLARE_HAS_METHOD(MethodName)                                                   \
template <typename T, typename... ArgTypes> struct has_method_##MethodName                      \
{                                                                                               \
    template <typename U> static ::stdext::true_type                                            \
        test(decltype(::stdext::declval<U>().MethodName(::stdext::declval<ArgTypes>()...))*);   \
    template <typename U> static ::stdext::false_type test(...);                                \
    static constexpr bool value = decltype(test<T>(nullptr))::value;                            \
}
#define STDEXT_HAS_METHOD(T, MethodName, ...) has_method_##MethodName<STDEXT_PP_DEPAREN(T), ## __VA_ARGS__>
#define STDEXT_HAS_METHOD_V(T, MethodName, ...) STDEXT_HAS_METHOD(T, MethodName, ## __VA_ARGS__)::value

namespace stdext
{
    // declval
    template <typename T> std::add_rvalue_reference_t<T> declval() noexcept;

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
        template <typename T1, typename T2,
            typename R1 = const std::remove_reference_t<T1>&,
            typename R2 = const std::remove_reference_t<T2>&>
        static true_type test_equality_comparable(decltype(
            declval<void (&)(bool, bool, bool, bool)>()(
                declval<R1>() == declval<R2>(),
                declval<R2>() == declval<R1>(),
                declval<R1>() != declval<R2>(),
                declval<R2>() != declval<R1>()))*);
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

    // Returns the provided type.  Useful for disabling template argument deduction.
    template <typename T> struct identity_type { using type = T; };
    template <typename T> using identity_type_t = typename identity_type<T>::type;

    // Strip top-level reference and cv qualifiers
    template <typename T> using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;
    template <typename T> using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    // Conversion from type From to type possibly-const To, where To is const-qualified if From is const-qualified.
    template <typename From, typename To> struct preserve_const { using type = To; };
    template <typename From, typename To> struct preserve_const<const From, To> { using type = const To; };

    template <typename From, typename To> using preserve_const_t = typename preserve_const<From, To>::type;

    // Type trait returning one of [u]intN_t based on the size and signedness of T
    template <typename T> struct equivalent_sized_type;
    template <typename T> using equivalent_sized_type_t = typename equivalent_sized_type<T>::type;

    namespace _private
    {
        template <typename T, bool IsIntegral = std::is_integral_v<T>> struct equivalent_sized_type_base;
        template <size_t Size, bool IsSigned> struct make_sized_integral_type;
        template <size_t Size, bool IsSigned> using make_sized_integral_type_t = typename make_sized_integral_type<Size, IsSigned>::type;

        template <typename T>
        struct equivalent_sized_type_base<T, false>
        {
        };

        template <typename T>
        struct equivalent_sized_type_base<T, true>
        {
            using type = make_sized_integral_type_t<sizeof(T), std::is_signed_v<T>>;
        };

        template <size_t Size, bool IsSigned>
        struct make_sized_integral_type
        {
        };

        template <bool IsSigned>
        struct make_sized_integral_type<1, IsSigned>
        {
            using type = std::conditional_t<IsSigned, int8_t, uint8_t>;
        };
        template <bool IsSigned>
        struct make_sized_integral_type<2, IsSigned>
        {
            using type = std::conditional_t<IsSigned, int16_t, uint16_t>;
        };
        template <bool IsSigned>
        struct make_sized_integral_type<4, IsSigned>
        {
            using type = std::conditional_t<IsSigned, int32_t, uint32_t>;
        };
        template <bool IsSigned>
        struct make_sized_integral_type<8, IsSigned>
        {
            using type = std::conditional_t<IsSigned, int64_t, uint64_t>;
        };
    }

    template <typename T> struct equivalent_sized_type : _private::equivalent_sized_type_base<T> { };
}

#endif
