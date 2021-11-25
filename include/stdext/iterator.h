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

    namespace detail
    {
        DECLARE_HAS_INNER_TYPE(iterator_category);
    };
    template <class Iterator> struct is_iterator
        : std::conditional_t<detail::HAS_INNER_TYPE(std::iterator_traits<Iterator>, iterator_category),
            true_type,
            false_type>
    { };

    template <class Iterator> using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    namespace detail
    {
        template <class Iterator>
        struct value_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<Iterator>::value_type>;
        };
        template <class Iterator>
        struct difference_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<Iterator>::difference_type>;
        };
        template <class Iterator>
        struct pointer_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<Iterator>::pointer>;
        };
        template <class Iterator>
        struct reference_type_of<Iterator, is_iterator, true>
        {
            using type = std::enable_if_t<is_iterator<Iterator>::value, typename std::iterator_traits<Iterator>::reference>;
        };
        template <class Iterator>
        struct size_type_of<Iterator, is_iterator, true>
        {
            using type = std::make_unsigned_t<difference_type_t<Iterator, is_iterator>>;
        };
    }

    template <class Iterator> using iterator_value_type = value_type<Iterator, is_iterator>;
    template <class Iterator> using iterator_value_type_t = value_type_t<Iterator, is_iterator>;
    template <class Iterator> using iterator_difference_type = difference_type<Iterator, is_iterator>;
    template <class Iterator> using iterator_difference_type_t = difference_type_t<Iterator, is_iterator>;
    template <class Iterator> using iterator_pointer_type = pointer_type<Iterator, is_iterator>;
    template <class Iterator> using iterator_pointer_type_t = pointer_type_t<Iterator, is_iterator>;
    template <class Iterator> using iterator_reference_type = reference_type<Iterator, is_iterator>;
    template <class Iterator> using iterator_reference_type_t = reference_type_t<Iterator, is_iterator>;
    template <class Iterator> using iterator_size_type = size_type<Iterator, is_iterator>;
    template <class Iterator> using iterator_size_type_t = size_type_t<Iterator, is_iterator>;

    template <class Iterator> struct is_input_iterator
    {
        static constexpr bool value = std::is_base_of<std::input_iterator_tag, iterator_category<Iterator>>::value;
    };
    template <class Iterator> struct is_forward_iterator
    {
        static constexpr bool value = std::is_base_of<std::forward_iterator_tag, iterator_category<Iterator>>::value;
    };
    template <class Iterator> struct is_bidirectional_iterator
    {
        static constexpr bool value = std::is_base_of<std::bidirectional_iterator_tag, iterator_category<Iterator>>::value;
    };
    template <class Iterator> struct is_random_access_iterator
    {
        static constexpr bool value = std::is_base_of<std::random_access_iterator_tag, iterator_category<Iterator>>::value;
    };

    template <class Iterator> constexpr auto is_input_iterator_v = is_input_iterator<Iterator>::value;
    template <class Iterator> constexpr auto is_forward_iterator_v = is_forward_iterator<Iterator>::value;
    template <class Iterator> constexpr auto is_bidirectional_iterator_v = is_bidirectional_iterator<Iterator>::value;
    template <class Iterator> constexpr auto is_random_access_iterator_v = is_random_access_iterator<Iterator>::value;

    template <class Iterator>
    class iterator_proxy
    {
    public:
        using value_type = typename std::iterator_traits<Iterator>::value_type;

    public:
        explicit iterator_proxy(const value_type& value) : value(value) { }
        explicit iterator_proxy(value_type&& value) : value(stdext::move(value)) { }

    public:
        value_type operator * () { return stdext::move(value); }

    private:
        value_type value;
    };
}

#endif
