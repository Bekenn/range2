//
//  iterator.h
//  stdext
//
//  Created by James Touton on 12/15/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_ITERATOR_INCLUDED
#define STDEXT_ITERATOR_INCLUDED
#pragma once

#include <stdext/utility.h>

#include <iterator>


namespace stdext
{
    // Useful declarations
    using std::begin;
    using std::end;

    namespace _private
    {
        template <typename X, typename = decltype(*declval<X&>())>
        std::conjunction<
            is_semiregular<X>,
            std::is_same<decltype(++declval<X&>()), X&>>
        test_is_iterator(int);
        template <typename X> false_type test_is_iterator(...);
    }
    template <typename T> struct is_iterator : decltype(_private::test_is_iterator<T>(0)) { };

    template <typename T> using iterator_category = typename std::iterator_traits<remove_cvref_t<T>>::iterator_category;

    template <typename T> using iterator_value_type = std::remove_cv_t<typename std::iterator_traits<T>::value_type>;
    template <typename T> using iterator_difference_type = typename std::iterator_traits<T>::difference_type;
    template <typename T> using iterator_pointer_type = typename std::iterator_traits<T>::pointer;
    template <typename T> using iterator_reference_type = typename std::iterator_traits<T>::reference;
    template <typename T> using iterator_size_type = std::make_unsigned_t<iterator_difference_type<T>>;

    namespace _private
    {
        template <typename X, typename Traits = std::iterator_traits<X>, typename T = typename Traits::value_type>
        std::conjunction<
            is_iterator<X>,
            is_equality_comparable<X>,
            std::is_same<decltype(*declval<X>()), typename Traits::reference>,
            std::is_convertible<decltype(*declval<X&>()++), T>,
            std::is_base_of<std::input_iterator_tag, typename Traits::iterator_category>>
        test_is_input_iterator(int);
        template <typename X> false_type test_is_input_iterator(...);
    }

    template <typename T>
    struct is_input_iterator : decltype(_private::test_is_input_iterator<T>(0)) { };

    namespace _private
    {
        template <typename X, typename Traits = std::iterator_traits<X>, typename T = typename Traits::value_type>
        std::conjunction<
            is_input_iterator<X>,
            std::is_default_constructible<X>,
            std::is_reference<typename Traits::reference>,
            std::is_same<remove_cvref_t<typename Traits::reference>, std::remove_cv_t<T>>,
            std::is_convertible<decltype(declval<X&>()++), const X&>,
            std::is_same<decltype(*declval<X&>()++), typename Traits::reference>,
            std::is_base_of<std::forward_iterator_tag, typename Traits::iterator_category>>
        test_is_forward_iterator(int);
        template <typename X> false_type test_is_forward_iterator(...);
    }

    template <typename T>
    struct is_forward_iterator : decltype(_private::test_is_forward_iterator<T>(0)) { };

    namespace _private
    {
        template <typename X, typename Traits = std::iterator_traits<X>>
        std::conjunction<
            is_forward_iterator<X>,
            std::is_same<decltype(--declval<X&>()), X&>,
            std::is_convertible<decltype(declval<X&>()--), const X&>,
            std::is_same<decltype(*declval<X&>()--), typename Traits::reference>,
            std::is_base_of<std::bidirectional_iterator_tag, typename Traits::iterator_category>>
        test_is_bidirectional_iterator(int);
        template <typename X> false_type test_is_bidirectional_iterator(...);
    }

    template <typename T>
    struct is_bidirectional_iterator : decltype(_private::test_is_bidirectional_iterator<T>(0)) { };

    namespace _private
    {
        template <typename X, typename Traits = std::iterator_traits<X>>
        std::conjunction<
            is_bidirectional_iterator<X>,
            std::is_same<decltype(declval<X&>() += declval<typename Traits::difference_type>()), X&>,
            std::is_same<decltype(declval<X>() + declval<typename Traits::difference_type>()), X>,
            std::is_same<decltype(declval<typename Traits::difference_type>() + declval<X>()), X>,
            std::is_same<decltype(declval<X&>() -= declval<typename Traits::difference_type>()), X&>,
            std::is_same<decltype(declval<X>() - declval<typename Traits::difference_type>()), X>,
            std::is_same<decltype(declval<X>() - declval<X>()), typename Traits::difference_type>,
            std::is_convertible<decltype(declval<X>()[declval<typename Traits::difference_type>()]), typename Traits::reference>,
            std::is_constructible<bool, decltype(declval<X>() < declval<X>())>,
            std::is_constructible<bool, decltype(declval<X>() > declval<X>())>,
            std::is_constructible<bool, decltype(declval<X>() <= declval<X>())>,
            std::is_constructible<bool, decltype(declval<X>() >= declval<X>())>,
            std::is_base_of<std::random_access_iterator_tag, typename Traits::iterator_category>>
        test_is_random_access_iterator(int);
        template <typename X> false_type test_is_random_access_iterator(...);
    }

    template <typename T>
    struct is_random_access_iterator : decltype(_private::test_is_random_access_iterator<T>(0)) { };

    template <typename T> constexpr auto is_iterator_v = is_iterator<T>::value;
    template <typename T> constexpr auto is_input_iterator_v = is_input_iterator<T>::value;
    template <typename T> constexpr auto is_forward_iterator_v = is_forward_iterator<T>::value;
    template <typename T> constexpr auto is_bidirectional_iterator_v = is_bidirectional_iterator<T>::value;
    template <typename T> constexpr auto is_random_access_iterator_v = is_random_access_iterator<T>::value;

    namespace _private
    {
        template <typename T, typename Iterator>
        std::conjunction<
            is_semiregular<T>,
            is_iterator<Iterator>,
            is_equality_comparable_with<T, Iterator>>
        test_is_sentinel_for(int);
        template <typename T, typename Iterator> false_type test_is_sentinel_for(...);
    }

    template <typename T, typename Iterator>
    struct is_sentinel_for : decltype(_private::test_is_sentinel_for<T, Iterator>(0)) { };

    template <typename T, typename Iterator> constexpr auto is_sentinel_for_v = is_sentinel_for<T, Iterator>::value;

    namespace _private
    {
        template <typename T, typename Iterator = decltype(begin(declval<T&>())), typename Sentinel = decltype(end(declval<T&>()))>
        is_sentinel_for<Sentinel, Iterator> test_is_std_range(int);
        template <typename T> false_type test_is_std_range(...);
    }

    template <typename T> struct is_std_range : decltype(_private::test_is_std_range<T>(0)) { };
    template <> struct is_std_range<void> : false_type { };
    template <> struct is_std_range<const void> : false_type { };
    template <> struct is_std_range<volatile void> : false_type { };
    template <> struct is_std_range<const volatile void> : false_type { };
    template <typename T> constexpr auto is_std_range_v = is_std_range<T>::value;

    template <typename StdRange> using iterator_type = decltype(begin(declval<StdRange&>()));
    template <typename StdRange> using sentinel_type = decltype(end(declval<StdRange&>()));

    namespace _private
    {
        template <typename T, typename Iterator = iterator_type<T>>
        std::conjunction<
            is_std_range<T>,
            is_input_iterator<Iterator>>
        test_is_std_input_range(int);
        template <typename T> false_type test_is_std_input_range(...);

        template <typename T, typename Iterator = iterator_type<T>>
        std::conjunction<
            is_std_range<T>,
            is_forward_iterator<Iterator>>
        test_is_std_forward_range(int);
        template <typename T> false_type test_is_std_forward_range(...);

        template <typename T, typename Iterator = iterator_type<T>>
        std::conjunction<
            is_std_range<T>,
            is_bidirectional_iterator<Iterator>>
        test_is_std_bidirectional_range(int);
        template <typename T> false_type test_is_std_bidirectional_range(...);

        template <typename T, typename Iterator = iterator_type<T>>
        std::conjunction<
            is_std_range<T>,
            is_random_access_iterator<Iterator>>
        test_is_std_random_access_range(int);
        template <typename T> false_type test_is_std_random_access_range(...);
    }

    template <typename T> struct is_std_input_range : decltype(_private::test_is_std_input_range<T>(0)) { };
    template <typename T> struct is_std_forward_range : decltype(_private::test_is_std_forward_range<T>(0)) { };
    template <typename T> struct is_std_bidirectional_range : decltype(_private::test_is_std_bidirectional_range<T>(0)) { };
    template <typename T> struct is_std_random_access_range : decltype(_private::test_is_std_random_access_range<T>(0)) { };

    template <typename T> constexpr auto is_std_input_range_v = is_std_input_range<T>::value;
    template <typename T> constexpr auto is_std_forward_range_v = is_std_forward_range<T>::value;
    template <typename T> constexpr auto is_std_bidirectional_range_v = is_std_bidirectional_range<T>::value;
    template <typename T> constexpr auto is_std_random_access_range_v = is_std_random_access_range<T>::value;

    template <typename Iterator>
    class iterator_proxy
    {
    public:
        using value_type = typename std::iterator_traits<Iterator>::value_type;

    public:
        iterator_proxy(const value_type& value) : _value(value) { }
        iterator_proxy(value_type&& value) : _value(stdext::move(value)) { }

    public:
        value_type&& operator * () && { return stdext::move(_value); }

    private:
        value_type _value;
    };
}

#endif
