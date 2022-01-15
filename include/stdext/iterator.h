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

    template <typename Iterator> using iterator_category = typename std::iterator_traits<remove_cvref_t<Iterator>>::iterator_category;
    namespace _private
    {
        template <typename Iterator>
        struct value_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<remove_cvref_t<Iterator>>::value_type>;
        };
        template <typename Iterator>
        struct difference_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<remove_cvref_t<Iterator>>::difference_type>;
        };
        template <typename Iterator>
        struct pointer_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<remove_cvref_t<Iterator>>::pointer>;
        };
        template <typename Iterator>
        struct reference_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<remove_cvref_t<Iterator>>::reference>;
        };
        template <typename Iterator>
        struct size_type_of<Iterator, is_iterator, true>
        {
            using type = std::make_unsigned_t<difference_type_t<Iterator, is_iterator>>;
        };
    }

    template <typename Iterator> using iterator_value_type = value_type<Iterator, is_iterator>;
    template <typename Iterator> using iterator_value_type_t = value_type_t<Iterator, is_iterator>;
    template <typename Iterator> using iterator_difference_type = difference_type<Iterator, is_iterator>;
    template <typename Iterator> using iterator_difference_type_t = difference_type_t<Iterator, is_iterator>;
    template <typename Iterator> using iterator_pointer_type = pointer_type<Iterator, is_iterator>;
    template <typename Iterator> using iterator_pointer_type_t = pointer_type_t<Iterator, is_iterator>;
    template <typename Iterator> using iterator_reference_type = reference_type<Iterator, is_iterator>;
    template <typename Iterator> using iterator_reference_type_t = reference_type_t<Iterator, is_iterator>;
    template <typename Iterator> using iterator_size_type = size_type<Iterator, is_iterator>;
    template <typename Iterator> using iterator_size_type_t = size_type_t<Iterator, is_iterator>;

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
