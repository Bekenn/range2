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
        STDEXT_DECLARE_HAS_INNER_TYPE(iterator_category);
        template <typename T, bool Allowed = std::is_object_v<std::remove_pointer_t<T>>>
        struct is_iterator_impl : std::bool_constant<_private::STDEXT_HAS_INNER_TYPE_V(std::iterator_traits<remove_cvref_t<T>>, iterator_category)> { };
        template <typename T> struct is_iterator_impl<T, false> : false_type { };
    }
    template <typename T> struct is_iterator : _private::is_iterator_impl<T> { };

    template <typename T> using iterator_category = typename std::iterator_traits<remove_cvref_t<T>>::iterator_category;

    template <typename T> using iterator_value_type = std::remove_cv_t<typename std::iterator_traits<T>::value_type>;
    template <typename T> using iterator_difference_type = typename std::iterator_traits<T>::difference_type;
    template <typename T> using iterator_pointer_type = typename std::iterator_traits<T>::pointer;
    template <typename T> using iterator_reference_type = typename std::iterator_traits<T>::reference;
    template <typename T> using iterator_size_type = std::make_unsigned_t<iterator_difference_type<T>>;

    template <typename Iterator, bool IsIterator = is_iterator<Iterator>::value>
    struct is_input_iterator : std::is_base_of<std::input_iterator_tag, iterator_category<Iterator>> { };
    template <typename Iterator>
    struct is_input_iterator<Iterator, false> : false_type { };

    template <typename Iterator, bool IsIterator = is_iterator<Iterator>::value>
    struct is_forward_iterator : std::is_base_of<std::forward_iterator_tag, iterator_category<Iterator>> { };
    template <typename Iterator>
    struct is_forward_iterator<Iterator, false> : false_type { };

    template <typename Iterator, bool IsIterator = is_iterator<Iterator>::value>
    struct is_bidirectional_iterator : std::is_base_of<std::bidirectional_iterator_tag, iterator_category<Iterator>> { };
    template <typename Iterator>
    struct is_bidirectional_iterator<Iterator, false> : false_type { };

    template <typename Iterator, bool IsIterator = is_iterator<Iterator>::value>
    struct is_random_access_iterator : std::is_base_of<std::random_access_iterator_tag, iterator_category<Iterator>> { };
    template <typename Iterator>
    struct is_random_access_iterator<Iterator, false> : false_type { };

    template <typename Iterator> constexpr auto is_iterator_v = is_iterator<Iterator>::value;
    template <typename Iterator> constexpr auto is_input_iterator_v = is_input_iterator<Iterator>::value;
    template <typename Iterator> constexpr auto is_forward_iterator_v = is_forward_iterator<Iterator>::value;
    template <typename Iterator> constexpr auto is_bidirectional_iterator_v = is_bidirectional_iterator<Iterator>::value;
    template <typename Iterator> constexpr auto is_random_access_iterator_v = is_random_access_iterator<Iterator>::value;

    namespace _private
    {
        template <typename T> true_type test_std_range(decltype(begin(declval<T>()))*, decltype(end(declval<T>()))*);
        template <typename T> false_type test_std_range(...);
    }

    template <typename T> struct is_std_range : decltype(_private::test_std_range<T>(nullptr, nullptr)) { };
    template <> struct is_std_range<void> : false_type { };
    template <> struct is_std_range<const void> : false_type { };
    template <> struct is_std_range<volatile void> : false_type { };
    template <> struct is_std_range<const volatile void> : false_type { };
    template <typename T> constexpr auto is_std_range_v = is_std_range<T>::value;

    template <typename StdRange> using iterator_type = decltype(begin(declval<StdRange&>()));
    template <typename StdRange> using sentinel_type = decltype(end(declval<StdRange&>()));

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
