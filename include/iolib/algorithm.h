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
#include "tuple.h"

#include <functional>


namespace iolib
{
    constexpr bool multi_and() noexcept
    {
        return true;
    }
    template <class Bool, class... Bools>
    constexpr bool multi_and(Bool v, Bools... vs) noexcept
    {
        return v && multi_and(vs...);
    }

    constexpr bool multi_or() noexcept
    {
        return false;
    }
    template <class Bool, class... Bools>
    constexpr bool multi_or(Bool v, Bools... vs) noexcept
    {
        return v || multi_or(vs...);
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    position_type<Range, is_range> find_if(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (; !range.is_end_pos(pos); range.advance_pos(pos))
        {
            if (pred(range.at_pos(pos)))
                break;
        }

        return pos;
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    position_type<Range, is_range> find_if_not(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (; !range.is_end_pos(pos); range.advance_pos(pos))
        {
            if (!pred(range.at_pos(pos)))
                break;
        }

        return pos;
    }

    template <class Range, class T,
        REQUIRES(is_single_pass_range<Range>::value)>
    position_type<Range, is_range> find(const Range& range, const T& value)
    {
        return find_if(range, [&value](const auto& x) { return x == value; });
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    bool all_of(const Range& range, Predicate&& pred)
    {
        return find_if_not(range, pred) == range.end_pos();
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    bool none_of(const Range& range, Predicate&& pred)
    {
        return find_if(range, pred) == range.end_pos();
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    bool any_of(const Range& range, Predicate&& pred)
    {
        return !none_of(range, pred);
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    position_type<Range1, is_range> find_first_of(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = range1.begin_pos();
        for (; !range1.is_end_pos(pos); range1.advance_pos(pos))
        {
            if (any_of(range2, [&value = range1.at_pos(pos), &pred](const auto& x) { return pred(value, x); }))
                break;
        }
        return pos;
    }

    template <class Range1, class Range2,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    position_type<Range1, is_range> find_first_of(const Range1& range1, const Range2& range2)
    {
        return find_first_of(range1, range2, ::std::equal_to<>());
    }

    template <class Range, class BinaryPredicate,
        REQUIRES(is_multi_pass_range<Range>::value)>
    position_type<Range, is_range> adjacent_find(const Range& range, BinaryPredicate&& pred)
    {
        auto even = range.begin_pos();
        if (range.is_end_pos(even))
            return even;

        position_type<Range> odd;
        while (true)
        {
            if (range.is_end_pos(odd = next_pos(range, even)))
                return odd;
            if (pred(range.at_pos(even), range.at_pos(odd)))
                return even;
            if (range.is_end_pos(even = next_pos(range, odd)))
                return even;
            if (pred(range.at_pos(odd), range.at_pos(even)))
                return odd;
        }
    }

    template <class Range,
        REQUIRES(is_multi_pass_range<Range>::value)>
    position_type<Range, is_range> adjacent_find(const Range& range)
    {
        return adjacent_find(range, ::std::equal_to<>());
    }

    template <class Function, class... Args>
    void for_each_argument(Function&& func, Args&&... args)
    {
        func(::std::forward<Args>(args))...;
    }

    namespace detail
    {
        template <class... Ranges, ::std::size_t... indices>
        ::std::tuple<::std::tuple<const Ranges&, position_type<Ranges, is_range>&>...>
            make_range_pos_pairs(::std::tuple<const Ranges&...> ranges, ::std::tuple<position_type<Ranges, is_range>...>& positions, type_list<constant<::std::size_t, indices>...>)
        {
            return ::std::make_tuple(::std::make_tuple(::std::ref(::std::get<indices>(ranges)), ::std::ref(::std::get<indices>(positions)))...);
        }
    }
    template <class Function, class... Ranges,
        REQUIRES(const_and<is_single_pass_range<Ranges>::value...>::value)>
    ::std::tuple<position_type<Ranges, is_range>...> for_each(Function&& f, const Ranges&... ranges)
    {
        ::std::tuple<position_type<Ranges, is_range>...> positions(ranges.begin_pos()...);
        auto args = detail::make_range_pos_pairs(::std::make_tuple(::std::ref(ranges)...), positions, iota_list<sizeof...(Ranges), ::std::size_t>());
        apply([&](auto&... rp)
        {
            for (; !multi_or(::std::get<0>(rp).is_end_pos(::std::get<1>(rp))); for_each_argument([](auto& rp) { ::std::get<0>(rp).advance_pos(::std::get<1>(rp)); }, rp...))
                for_each_argument([&](const auto& rp) { f(::std::get<0>(rp).at_pos(::std::get<1>(rp))); });
        }, args);
        return positions;
    }

    template <class Range, class Predicate,
        REQUIRES(is_single_pass_range<Range>::value)>
    size_type<Range, is_range> count_if(const Range& range, Predicate&& pred)
    {
        size_type<Range, is_range> n = 0;
        for_each(range, [&](auto& v)
        {
            if (pred(v))
                ++n;
        });
        return n;
    }

    template <class Range, class T,
        REQUIRES(is_single_pass_range<Range>::value)>
    size_type<Range, is_range> count(const Range& range, const T& value)
    {
        return count_if(range, [&](const auto& v)
        {
            return v == value;
        });
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_single_pass_range<Range2>::value)>
    ::std::pair<position_type<Range1, is_range>, position_type<Range2, is_range>> mismatch(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = ::std::make_pair(range1.begin_pos(), range2.begin_pos());
        for (; !range1.is_end_pos(pos.first) && !range2.is_end_pos(pos.second);
             range1.advance_pos(pos.first), range2.advance_pos(pos.second))
        {
            if (!pred(range1.at_pos(pos.first), range2.at_pos(pos.second)))
                break;
        }

        return pos;
    }

    template <class Range1, class Range2,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_single_pass_range<Range2>::value)>
    ::std::pair<position_type<Range1, is_range>, position_type<Range2, is_range>> mismatch(const Range1& range1, const Range2& range2)
    {
        return mismatch(range1, range2, ::std::equal_to<>());
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_single_pass_range<Range2>::value)>
    bool equal(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = mismatch(range1, range2, ::std::forward<BinaryPredicate>(pred));
        return range1.is_end_pos(pos.first) && range2.is_end_pos(pos.second);
    }

    template <class Range1, class Range2,
        REQUIRES(is_single_pass_range<Range1>::value), REQUIRES(is_single_pass_range<Range2>::value)>
    bool equal(const Range1& range1, const Range2& range2)
    {
        return equal(range1, range2, ::std::equal_to<>())
    }

    namespace detail
    {
        template <class Range1, class Range2, class BinaryPredicate>
        bool is_permutation(const Range1& range1, const Range2& range2, BinaryPredicate&& pred, ::std::false_type counted)
        {
            auto pos = mismatch(range1, range2, pred);
            if (range1.is_end_pos(pos.first))
                return range2.is_end_pos(pos.second);
            if (range2.is_end_pos(pos.second))
                return false;

            auto size1 = size_after(range1, pos.first);
            auto size2 = size_after(range2, pos.second);
            if (size1 != size2)
                return false;

            for (auto pos1 = pos.first; !range1.is_end_pos(pos1); range1.advance_pos(pos1))
            {
                if (any_of(make_range(range1, pos.first, pos1), [&](const auto& value) { return pred(value, range1.at_pos(pos1)); }))
                    continue;

                auto count1 = count_if(make_range(range1, next_pos(range1, pos1), [&](const auto& r, const auto& p) { return range1.is_end_pos(p); }),
                    [&](const auto& value) { return pred(value, range1.at_pos(pos1)); });
                auto count2 = count_if(range2, [&](const auto& value) { return pred(value, range1.at_pos(pos1)); });
                if (count1 != count2)
                    return false;
            }

            return true;
        }

        template <class Range1, class Range2, class BinaryPredicate>
        bool is_permutation(const Range1& range1, const Range2& range2, BinaryPredicate&& pred, ::std::true_type counted)
        {
            if (range1.size() != range2.size())
                return false;
            return is_permutation(range1, range2, ::std::forward<BinaryPredicate>(pred), ::std::false_type());
        }
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    bool is_permutation(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        return detail::is_permutation(range1, range2, ::std::forward<BinaryPredicate>(pred),
            ::std::conditional_t<is_counted_range<Range1>::value && is_counted_range<Range2>::value,
                ::std::true_type,
                ::std::false_type>());
    }

    template <class Range1, class Range2,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    bool is_permutation(const Range1& range1, const Range2& range2)
    {
        return is_permutation(range1, range2, ::std::equal_to<>());
    }

    template <class Range1, class Range2, class BinaryPredicate,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    position_type<Range1, is_range> search(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto range = range1;
        auto pos = mismatch(range, range2, pred);
        for (!range1.is_end_pos(pos.first); drop_first(range))
        {
            if (pos.second == last2)
                return range.begin_pos();
        }

        return pos.first;
    }

    template <class Range1, class Range2,
        REQUIRES(is_multi_pass_range<Range1>::value), REQUIRES(is_multi_pass_range<Range2>::value)>
    position_type<Range1, is_range> search(const Range1& range1, const Range2& range2)
    {
        return search(range1, range2, ::std::equal_to<>());
    }

    template <class Range, class Size, class T, class BinaryPredicate,
        REQUIRES(is_multi_pass_range<Range>::value), REQUIRES(::std::is_integral<Size>::value)>
    position_type<Range, is_range> search_n(const Range& range, Size count, const T& value, BinaryPredicate&& pred)
    {
        auto const_gen = make_constant_generator(value);
        auto const_rng = make_counted_range(const_gen, count);
        auto subrange = make_range(range, range.begin_pos(), [&](const auto& r, const auto& p) { return range.is_end_pos(p); });
        while (true)
        {
            auto pos = mismatch(subrange, const_rng, pred);
            if (range.is_end_pos(pos.first()))
                return pos.first();
            if (const_rng.is_end_pos(pos.second()))
                return subrange.begin_pos();

            drop_first(subrange);
        }
    }

    template <class Range, class Function>
    position_type<Range, is_range> generate(const Range& range, Function&& func)
    {
        return for_each(range, [&](auto& value) { value = func(); });
    }

    template <class Range1, class Range2>
    ::std::pair<position_type<Range1, is_range>, position_type<Range2, is_range>> copy(const Range1& range1, const Range2& range2)
    {
    }
}

#endif
