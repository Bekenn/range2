//
//  algorithm.h
//  stdext
//
//  Created by James Touton on 10/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_ALGORITHM_INCLUDED
#define STDEXT_ALGORITHM_INCLUDED
#pragma once

#include <stdext/range.h>
#include <stdext/tuple.h>

#include <functional>
#include <random>
#include <vector>


namespace stdext
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
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> find_if(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (; !range.is_end_pos(pos); range.inc_pos(pos))
        {
            if (pred(range.at_pos(pos)))
                break;
        }

        return pos;
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> find_if_not(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (; !range.is_end_pos(pos); range.inc_pos(pos))
        {
            if (!pred(range.at_pos(pos)))
                break;
        }

        return pos;
    }

    template <class Range, class Predicate>
    range_position_type_t<Range> find_pos(const Range& range, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        for (; !range.is_end_pos(pos); range.inc_pos(pos))
        {
            if (pred(range, pos))
                break;
        }

        return pos;
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> find(const Range& range, const T& value)
    {
        return find_if(range, [&value](const auto& x) { return x == value; });
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    bool all_of(const Range& range, Predicate&& pred)
    {
        return find_if_not(range, pred) == range.end_pos();
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    bool none_of(const Range& range, Predicate&& pred)
    {
        return find_if(range, pred) == range.end_pos();
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    bool any_of(const Range& range, Predicate&& pred)
    {
        return !none_of(range, pred);
    }

    template <class Range1, class Range2, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    range_position_type_t<Range1> find_first_of(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = range1.begin_pos();
        for (; !range1.is_end_pos(pos); range1.inc_pos(pos))
        {
            if (any_of(range2, [&value = range1.at_pos(pos), &pred](const auto& x) { return pred(value, x); }))
                break;
        }
        return pos;
    }

    template <class Range1, class Range2,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    range_position_type_t<Range1> find_first_of(const Range1& range1, const Range2& range2)
    {
        return find_first_of(range1, range2, std::equal_to<>());
    }

    template <class Range, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> adjacent_find(const Range& range, BinaryPredicate&& pred)
    {
        auto even = range.begin_pos();
        if (range.is_end_pos(even))
            return even;

        range_position_type_t<Range> odd;
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
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> adjacent_find(const Range& range)
    {
        return adjacent_find(range, std::equal_to<>());
    }

    template <class Function, class... Args>
    void for_each_argument(Function&& func, Args&&... args)
    {
        using t = int[sizeof...(Args)];
        t{(func(forward<Args>(args)), 0)...};
    }

    namespace detail
    {
        template <class... Ranges, size_t... indices>
        std::tuple<std::tuple<const Ranges&, range_position_type_t<Ranges>&>...>
            make_range_pos_pairs(std::tuple<const Ranges&...> ranges, std::tuple<range_position_type_t<Ranges>...>& positions, value_list<indices...>)
        {
            return std::make_tuple(std::make_tuple(std::ref(std::get<indices>(ranges)), std::ref(std::get<indices>(positions)))...);
        }
    }
    template <class Function, class... Ranges,
        STDEXT_REQUIRES(const_and<is_multi_pass_range<Ranges>::value...>::value)>
    std::tuple<range_position_type_t<Ranges>...> for_each(Function&& f, const Ranges&... ranges)
    {
        std::tuple<range_position_type_t<Ranges>...> positions(ranges.begin_pos()...);
        auto args = detail::make_range_pos_pairs(std::make_tuple(std::ref(ranges)...), positions, iota_list_t<sizeof...(Ranges), size_t>());
        std::apply([&](auto&... rp)
        {
            for (; !multi_or(std::get<0>(rp).is_end_pos(std::get<1>(rp))...); for_each_argument([](auto& rp) { std::get<0>(rp).inc_pos(std::get<1>(rp)); }, rp...))
                for_each_argument([&](const auto& rp) { f(std::get<0>(rp).at_pos(std::get<1>(rp))); });
        }, args);
        return positions;
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_size_type_t<Range> count_if(const Range& range, Predicate&& pred)
    {
        range_size_type_t<Range> n = 0;
        for_each([&](auto& v)
        {
            if (pred(v))
                ++n;
        }, range);
        return n;
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_size_type_t<Range> count(const Range& range, const T& value)
    {
        return count_if(range, [&](const auto& v)
        {
            return v == value;
        });
    }

    template <class Range1, class Range2, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    std::pair<range_position_type_t<Range1>, range_position_type_t<Range2>>
        mismatch(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = std::make_pair(range1.begin_pos(), range2.begin_pos());
        for (; !range1.is_end_pos(pos.first) && !range2.is_end_pos(pos.second);
             range1.inc_pos(pos.first), range2.inc_pos(pos.second))
        {
            if (!pred(range1.at_pos(pos.first), range2.at_pos(pos.second)))
                break;
        }

        return pos;
    }

    template <class Range1, class Range2,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    std::pair<range_position_type_t<Range1>, range_position_type_t<Range2>>
        mismatch(const Range1& range1, const Range2& range2)
    {
        return mismatch(range1, range2, std::equal_to<>());
    }

    template <class Range1, class Range2, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    bool equal(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto pos = mismatch(range1, range2, forward<BinaryPredicate>(pred));
        return range1.is_end_pos(pos.first) && range2.is_end_pos(pos.second);
    }

    template <class Range1, class Range2,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    bool equal(const Range1& range1, const Range2& range2)
    {
        return equal(range1, range2, std::equal_to<>());
    }

    namespace detail
    {
        template <class Range1, class Range2, class BinaryPredicate>
        bool is_permutation(const Range1& range1, const Range2& range2, BinaryPredicate&& pred, false_type counted)
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

            for (auto pos1 = pos.first; !range1.is_end_pos(pos1); range1.inc_pos(pos1))
            {
                if (any_of(make_range(range1, pos.first, pos1), [&](const auto& r, const auto& p) { return pred(r.at_pos(p), range1.at_pos(pos1)); }))
                    continue;

                auto count1 = count_if(make_range(range1, next_pos(range1, pos1), [](const auto& r, const auto& p) { return r.is_end_pos(p); }),
                    [&](const auto& value) { return pred(value, range1.at_pos(pos1)); });
                auto count2 = count_if(range2, [&](const auto& value) { return pred(value, range1.at_pos(pos1)); });
                if (count1 != count2)
                    return false;
            }

            return true;
        }

        template <class Range1, class Range2, class BinaryPredicate>
        bool is_permutation(const Range1& range1, const Range2& range2, BinaryPredicate&& pred, true_type counted)
        {
            if (range1.size() != range2.size())
                return false;
            return is_permutation(range1, range2, forward<BinaryPredicate>(pred), false_type());
        }
    }

    template <class Range1, class Range2, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    bool is_permutation(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        return detail::is_permutation(range1, range2, forward<BinaryPredicate>(pred),
            std::conditional_t<is_counted_range<Range1>::value && is_counted_range<Range2>::value,
                true_type,
                false_type>());
    }

    template <class Range1, class Range2,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    bool is_permutation(const Range1& range1, const Range2& range2)
    {
        return is_permutation(range1, range2, std::equal_to<>());
    }

    template <class Range1, class Range2, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    range_position_type_t<Range1> search(const Range1& range1, const Range2& range2, BinaryPredicate&& pred)
    {
        auto range = subrange_from(range1, range1.begin_pos());
        auto pos = mismatch(range, range2, pred);
        for (; !range1.is_end_pos(pos.first); drop_first(range))
        {
            if (pos.second == range2.end_pos())
                return range.begin_pos();
        }

        return pos.first;
    }

    template <class Range1, class Range2,
        STDEXT_REQUIRES(is_multi_pass_range<Range1>::value && is_multi_pass_range<Range2>::value)>
    range_position_type_t<Range1> search(const Range1& range1, const Range2& range2)
    {
        return search(range1, range2, std::equal_to<>());
    }

    template <class Range, class Size, class T, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && std::is_integral<Size>::value)>
    range_position_type_t<Range> search_n(const Range& range, Size count, const T& value, BinaryPredicate&& pred)
    {
        auto const_gen = make_constant_generator(value);
        auto const_rng = make_counted_range(const_gen, count);
        auto subrange = make_range(range, range.begin_pos(), [](const auto& r, const auto& p) { return r.is_end_pos(p); });
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

    template <class Range, class OutputRange>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        copy(const Range& range, const OutputRange& result)
    {
        return for_each([](const auto& from, auto& to) { to = from; }, range, result);
    }

    template <class Range, class Size, class OutputRange>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        copy_n(const Range& range, Size n, const OutputRange& result)
    {
        auto counted = make_counted_range(range, n);
        auto pos = copy(counted, result);
        return { counted.base_pos(pos.first), pos.second };
    }

    template <class Range, class OutputRange, class Predicate>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        copy_if(const Range& range, const OutputRange& result, Predicate&& pred)
    {
        auto pos = std::make_pair(range.begin_pos(), result.begin_pos());
        for (; !range.is_end_pos(pos.first) && !result.is_end_pos(pos.second); range.inc_pos(pos.first), result.inc_pos(pos.second))
        {
            if (pred(range.at_pos(pos.first)))
                result.at_pos(pos.second) = range.at_pos(pos.first);
        }
        return pos;
    }

    template <class Range, class OutputRange,
        STDEXT_REQUIRES(is_bidirectional_range<OutputRange>::value && is_delimited_range<OutputRange>::value)>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        copy_backward(const Range& range, const OutputRange& result)
    {
        auto rev = make_reverse_range(result);
        auto pos = copy(range, rev);
        return { pos.first, rev.base_pos(pos.second) };
    }

    template <class Range, class OutputRange>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        move(const Range& range, const OutputRange& result)
    {
        return for_each([](auto& from, auto& to) { to = move(from); }, range, result);
    }

    template <class Range, class OutputRange,
        STDEXT_REQUIRES(is_bidirectional_range<OutputRange>::value && is_delimited_range<OutputRange>::value)>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        move_backward(const Range& range, const OutputRange& result)
    {
        auto rev = make_reverse_range(result);
        auto pos = move(range, rev);
        return { pos.first, rev.base_pos(pos.second) };
    }

    template <class Range1, class Range2>
    std::pair<range_position_type_t<Range1>, range_position_type_t<Range2>>
        swap_ranges(const Range1& range1, const Range2& range2)
    {
        return for_each([](auto& from, auto& to) { swap(from, to); }, range1, range2);
    }

    template <class Range, class OutputRange, class UnaryOperation>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        transform(const Range& range, const OutputRange& result, UnaryOperation&& op)
    {
        return for_each([&](const auto& from, auto& to) { to = op(from); }, range, result);
    }

    template <class Range1, class Range2, class OutputRange, class BinaryOperation>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        transform(const Range1& range1, const Range2& range2, const OutputRange& result, BinaryOperation&& op)
    {
        return for_each([&](const auto& from1, const auto& from2, auto& to) { to = op(from1, from2); }, range1, range2, result);
    }

    template <class Range, class Predicate, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> replace_if(const Range& range, Predicate&& pred, const T& new_value)
    {
        return std::get<0>(for_each([&](auto& value) { if (pred(value)) value = new_value; }, range));
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> replace(const Range& range, const T& old_value, const T& new_value)
    {
        return replace_if(range, [&](const auto& value) { return value == old_value; }, new_value);
    }

    template <class Range, class OutputRange, class Predicate, class T>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        replace_copy_if(const Range& range, const OutputRange& result, Predicate&& pred, const T& new_value)
    {
        return for_each([&](const auto& from, auto& to) { to = pred(from) ? new_value : from; }, range, result);
    }

    template <class Range, class OutputRange, class T>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        replace_copy(const Range& range, const OutputRange& result, const T& old_value, const T& new_value)
    {
        return replace_copy_if(range, result, [&](const auto& value) { return value == old_value; }, new_value);
    }

    template <class Range, class T>
    range_position_type_t<Range> fill(const Range& range, const T& value)
    {
        return std::get<0>(for_each([&](auto& v) { v = value; }));
    }

    template <class Range, class Size, class T>
    range_position_type_t<Range> fill_n(const Range& range, Size n, const T& value)
    {
        auto counted = make_counted_range(range, n);
        auto pos = fill(counted, value);
        return counted.base_pos(pos);
    }

    template <class Range, class Function>
    range_position_type_t<Range> generate(const Range& range, Function&& func)
    {
        return std::get<0>(for_each([&](auto& value) { value = func(); }, range));
    }

    template <class Range, class Size, class Function>
    range_position_type_t<Range> generate_n(const Range& range, Size n, Function&& func)
    {
        auto counted = make_counted_range(range, n);
        auto pos = generate(counted, forward<Function>(func));
        return counted.base_pos(pos);
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> remove_if(const Range& range, Predicate&& pred)
    {
        auto pos = find_if(range, pred);
        auto rng = make_range(range, pos, [](const auto& r, const auto& p) { return r.is_end_pos(p); });
        for_each([&](auto& value)
        {
            if (!pred(value))
            {
                rng.at_pos(pos) = move(value);
                rng.inc_pos(pos);
            }
        }, rng);

        return pos;
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> remove(const Range& range, const T& value)
    {
        return remove_if(range, [&](const auto& v) { return v == value; });
    }

    template <class Range, class OutputRange, class Predicate>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        remove_copy_if(const Range& range, const OutputRange& result, Predicate&& pred)
    {
        return copy_if(range, result, [&](const auto& v) { return !pred(v); });
    }

    template <class Range, class OutputRange, class T>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        remove_copy(const Range& range, const OutputRange& result, const T& value)
    {
        return remove_copy_if(range, result, [&](const auto& v) { return v == value; });
    }

    template <class Range, class BinaryPredicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> unique(const Range& range, BinaryPredicate&& pred)
    {
        auto read_pos = range.begin_pos();
        if (range.is_end_pos(read_pos))
            return read_pos;

        range.inc_pos(read_pos);
        auto write_pos = find_if(range, [&](const auto& value)
        {
            if (range.is_end_pos(read_pos) || pred(value, range.at_pos(read_pos)))
                return true;
            inc_pos(read_pos);
            return false;
        });

        range.inc_pos(write_pos);
        if (range.is_end_pos(read_pos))
            return write_pos;

        range.inc_pos(read_pos);
        while (!range.is_end_pos(read_pos))
        {
            range.at_pos(write_pos) = move(range.at_pos(read_pos));
            range.inc_pos(read_pos);
            range.inc_pos(write_pos);
            read_pos = find_if(subrange_from(read_pos), [&](const auto& value) { return !pred(value, range.at_pos(write_pos)); });
        }

        return write_pos;
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> unique(const Range& range)
    {
        return unique(range, std::equal_to<>());
    }

    template <class Range, class OutputRange, class BinaryPredicate>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        unique_copy(const Range& range, const OutputRange& result, BinaryPredicate&& pred)
    {
        auto from = range.begin_pos();
        if (range.is_end_pos(from))
            return std::make_pair(from, result.begin_pos());

        auto to = for_each([&](auto& out)
        {
            out = range.at_pos(from);
            from = find_if(next_pos(range, from), [&](const auto& value) { return !pred(value, range.at_pos(from)); });
        }, result);
        return std::make_pair(from, to);
    }

    template <class Range, class OutputRange>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        unique_copy(const Range& range, const OutputRange& result)
    {
        return unique_copy(range, result, std::equal_to<>());
    }

    template <class Range,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    void reverse(Range range)
    {
        auto l = range.begin_pos();
        if (range.is_end_pos(l))
            return;

        for (auto r = range.end_pos(); range.dec_pos(r) != l; )
        {
            swap(range.at_pos(l), range.at_pos(r));
            if (range.inc_pos(l) == r)
                return;
        }
    }

    template <class Range, class OutputRange,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        reverse_copy(const Range& range, const OutputRange& result)
    {
        auto rng = make_reverse_range(range);
        auto pos = copy(rng, result);
        return std::make_pair(rng.base_pos(pos.first), pos.second);
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> rotate(const Range& range, range_position_type_t<Range> middle)
    {
        if (middle == range.begin_pos() || range.is_end_pos(middle))
            return range.begin_pos();

        auto ranges = split_range(range, middle);
        auto pos = for_each([](auto& a, auto& b) { swap(a, b); }, ranges.first, ranges.second);

        if (ranges.first.is_end_pos(std::get<0>(pos)))
            return rotate(ranges.second, std::get<1>(pos));

        rotate(subrange_from(range, std::get<0>(pos)), middle);
        return std::get<0>(pos);
    }

    template <class Range, class OutputRange,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    std::pair<range_position_type_t<Range>, range_position_type_t<OutputRange>>
        rotate_copy(const Range& range, range_position_type_t<Range> middle, const OutputRange& result)
    {
        auto pos = for_each([](const auto& from, auto& to) { to = from; }, subrange_from(range, middle), result);
        return for_each([](const auto& from, auto& to) { to = from; }, subrange_to(range, middle), subrange_from(result, pos.second));
    }

    template <class Range, class UniformRandomNumberGenerator,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void shuffle(const Range& range, UniformRandomNumberGenerator&& g)
    {
        if (is_empty(range))
            return;

        std::uniform_int_distribution<range_size_type_t<Range>> dist;

        auto size = range.size();
        for (range_size_type_t<Range> n = 0; n < size - 1; ++n)
        {
            dist.param({ n, size - 1 });
            auto index = dist(g);
            if (index != n)
                swap(range[n], range[index]);
        }
    }

    template <class Range, class Predicate>
    bool is_partitioned(const Range& range, Predicate&& pred)
    {
        auto pos = find_if_not(range, pred);
        pos = find_if(subrange_from(range, pos), pred);
        return range.is_end_pos(pos);
    }

    namespace detail
    {
        template <class Range, class Predicate>
        range_position_type_t<Range> partition(const Range& range, Predicate&& pred, multi_pass_range_tag)
        {
            auto p1 = find_if_not(range, pred);
            if (range.is_end_pos(p1))
                return p1;

            while (true)
            {
                auto p2 = find_if(subrange_from(range, next_pos(range, p1)), pred);
                if (range.is_end_pos(p2))
                    return p1;

                swap(range.at_pos(p1), range.at_pos(p2));
                range.inc_pos(p1);
            }
        }

        template <class Range, class Predicate,
            STDEXT_REQUIRES(is_delimited_range<Range>::value)>
        range_position_type_t<Range> partition(const Range& range, Predicate&& pred, bidirectional_range_tag)
        {
            auto p1 = find_if_not(range, pred);
            if (range.is_end_pos(p1))
                return p1;

            auto rev = make_reverse_range(range);
            auto p2 = find_if(rev, pred);
            if (rev.is_end_pos(p2))
                return p1;

            while (p1 != rev.base_pos(p2))
            {
                swap(range.at_pos(p1), rev.at_pos(p2));
                p1 = find_if_not(subrange_from(range, next_pos(range, p1)), pred);
                p2 = find_if(subrange_from(rev, next_pos(rev, p2)), pred);
            }

            return p1;
        }
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> partition(const Range& range, Predicate&& pred)
    {
        return detail::partition(range, pred, range_category<Range>());
    }

    namespace detail
    {
        template <class Range, class Predicate>
        range_position_type_t<Range> fast_stable_partition(const Range& range, Predicate&& pred, std::vector<range_value_type_t<Range>>& buf)
        {
            auto l = find_if_not(range, pred);
            if (range.is_end_pos(l))
                return l;
            auto r = next_pos(range, l);

            while (true)
            {
                auto mid = find_if(subrange_from(range, r), pred);
                if (range.is_end_pos(mid))
                    return l;

                r = find_if_not(subrange_from(range, next_pos(range, mid)), pred);

                for (auto pos = l; pos != mid; range.inc_pos(pos))
                    buf.emplace_back(move(range.at_pos(pos)));

                l = move(make_range(range, mid, r), make_range(range, l, r)).second;
                move(make_range(buf.begin(), buf.end()), make_range(range, l, r));
                buf.resize(0);
            }
        }

        template <class Range, class Predicate>
        range_position_type_t<Range> slow_stable_partition(const Range& range, Predicate&& pred)
        {
            auto l = find_if_not(range, pred);
            if (range.is_end_pos(l))
                return l;

            auto r = next_pos(range, l);

            while (true)
            {
                auto mid = find_if(subrange_from(range, r), pred);
                if (range.is_end_pos(mid))
                    return l;

                r = find_if_not(subrange_from(range, next_pos(range, mid)), pred);
                l = rotate(make_range(range, l, r), mid);
            }
        }

        template <class Range, class Predicate>
        range_position_type_t<Range> stable_partition(const Range& range, Predicate&& pred, false_type /* is_counted */)
        {
            return slow_stable_partition(range, forward<Predicate>(pred));
        }

        template <class Range, class Predicate>
        range_position_type_t<Range> stable_partition(const Range& range, Predicate&& pred, true_type /* is_counted */)
        {
            std::vector<range_value_type_t<Range>> buf;
            try
            {
                buf.reserve(range.size());
            }
            catch (const std::bad_alloc&)
            {
                return slow_stable_partition(range, pred);
            }

            return fast_stable_partition(range, pred, buf);
        }
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    range_position_type_t<Range> stable_partition(const Range& range, Predicate&& pred)
    {
        return detail::stable_partition(range, forward<Predicate>(pred), is_counted_range<Range>());
    }

    template <class Range, class OutputRange1, class OutputRange2, class Predicate>
    std::tuple<range_position_type_t<Range>, range_position_type_t<OutputRange1>, range_position_type_t<OutputRange2>>
        partition_copy(const Range& range, const OutputRange1& out_true, const OutputRange2& out_false, Predicate&& pred)
    {
        auto pos = range.begin_pos();
        auto out1 = out_true.begin_pos();
        auto out2 = out_false.begin_pos();
        for (; !range.is_end_pos(pos); range.inc_pos(pos))
        {
            if (pred(range.at_pos(pos)))
            {
                if (out_true.is_end_pos(out1))
                    return make_tuple(pos, out1, out2);
                out_true.at_pos(out1) = range.at_pos(pos);
                out_true.inc_pos(out1);
            }
            else
            {
                if (out_false.is_end_pos(out2))
                    return make_tuple(pos, out1, out2);
                out_false.at_pos(out2) = range.at_pos(pos);
                out_false.inc_pos(out2);
            }
        }

        return make_tuple(pos, out1, out2);
    }

    namespace detail
    {
        template <class Range, class Predicate>
        range_position_type_t<Range> partition_point(const Range& range, Predicate&& pred, random_access_range_tag)
        {
            auto l = range_size_type_t<Range>(0);
            auto r = range.size();
            while (l != r)
            {
                auto n = l + (r - l) / 2;
                if (pred(range[n]))
                    l = n + 1;
                else
                    r = n;
            }

            auto pos = range.begin_pos();
            return range.advance_pos(pos, r);
        }

        template <class Range, class Predicate>
        range_position_type_t<Range> partition_point(const Range& range, Predicate&& pred, true_type /* is_counted */)
        {
            auto size = range.size();
            auto l = std::make_pair(range.begin_pos(), range_size_type_t<Range>(0));
            auto r = std::make_pair(range.begin_pos(), size);
            advance_pos(range, r.first, size);

            while (l.second != r.second)
            {
                auto n = (r.second - l.second) / 2;
                auto pos = l;
                advance_pos(range, pos.first, n);
                pos.second += n;
                if (pred(range.at_pos(pos.first)))
                {
                    l = pos;
                    range.inc_pos(l.first);
                    ++l.second;
                }
                else
                    r = pos;
            }

            return l.first;
        }

        template <class Range, class Predicate>
        range_position_type_t<Range> partition_point(const Range& range, Predicate&& pred, false_type /* is_counted */)
        {
            auto size = range_size_type_t<Range>(distance(range, range.begin_pos(), range.end_pos()));
            auto counted = make_counted_range(range, range.begin_pos(), size);
            auto pos = partition_point(counted, pred, true_type());
            return counted.base_pos(pos);
        }

        template <class Range, class Predicate>
        range_position_type_t<Range> partition_point(const Range& range, Predicate&& pred, multi_pass_range_tag)
        {
            return partition_point(range, pred, is_counted_range<Range>());
        }
    }

    template <class Range, class Predicate,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    range_position_type_t<Range> partition_point(const Range& range, Predicate&& pred)
    {
        return detail::partition_point(range, pred, range_category<Range>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void push_heap(const Range& range, Compare&& comp)
    {
        auto size = range.size();
        if (size <= 1)
            return;

        auto index = size - 1;
        while (index != 0)
        {
            auto parent = (index - 1) / 2;
            if (!comp(range[parent], range[index]))
                break;

            swap(range[parent], range[index]);
            index = parent;
        }
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void push_heap(const Range& range)
    {
        return push_heap(range, std::less<>());
    }

    namespace detail
    {
        template <class Range, class Compare>
        void heap_rebalance_root(const Range& range, Compare&& comp)
        {
            auto size = range.size();
            if (size <= 1)
                return;

            auto index = range_size_type_t<Range>(0);
            while (true)
            {
                auto child = 2 * index + 1;
                if (child >= size)
                    break;

                if (comp(range[index], range[child]))
                {
                    swap(range[index], range[child]);
                    index = child;
                    continue;
                }

                if (++child == size)
                    break;

                if (!comp(range[index], range[child]))
                    break;

                swap(range[index], range[child]);
                index = child;
            }
        }
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void pop_heap(const Range& range, Compare&& comp)
    {
        if (range.size() <= 1)
            return;

        auto first = range.begin_pos();
        auto last = range.end_pos();
        range.dec_pos(last);

        swap(range.at_pos(first), range.at_pos(last));
        detail::heap_rebalance_root(subrange_to(range, last), comp);
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void pop_heap(const Range& range)
    {
        pop_heap(range, std::less<>());
    }

    template <class Range, class T, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void emplace_heap(const Range& range, T&& value, Compare&& comp)
    {
        range[0] = forward<T>(value);
        detail::heap_rebalance_root(range, comp);
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void emplace_heap(const Range& range, T&& value)
    {
        emplace_heap(range, forward<T>(value), std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void make_heap(const Range& range, Compare&& comp)
    {
        auto size = range.size();
        if (size <= 1)
            return;

        for (auto index = range_size_type_t<Range>(0); ; ++index)
        {
            auto left = 2 * index + 1;
            if (left >= size)
                break;
            auto right = left + 1;

            if (comp(range[index], range[left]))
            {
                if (right < size && comp(range[left], range[right]))
                    swap(range[index], range[right]);
                else
                    swap(range[index], range[left]);
                continue;
            }

            if (right >= size)
                break;

            if (comp(range[index], range[right]))
                swap(range[index], range[right]);
        }
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void make_heap(const Range& range)
    {
        make_heap(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void sort_heap(const Range& range, Compare&& comp)
    {
        if (range.size() <= 1)
            return;

        auto subrange = subrange_from(range, range.begin_pos());
        while (subrange.size() > 1)
        {
            pop_heap(subrange, comp);
            drop_last(subrange);
        }
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void sort_heap(const Range& range)
    {
        sort_heap(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    range_position_type_t<Range> is_heap_until(const Range& range, Compare&& comp)
    {
        auto size = range.size();
        if (size <= 1)
            return range.end_pos();

        for (auto index = range_size_type_t<Range>(1); index < size; ++index)
        {
            auto parent = (index - 1) / 2;
            if (comp(range[parent], range[index]))
            {
                auto pos = range.begin_pos();
                range.advance_pos(pos, index);
                return pos;
            }
        }

        return range.end_pos();
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    range_position_type_t<Range> is_heap_until(const Range& range)
    {
        return is_heap_until(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    bool is_heap(const Range& range, Compare&& comp)
    {
        return range.is_end_pos(is_heap_until(range, comp));
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    bool is_heap(const Range& range)
    {
        return range.is_end_pos(is_heap_until(range, std::less<>()));
    }

    namespace detail
    {
        template <class Range, class Compare>
        range_position_type_t<Range> choose_pivot(const Range& range, Compare&& comp)
        {
            if (range.size() == 0)
                return range.end_pos();

            if (range.size() <= 8)
            {
                auto p = range.begin_pos();
                range.advance_pos(p, range.size() / 2);
                return p;
            }

            auto delta = range.size() / 4;

            auto p1 = range.begin_pos();
            range.advance_pos(p1, delta);
            auto p2 = range.begin_pos();
            range.advance_pos(p2, 2 * delta);
            auto p3 = range.begin_pos();
            range.advance_pos(p3, 3 * delta);

            if (comp(range.at_pos(p2), range.at_pos(p1)))
                swap(p1, p2);
            if (comp(range.at_pos(p3), range.at_pos(p1)))
                swap(p1, p3);
            return comp(range.at_pos(p2), range.at_pos(p3)) ? p2 : p3;
        }

        template <class Range, class Compare>
        range_position_type_t<Range> pivot_partition(const Range& range, Compare&& comp)
        {
            auto pivot = detail::choose_pivot(range, comp);
            if (range.is_end_pos(pivot))
                return pivot;

            auto back_pos = prev_pos(range, range.end_pos());
            if (pivot != back_pos)
            {
                swap(range.at_pos(pivot), range.at_pos(back_pos));
                pivot = back_pos;
            }

            pivot = partition(subrange_to(range, pivot),
                [&](const auto& value)
            {
                return comp(value, range.at_pos(pivot));
            });
            if (pivot != back_pos)
                swap(range.at_pos(back_pos), range.at_pos(pivot));

            return pivot;
        }
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void nth_element(const Range& range, range_position_type_t<Range> nth, Compare&& comp)
    {
        if (range.is_end_pos(nth))
            return;

        auto pivot = detail::pivot_partition(range, comp);
        auto dist = range.distance(pivot, nth);
        if (dist < 0)
            nth_element(subrange_to(range, pivot), nth, comp);
        else if (dist > 0)
            nth_element(subrange_from(range, next_pos(range, pivot)), nth, comp);
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void nth_element(const Range& range, range_position_type_t<Range> nth)
    {
        nth_element(range, nth, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void sort(const Range& range, Compare&& comp)
    {
        if (range.size() <= 1)
            return;

        auto pivot = detail::pivot_partition(range, comp);
        if (range.is_end_pos(pivot))
            return;

        sort(subrange_to(range, pivot), comp);
        sort(subrange_from(range, next_pos(range, pivot)), comp);
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void sort(const Range& range)
    {
        sort(range, std::less<>());
    }

    namespace detail
    {
        template <class Range, class Compare>
        void fast_stable_sort(const Range& range, Compare&& comp, std::vector<range_value_type_t<Range>>& buf)
        {
            if (range.size() <= 1)
                return;

            auto l = range.begin_pos();
            auto r = next_pos(range, l);
            auto pred = [&](const auto& value)
            {
                return comp(value, range.at_pos(l));
            };

            while (true)
            {
                auto mid = find_if(subrange_from(range, r), pred);
                if (range.is_end_pos(mid))
                    break;

                r = find_if_not(subrange_from(range, next_pos(range, mid)), pred);
                for (auto pos = l; pos != mid; range.inc_pos(pos))
                    buf.emplace_back(move(range.at_pos(pos)));
                l = move(make_range(range, mid, r), make_range(range, l, r)).second;
                move(make_range(buf.begin(), buf.end()), make_range(range, l, r));
                buf.resize(0);
            }

            fast_stable_sort(subrange_to(range, l), comp, buf);
            fast_stable_sort(subrange_from(range, next_pos(range, l)), comp, buf);
        }

        template <class Range, class Compare>
        void slow_stable_sort(const Range& range, Compare&& comp)
        {
            if (range.size() <= 1)
                return;

            auto l = range.begin_pos();
            auto r = next_pos(range, l);
            auto pred = [&](const auto& value)
            {
                return comp(value, range.at_pos(l));
            };

            while (true)
            {
                auto mid = find_if(subrange_from(range, r), pred);
                if (range.is_end_pos(mid))
                    break;
                r = find_if_not(subrange_from(range, next_pos(range, mid)), pred);
                l = rotate(make_range(range, l, r), mid);
            }

            stable_sort(subrange_to(range, l), comp);
            stable_sort(subrange_from(range, next_pos(range, l)), comp);
        }
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void stable_sort(const Range& range, Compare&& comp)
    {
        std::vector<range_value_type_t<Range>> buf;
        try
        {
            buf.reserve(range.size());
        }
        catch (const std::bad_alloc&)
        {
            detail::slow_stable_sort(range, comp);
            return;
        }

        detail::fast_stable_sort(range, comp, buf);
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_counted_range<Range>::value)>
    void stable_sort(const Range& range)
    {
        stable_sort(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_delimited_range<Range>::value)>
    void partial_sort(const Range& range, range_position_type_t<Range> middle, Compare&& comp)
    {
        nth_element(range, middle, comp);
        sort(subrange_to(range, middle), comp);
    }

    template <class Range,
        STDEXT_REQUIRES(is_random_access_range<Range>::value && is_delimited_range<Range>::value)>
    void partial_sort(const Range& range, range_position_type_t<Range> middle)
    {
        partial_sort(range, middle, std::less<>());
    }

    template <class Range, class OutputRange, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && is_random_access_range<OutputRange>::value && is_counted_range<OutputRange>::value)>
    range_position_type_t<OutputRange> partial_sort_copy(const Range& range, const OutputRange& out, Compare&& comp)
    {
        if (out.size() == 0)
            return out.end_pos();

        auto in_pos = range.begin_pos();
        auto heap = subrange_to(out, out.begin_pos());
        while (!range.is_end_pos(in_pos) && !out.is_end_pos(heap.end_pos()))
        {
            auto last = heap.end_pos();
            heap.at_pos(last) = range.at_pos(in_pos);
            range.inc_pos(in_pos);
            heap.inc_pos(last);
            heap.end_pos(last);

            push_heap(heap, comp);
        }

        while (!range.is_end_pos(in_pos))
        {
            if (comp(out[0], range.at_pos(in_pos)))
                emplace_heap(out, range.at_pos(in_pos), comp);
            range.inc_pos(in_pos);
        }

        sort_heap(heap, comp);
        return heap.end_pos();
    }

    template <class Range, class OutputRange,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && is_random_access_range<OutputRange>::value && is_counted_range<OutputRange>::value)>
    range_position_type_t<OutputRange> partial_sort_copy(const Range& range, const OutputRange& out)
    {
        return partial_sort_copy(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> is_sorted_until(const Range& range, Compare&& comp)
    {
        auto current = range.begin_pos();
        if (range.is_end_pos(current))
            return current;

        auto next = next_pos(range, current);
        while (!range.is_end_pos(next))
        {
            if (comp(range.at_pos(next), range.at_pos(current)))
                return next;

            current = next;
            range.inc_pos(next);
        }

        return next;
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> is_sorted_until(const Range& range)
    {
        return is_sorted_until(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    bool is_sorted(const Range& range, Compare&& comp)
    {
        return range.is_end_pos(is_sorted_until(range, comp));
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    bool is_sorted(const Range& range)
    {
        return range.is_end_pos(is_sorted_until(range, std::less<>()));
    }

    template <class Range, class T, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    range_position_type_t<Range> lower_bound(const Range& range, const T& value, Compare&& comp)
    {
        return partition_point(range, [&](const auto& v) { return comp(v, value); });
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
        range_position_type_t<Range> lower_bound(const Range& range, const T& value)
    {
        return partition_point(range, [&](const auto& v) { return v < value; });
    }

    template <class Range, class T, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    range_position_type_t<Range> upper_bound(const Range& range, const T& value, Compare&& comp)
    {
        return partition_point(range, [&](const auto& v) { return !comp(v, value); });
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    range_position_type_t<Range> upper_bound(const Range& range, const T& value)
    {
        return partition_point(range, [&](const auto& v) { return !(v < value); });
    }

    template <class Range, class T, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    delimited_range<Range> equal_range(const Range& range, const T& value, Compare&& comp)
    {
        auto l = lower_bound(range, value, comp);
        auto remainder = subrange_from(range, l);
        return make_range(range, l, upper_bound(remainder, value, comp));
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    delimited_range<Range> equal_range(const Range& range, const T& value)
    {
        return equal_range(range, value, std::less<>());
    }

    template <class Range, class T, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    bool binary_search(const Range& range, const T& value, Compare&& comp)
    {
        auto pos = lower_bound(range, value, comp);
        if (range.is_end_pos(pos))
            return false;

        return !comp(value, range.at_pos(pos));
    }

    template <class Range, class T,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value && (is_counted_range<Range>::value || is_delimited_range<Range>::value))>
    bool binary_search(const Range& range, const T& value)
    {
        return binary_search(range, value, std::less<>());
    }

    template <class Range1, class Range2, class OutputRange, class Compare>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        merge(const Range1& range1, const Range2& range2, const OutputRange& result, Compare&& comp)
    {
        auto pos = std::make_tuple(range1.begin_pos(), range2.begin_pos(), result.begin_pos());
        while (!range1.is_end_pos(std::get<0>(pos)) && !range2.is_end_pos(std::get<1>(pos)) && !result.is_end_pos(std::get<2>(pos)))
        {
            if (comp(range2.at_pos(std::get<1>(pos)), range1.at_pos(std::get<0>(pos))))
                write(result, std::get<2>(pos), read(range2, std::get<1>(pos)));
            else
                write(result, std::get<2>(pos), read(range1, std::get<0>(pos)));
        }

        if (!result.is_end_pos(std::get<2>(pos)))
        {
            if (!range1.is_end_pos(std::get<0>(pos)))
            {
                auto pos2 = copy(subrange_from(range1, std::get<0>(pos)), subrange_from(result, std::get<2>(pos)));
                std::get<0>(pos) = pos2.first;
                std::get<2>(pos) = pos2.second;
            }
            else if (!range2.is_end_pos(std::get<1>(pos)))
            {
                auto pos2 = copy(subrange_from(range2, std::get<1>(pos)), subrange_from(result, std::get<2>(pos)));
                std::get<1>(pos) = pos2.first;
                std::get<2>(pos) = pos2.second;
            }
        }

        return pos;
    }

    template <class Range1, class Range2, class OutputRange>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        merge(const Range1& range1, const Range2& range2, const OutputRange& result)
    {
        return merge(range1, range2, result, std::less<>());
    }

    namespace detail
    {
        template <class Range, class Compare>
        void fast_inplace_merge(const Range& range, range_position_type_t<Range> middle, Compare&& comp, std::vector<range_value_type_t<Range>>& buf)
        {
            auto l = range.begin_pos();
            while (!range.is_end_pos(middle))
            {
                l = lower_bound(make_range(range, l, middle), [&](const auto& r, const auto& p) { return comp(range.at_pos(middle), r.at_pos(p)); });
                if (l == middle)
                    return;

                auto r = lower_bound(subrange_from(range, middle), [&](const auto& value) { return comp(range.at_pos(l), value); });
                for_each([&](auto& value) { buf.emplace_back(move(value)); }, make_range(range, l, middle));
                middle = move(make_range(range, middle, r), make_range(range, l, r)).second;
                move(make_range(buf.begin(), buf.end()), make_range(range, middle, r));
                buf.resize(0);
                l = middle;
                middle = r;
            }
        }

        template <class Range, class Compare>
        void slow_inplace_merge(const Range& range, range_position_type_t<Range> middle, Compare&& comp)
        {
            auto l = range.begin_pos();
            while (!range.is_end_pos(middle))
            {
                l = lower_bound(make_range(range, l, middle), [&](const auto& r, const auto& p) { return comp(range.at_pos(middle), r.at_pos(p)); });
                if (l == middle)
                    return;

                auto r = lower_bound(subrange_from(range, middle), [&](const auto& value) { return comp(range.at_pos(l), value); });
                l = rotate(make_range(range, l, r), middle);
                middle = r;
            }
        }

        template <class Range, class Compare>
        void inplace_merge(const Range& range, range_position_type_t<Range> middle, Compare&& comp, false_type /* is_counted */)
        {
            slow_inplace_merge(range, middle, comp);
        }

        template <class Range, class Compare>
        void inplace_merge(const Range& range, range_position_type_t<Range> middle, Compare&& comp, true_type /* is_counted */)
        {
            std::vector<range_value_type_t<Range>> buf;
            try
            {
                buf.reserve(range.size());
            }
            catch (const std::bad_alloc&)
            {
                slow_inplace_merge(range, middle, comp);
                return;
            }

            fast_inplace_merge(range, middle, comp, buf);
        }
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value)>
    void inplace_merge(const Range& range, range_position_type_t<Range> middle, Compare&& comp)
    {
        detail::inplace_merge(range, middle, comp, is_counted_range<Range>());
    }

    template <class Range,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value)>
    void inplace_merge(const Range& range, range_position_type_t<Range> middle)
    {
        detail::inplace_merge(range, middle, std::less<>(), is_counted_range<Range>());
    }

    template <class Range1, class Range2, class Compare>
    bool includes(const Range1& range1, const Range2& range2, Compare&& comp)
    {
        auto pos1 = range1.begin_pos();
        auto pos2 = find_if(range2, [&](const auto& v2)
        {
            pos1 = find_if(subrange_from(range1, pos1), [&](const auto& v1) { return !comp(v1, v2); });
            return range1.is_end_pos(pos1) || comp(v2, range1.at_pos(pos1));
        });
        return !range2.is_end_pos(pos2);
    }

    template <class Range1, class Range2>
    bool includes(const Range1& range1, const Range2& range2)
    {
        return includes(range1, range2, std::less<>());
    }

    template <class Range1, class Range2, class OutputRange, class Compare>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_union(const Range1& range1, const Range2& range2, const OutputRange& result, Compare&& comp)
    {
        auto pos = std::make_tuple(range1.begin_pos(), range2.begin_pos(), result.begin_pos());
        while (!range1.is_end_pos(std::get<0>(pos)) && !range2.is_end_pos(std::get<1>(pos)) && !result.is_end_pos(std::get<2>(pos)))
        {
            if (comp(range1.at_pos(std::get<0>(pos)), range2.at_pos(std::get<1>(pos))))
                write(result, std::get<2>(pos), read(range1, std::get<0>(pos)));
            else if (comp(range2.at_pos(std::get<1>(pos)), range1.at_pos(std::get<0>(pos))))
                write(result, std::get<2>(pos), read(range2, std::get<1>(pos)));
            else
            {
                write(result, std::get<2>(pos), read(range1, std::get<0>(pos)));
                range2.inc_pos(std::get<1>(pos));
            }
        }

        std::tie(std::get<0>(pos), std::get<2>(pos)) = copy(subrange_from(range1, std::get<0>(pos)), subrange_from(result, std::get<2>(pos)));
        std::tie(std::get<1>(pos), std::get<2>(pos)) = copy(subrange_from(range2, std::get<1>(pos)), subrange_from(result, std::get<2>(pos)));
        return pos;
    }

    template <class Range1, class Range2, class OutputRange>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_union(const Range1& range1, const Range2& range2, const OutputRange& result)
    {
        return set_union(range1, range2, result, std::less<>());
    }

    template <class Range1, class Range2, class OutputRange, class Compare>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_intersection(const Range1& range1, const Range2& range2, const OutputRange& result, Compare&& comp)
    {
        auto pos = std::make_tuple(range1.begin_pos(), range2.begin_pos(), result.begin_pos());
        while (!range1.is_end_pos(std::get<0>(pos)) && !range2.is_end_pos(std::get<1>(pos)) && !result.is_end_pos(std::get<2>(pos)))
        {
            if (comp(range1.at_pos(std::get<0>(pos)), range2.at_pos(std::get<1>(pos))))
                range1.inc_pos(std::get<0>(pos));
            else if (comp(range2.at_pos(std::get<1>(pos)), range1.at_pos(std::get<0>(pos))))
                range2.inc_pos(std::get<1>(pos));
            else
            {
                write(result, std::get<2>(pos), read(range1, std::get<0>(pos)));
                range2.inc_pos(std::get<1>(pos));
            }
        }

        return pos;
    }

    template <class Range1, class Range2, class OutputRange>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_intersection(const Range1& range1, const Range2& range2, const OutputRange& result)
    {
        return set_intersection(range1, range2, result, std::less<>());
    }

    template <class Range1, class Range2, class OutputRange, class Compare>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_difference(const Range1& range1, const Range2& range2, const OutputRange& result, Compare&& comp)
    {
        auto pos = std::make_tuple(range1.begin_pos(), range2.begin_pos(), result.begin_pos());
        while (!range1.is_end_pos(std::get<0>(pos)) && !range2.is_end_pos(std::get<1>(pos)) && !result.is_end_pos(std::get<2>(pos)))
        {
            if (comp(range1.at_pos(std::get<0>(pos)), range2.at_pos(std::get<1>(pos))))
                write(result, std::get<2>(pos), read(range1, std::get<0>(pos)));
            else if (comp(range2.at_pos(std::get<1>(pos)), range1.at_pos(std::get<0>(pos))))
                range2.inc_pos(std::get<1>(pos));
            else
            {
                range1.inc_pos(std::get<0>(pos));
                range2.inc_pos(std::get<1>(pos));
            }
        }

        std::tie(std::get<0>(pos), std::get<2>(pos)) = copy(subrange_from(range1, std::get<0>(pos)), subrange_from(result, std::get<2>(pos)));
        return pos;
    }

    template <class Range1, class Range2, class OutputRange>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_difference(const Range1& range1, const Range2& range2, const OutputRange& result)
    {
        return set_difference(range1, range2, result, std::less<>());
    }


    template <class Range1, class Range2, class OutputRange, class Compare>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_symmetric_difference(const Range1& range1, const Range2& range2, const OutputRange& result, Compare&& comp)
    {
        auto pos = std::make_tuple(range1.begin_pos(), range2.begin_pos(), result.begin_pos());
        while (!range1.is_end_pos(std::get<0>(pos)) && !range2.is_end_pos(std::get<1>(pos)) && !result.is_end_pos(std::get<2>(pos)))
        {
            if (comp(range1.at_pos(std::get<0>(pos)), range2.at_pos(std::get<1>(pos))))
                write(result, std::get<2>(pos), read(range1, std::get<0>(pos)));
            else if (comp(range2.at_pos(std::get<1>(pos)), range1.at_pos(std::get<0>(pos))))
                write(result, std::get<2>(pos), read(range2, std::get<1>(pos)));
            else
            {
                range1.inc_pos(std::get<0>(pos));
                range2.inc_pos(std::get<1>(pos));
            }
        }

        std::tie(std::get<0>(pos), std::get<2>(pos)) = copy(subrange_from(range1, std::get<0>(pos)), subrange_from(result, std::get<2>(pos)));
        std::tie(std::get<1>(pos), std::get<2>(pos)) = copy(subrange_from(range2, std::get<1>(pos)), subrange_from(result, std::get<2>(pos)));
        return pos;
    }

    template <class Range1, class Range2, class OutputRange>
    std::tuple<range_position_type_t<Range1>, range_position_type_t<Range2>, range_position_type_t<OutputRange>>
        set_symmetric_difference(const Range1& range1, const Range2& range2, const OutputRange& result)
    {
        return set_symmetric_difference(range1, range2, result, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> min_element(const Range& range, Compare&& comp)
    {
        auto best = range.begin_pos();
        if (range.is_end_pos(best))
            return best;

        while (true)
        {
            auto pos = find_if(subrange_from(range, next_pos(range, best)),
                [&](const auto& value)
            {
                return comp(value, range.at_pos(best));
            });
            if (range.is_end_pos(pos))
                return best;
            best = pos;
        }
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> min_element(const Range& range)
    {
        return min_element(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> max_element(const Range& range, Compare&& comp)
    {
        auto best = range.begin_pos();
        if (range.is_end_pos(best))
            return best;

        while (true)
        {
            auto pos = find_if(subrange_from(range, next_pos(range, best)),
                [&](const auto& value)
            {
                return comp(range.at_pos(best), value);
            });
            if (range.is_end_pos(pos))
                return best;
            best = pos;
        }
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    range_position_type_t<Range> max_element(const Range& range)
    {
        return max_element(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    std::pair<range_position_type_t<Range>, range_position_type_t<Range>>
        minmax_element(const Range& range, Compare&& comp)
    {
        auto best = std::make_pair(range.begin_pos(), range.begin_pos());
        if (range.is_end_pos(best.first))
            return best;

        auto subrange = subrange_from(range, next_pos(range, best.first));
        for (auto pos = subrange.begin_pos(); pos != subrange.end_pos(); subrange.inc_pos(pos))
        {
            decltype(auto) value = subrange.at_pos(pos);
            if (comp(value, range.at_pos(best.first)))
                best.first = pos;
            if (comp(range.at_pos(best.second), value))
                best.second = pos;
        }

        return best;
    }

    template <class Range,
        STDEXT_REQUIRES(is_multi_pass_range<Range>::value)>
    std::pair<range_position_type_t<Range>, range_position_type_t<Range>>
        minmax_element(const Range& range)
    {
        return minmax_element(range, std::less<>());
    }

    template <class Range1, class Range2, class Compare>
    bool lexicographical_compare(const Range1& range1, const Range2& range2, Compare&& comp)
    {
        auto pos1 = range1.begin_pos();
        auto pos2 = range2.begin_pos();

        while (!range1.is_end_pos(pos1) && !range2.is_end_pos(pos2))
        {
            if (comp(range1.at_pos(pos1), range2.at_pos(pos2)))
                return true;
            if (comp(range2.at_pos(pos2), range1.at_pos(pos1)))
                return false;
        }

        return !range2.is_end_pos(pos2);
    }

    template <class Range1, class Range2>
    bool lexicographical_compare(const Range1& range1, const Range2& range2)
    {
        return lexicographical_compare(range1, range2, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    bool next_permutation(const Range& range, Compare&& comp)
    {
        auto rrange = make_reverse_range(range);
        auto rpos = adjacent_find(rrange, [&](const auto& v1, const auto& v2) { return comp(v2, v1); });
        auto pos = rrange.base_pos(rpos);
        if (pos == range.begin_pos())
        {
            sort(range, comp);
            return false;
        }

        range.dec_pos(pos);
        swap(range.at_pos(prev_pos(range, pos)), range.at_pos(pos));
        reverse(subrange_from(range, pos));
    }

    template <class Range,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    bool next_permutation(const Range& range)
    {
        return next_permutation(range, std::less<>());
    }

    template <class Range, class Compare,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    bool prev_permutation(const Range& range, Compare&& comp)
    {
        return next_permutation(range, [&](const auto& v1, const auto& v2) { return comp(v2, v1); });
    }

    template <class Range,
        STDEXT_REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    bool prev_permutation(const Range& range)
    {
        return next_permutation(range, std::greater<>());
    }
}

#endif
