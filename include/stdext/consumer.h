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
    template <typename T> struct is_consumer : false_type { };
    template <typename T> constexpr auto is_consumer_v = is_consumer<T>::value;
    template <typename T, typename Elem>
    struct is_consumer<T(Elem)> : std::is_invocable_r<bool, T, Elem> { };

    // I would love to add is_consumer_adaptable, as_consumer, and can_consume, but I can't
    // because there's no way to force is_consumer_adaptable to see make_consumer<Elem> as
    // a dependent function template.  C++20 will do this by default!

    template <typename Iterator>
    class iterator_consumer
    {
    public:
        using iterator = Iterator;

    public:
        iterator_consumer() : i() { }
        explicit iterator_consumer(const iterator& i) : i(i) { }
        explicit iterator_consumer(iterator&& i) : i(stdext::move(i)) { }

    public:
        template <typename T>
        bool operator () (T&& value)
        {
            *i++ = stdext::forward<T>(value);
            return true;
        }

    private:
        iterator i;
    };

    template <typename Iterator, typename Sentinel>
    class delimited_iterator_consumer
    {
    public:
        using iterator = Iterator;
        using sentinel = Sentinel;

    public:
        delimited_iterator_consumer() : i(), j() { }
        delimited_iterator_consumer(const iterator& i, const sentinel& j) : i(i), j(j) { }
        delimited_iterator_consumer(const iterator& i, sentinel&& j) : i(i), j(stdext::move(j)) { }
        delimited_iterator_consumer(iterator&& i, const sentinel& j) : i(stdext::move(i)), j(j) { }
        delimited_iterator_consumer(iterator&& i, sentinel&& j) : i(stdext::move(i)), j(stdext::move(j)) { }

    public:
        template <typename T>
        bool operator () (T&& value)
        {
            if (i == j)
                return false;
            *i++ = stdext::forward<T>(value);
            return true;
        }

    private:
        Iterator i;
        Sentinel j;
    };

    template <typename Elem, typename Iterator,
        STDEXT_REQUIRES(std::is_assignable<decltype(*std::declval<std::decay_t<Iterator>>()), Elem>::value)>
    auto make_consumer(Iterator&& i)
    {
        return iterator_consumer<std::decay_t<Iterator>>(stdext::forward<Iterator>(i));
    }

    template <typename Elem, typename Iterator, typename Sentinel,
        STDEXT_REQUIRES(std::is_assignable<decltype(*std::declval<std::decay_t<Iterator>>()), Elem>::value
            && is_equality_comparable_with<std::decay_t<Iterator>, std::decay_t<Sentinel>>::value)>
    auto make_consumer(Iterator&& i, Sentinel&& j)
    {
        return delimited_iterator_consumer<std::decay_t<Iterator>, std::decay_t<Sentinel>>(stdext::forward<Iterator>(i), stdext::forward<Sentinel>(j));
    }
}

#endif
