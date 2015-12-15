#ifndef IOLIB_ITERATOR_INCLUDED
#define IOLIB_ITERATOR_INCLUDED
#pragma once

#include "concept.h"

#include <iterator>
#include <utility>


namespace iolib
{
    namespace detail
    {
        DECLARE_HAS_INNER_TYPE(iterator_category);
    };
    template <class Iterator> struct is_iterator
        : ::std::conditional_t<detail::HAS_INNER_TYPE(::std::iterator_traits<Iterator>, iterator_category),
            ::std::true_type,
            ::std::false_type>
    { };

    template <class Iterator> using iterator_category = typename ::std::iterator_traits<Iterator>::iterator_category;
    namespace detail
    {
        template <class Iterator>
        struct value_type_of<Iterator, is_iterator>
        {
            using type = ::std::enable_if_t<is_iterator<Iterator>::value, typename ::std::iterator_traits<Iterator>::value_type>;
        };
        template <class Iterator>
        struct difference_type_of<Iterator, is_iterator>
        {
            using type = ::std::enable_if_t<is_iterator<Iterator>::value, typename ::std::iterator_traits<Iterator>::difference_type>;
        };
        template <class Iterator>
        struct pointer_type_of<Iterator, is_iterator>
        {
            using type = ::std::enable_if_t<is_iterator<Iterator>::value, typename ::std::iterator_traits<Iterator>::pointer>;
        };
        template <class Iterator>
        struct reference_type_of<Iterator, is_iterator>
        {
            using type = ::std::enable_if_t<is_iterator<Iterator>::value, typename ::std::iterator_traits<Iterator>::reference>;
        };
    }

    template <class Iterator> struct is_input_iterator
    {
        static constexpr bool value = ::std::is_base_of<::std::input_iterator_tag, iterator_category<Iterator>>::value;
    };
    template <class Iterator> struct is_forward_iterator
    {
        static constexpr bool value = ::std::is_base_of<::std::forward_iterator_tag, iterator_category<Iterator>>::value;
    };
    template <class Iterator> struct is_bidirectional_iterator
    {
        static constexpr bool value = ::std::is_base_of<::std::bidirectional_iterator_tag, iterator_category<Iterator>>::value;
    };
    template <class Iterator> struct is_random_access_iterator
    {
        static constexpr bool value = ::std::is_base_of<::std::random_access_iterator_tag, iterator_category<Iterator>>::value;
    };

    template <class Iterator>
    class iterator_proxy
    {
    public:
        using value_type = typename ::std::iterator_traits<Iterator>::value_type;

    public:
        explicit iterator_proxy(const value_type& value) : value(value) { }
        explicit iterator_proxy(value_type&& value) : value(::std::move(value)) { }

    public:
        value_type operator * () { return ::std::move(value); }

    private:
        value_type value;
    };
}

#endif
