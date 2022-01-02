//
//  consumer.h
//  stdext
//
//  Created by James Touton on 5/11/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_CONSUMER_INCLUDED
#define STDEXT_CONSUMER_INCLUDED
#pragma once

#include <stdext/iterator.h>


namespace stdext
{
    template <typename T, typename Elem> struct is_consumer : std::is_invocable_r<bool, T, Elem> { };
    template <typename T, typename Elem> constexpr auto is_consumer_v = is_consumer<T, Elem>::value;

    // I would love to add is_consumer_adaptable, as_consumer, and can_consume, but I can't
    // because there's no way to force is_consumer_adaptable to see make_consumer<Elem> as
    // a dependent function template.  C++20 will do this by default!

    template <typename Iterator>
    class iterator_consumer
    {
    public:
        using iterator = Iterator;

    public:
        constexpr iterator_consumer() = default;
        explicit constexpr iterator_consumer(const iterator& i) : _i(i) { }
        explicit constexpr iterator_consumer(iterator&& i) noexcept : _i(stdext::move(i)) { }

    public:
        template <typename T, STDEXT_REQUIRES(std::is_assignable_v<decltype(*declval<iterator&>()), T>)>
        constexpr bool operator () (T&& value) noexcept(std::is_nothrow_assignable_v<decltype(*declval<iterator&>()), T>)
        {
            *_i = stdext::forward<T>(value);
            ++_i;
            return true;
        }

    private:
        iterator _i;
    };

    template <typename Iterator>
    iterator_consumer(Iterator&&) -> iterator_consumer<remove_cvref_t<Iterator>>;

    template <typename Iterator, typename Sentinel = Iterator>
    class delimited_iterator_consumer
    {
        static_assert(is_equality_comparable_with_v<Iterator&, Sentinel&>);

    public:
        using iterator = Iterator;
        using sentinel = Sentinel;

    public:
        constexpr delimited_iterator_consumer() = default;
        explicit constexpr delimited_iterator_consumer(const iterator& i, const sentinel& j) : _i(i), _j(j) { }
        explicit constexpr delimited_iterator_consumer(const iterator& i, sentinel&& j) : _i(i), _j(stdext::move(j)) { }
        explicit constexpr delimited_iterator_consumer(iterator&& i, const sentinel& j) : _i(stdext::move(i)), _j(j) { }
        explicit constexpr delimited_iterator_consumer(iterator&& i, sentinel&& j) noexcept : _i(stdext::move(i)), _j(stdext::move(j)) { }

    public:
        template <typename T, STDEXT_REQUIRES(std::is_assignable_v<decltype(*declval<iterator&>()), T>)>
        constexpr bool operator () (T&& value) noexcept(std::is_nothrow_assignable_v<decltype(*declval<iterator&>()), T>)
        {
            if (_i == _j)
                return false;
            *_i = stdext::forward<T>(value);
            ++_i;
            return true;
        }

    private:
        iterator _i;
        sentinel _j;
    };

    template <typename Iterator, typename Sentinel>
    delimited_iterator_consumer(Iterator&&, Sentinel&&) -> delimited_iterator_consumer<remove_cvref_t<Iterator>, remove_cvref_t<Sentinel>>;

    template <typename Elem, typename Iterator,
        STDEXT_REQUIRES(std::is_assignable_v<decltype(*std::declval<remove_cvref_t<Iterator>>()), Elem>)>
    constexpr auto make_consumer(Iterator&& i)
    {
        return iterator_consumer<remove_cvref_t<Iterator>>(stdext::forward<Iterator>(i));
    }

    template <typename Elem, typename Iterator, typename Sentinel,
        STDEXT_REQUIRES(std::is_assignable_v<decltype(*std::declval<remove_cvref_t<Iterator>>()), Elem>
            && is_equality_comparable_with_v<remove_cvref_t<Iterator>&, remove_cvref_t<Sentinel>&>)>
    constexpr auto make_consumer(Iterator&& i, Sentinel&& j)
    {
        return delimited_iterator_consumer<remove_cvref_t<Iterator>, remove_cvref_t<Sentinel>>(stdext::forward<Iterator>(i), stdext::forward<Sentinel>(j));
    }
}

#endif
