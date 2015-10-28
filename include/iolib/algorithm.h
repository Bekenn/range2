//
//  algorithm.h
//  iolib
//
//  Created by James Touton on 10/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_ALGORITHM_INCLUDED
#define IOLIB_ALGORITHM_INCLUDED
#pragma once

#include "range.h"

#include <functional>


namespace iolib
{
    template <class Range, class Function,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto for_each(const Range& range, Function&& f)
    {
        auto pos = range.begin_pos();
        for (auto last = range.end_pos(); pos != last; range.advance_pos(pos))
            f(range.at_pos(pos));
        return pos;
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto find_if(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (auto last = range.end_pos(); pos != last; range.advance_pos(pos))
        {
            if (pred(range.at_pos(pos)))
                break;
        }

        return pos;
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto find_if_not(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (auto last = range.end_pos(); pos != last; range.advance_pos(pos))
        {
            if (!pred(range.at_pos(pos)))
                break;
        }

        return pos;
    }

    template <class Range, class T,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto find(const Range& range, const T& value)
    {
        return find_if(range, [&value](const auto& x) { return x == value; });
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto all_of(const Range& range, Predicate&& pred)
    {
        return find_if_not(range, pred) == range.end_pos();
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto none_of(const Range& range, Predicate&& pred)
    {
        return find_if(range, pred) == range.end_pos();
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    auto any_of(const Range& range, Predicate&& pred)
    {
        return !none_of(range, pred);
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    auto find_first_of(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = range1.begin_pos();
        for (auto last = range1.end_pos(); pos != last; range1.advance_pos(pos))
        {
            if (any_of(range2, [&value = range1.at_pos(pos), &pred](const auto& x) { return pred(value, x); }))
                break;
        }
        return pos;
    }

    template <class Range1, class Range2,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    auto find_first_of(const Range1& range1, const Range2& range2)
    {
        return find_first_of(range1, range2, ::std::equal_to<>());
    }

    template <class Range, class BinaryPredicate,
        REQUIRES(is_multi_pass_range<Range>::value)>
    auto adjacent_find(const Range& range, BinaryPredicate&& pred)
    {
        auto even = range.begin_pos();
        auto last = range.end_pos();
        if (even == last)
            return even;

        position_type<Range> odd;
        while (true)
        {
            if ((odd = next_pos(range, even)) == last)
                return odd;
            if (pred(range.at_pos(even), range.at_pos(odd)))
                return even;
            if ((even = next_pos(range, odd)) == last)
                return even;
            if (pred(range.at_pos(odd), range.at_pos(even)))
                return odd;
        }
    }

    template <class Range,
        REQUIRES(is_multi_pass_range<Range>::value)>
    auto adjacent_find(const Range& range)
    {
        return adjacent_find(range, ::std::equal_to<>());
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_single_pass_range<Range2>::value)>
    auto mismatch(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = ::std::make_pair(range1.begin_pos(), range2.begin_pos());
        for (auto last = ::std::make_pair(range1.end_pos(), range2.end_pos());
             pos.first != last.first && pos.second != last.second;
             void(range1.advance_pos(pos.first)), range2.advance_pos(pos.second))
        {
            if (!pred(range1.at_pos(pos.first), range2.at_pos(pos.second)))
                break;
        }

        return pos;
    }

    template <class Range1, class Range2,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_single_pass_range<Range2>::value)>
    auto mismatch(const Range1& range1, const Range2& range2)
    {
        return mismatch(range1, range2, ::std::equal_to<>());
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    auto search(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto last1 = range1.end_pos();
        auto last2 = range2.end_pos();
        auto range = range1;
        for (auto pos = mismatch(range, range2, pred); pos.first != last1; drop_first(range))
        {
            if (pos.second == last2)
                return range.begin_pos();
        }

        return last1;
    }

    template <class Range1, class Range2,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    auto search(const Range1& range1, const Range2& range2)
    {
        return search(range1, range2, ::std::equal_to<>());
    }

    template <class Range1, class Size, class T,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(::std::is_integral<Size>::value)>
    auto search_n(const Range1, Size count, const T& value)
    {
        // need generator ranges
    }
}

#endif
