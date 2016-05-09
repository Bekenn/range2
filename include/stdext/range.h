//
//  range.h
//  stdext
//
//  Created by James Touton on 10/19/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_RANGE_INCLUDED
#define STDEXT_RANGE_INCLUDED
#pragma once

#include "generator.h"
#include "utility.h"

#include <stdexcept>


namespace stdext
{
    // TODO: Drop the concept of a single-pass range and rethink generators.
    // Generators should be single-pass only; ranges should be multi-pass only.  Generators
    // should be optionally bounded.  Generators should not have positions, whereas positions
    // should always be meaningful for ranges.
    // One option: ranges should extend the interface for generators so that single-pass
    // algorithms continue to work without the use of a generator adapter.

    // range categories
    struct single_pass_range_tag { };
    struct multi_pass_range_tag : single_pass_range_tag { };
    struct bidirectional_range_tag : multi_pass_range_tag { };
    struct random_access_range_tag : bidirectional_range_tag { };

    // range traits
    namespace detail
    {
        DECLARE_HAS_INNER_TYPE(range_category);
    }
    template <class T> struct is_range
        : ::std::conditional_t<detail::HAS_INNER_TYPE(T, range_category),
            ::std::true_type,
            ::std::false_type>
    { };

    namespace detail
    {
        DECLARE_HAS_METHOD(end_pos);
        template <class T>
        struct check_end_pos_getter
        {
            static HAS_METHOD_T(T, end_pos) test(void*);
            static ::std::false_type test(...);
            static constexpr bool value = decltype(test(nullptr))::value;
        };
        template <class T>
        struct check_end_pos_setter
        {
            static HAS_METHOD_T(T, end_pos, position_type<T, is_range>) test(void*);
            static ::std::false_type test(...);
            static constexpr bool value = decltype(test(nullptr))::value;
        };
    }
    template <class T> struct is_delimited_range
        : ::std::conditional_t<is_range<T>::value && detail::check_end_pos_getter<T>::value && detail::check_end_pos_setter<T>::value,
            ::std::true_type,
            ::std::false_type>
    { };

    namespace detail
    {
        DECLARE_HAS_METHOD(size);
        template <class T>
        struct check_has_size
        {
            static HAS_METHOD_T(T, size) test(void*);
            static ::std::false_type test(...);
            static constexpr bool value = decltype(test(nullptr))::value;
        };
        DECLARE_HAS_METHOD(resize);
        template <class T>
        struct check_has_resize
        {
            static HAS_METHOD_T(T, resize, size_type<T, is_range>) test(void*);
            static ::std::false_type test(...);
            static constexpr bool value = decltype(test(nullptr))::value;
        };
    }
    template <class T> struct is_counted_range
        : ::std::conditional_t<is_range<T>::value && detail::check_has_size<T>::value && detail::check_has_resize<T>::value,
            ::std::true_type,
            ::std::false_type>
    { };

    template <class Range> using range_category = typename Range::range_category;

    namespace detail
    {
        template <class Range>
        struct value_type_of<Range, is_range> { using type = typename Range::value_type; };
        template <class Range>
        struct position_type_of<Range, is_range> { using type = typename Range::position; };
        template <class Range>
        struct difference_type_of<Range, is_range> { using type = typename Range::difference_type; };
        template <class Range>
        struct reference_type_of<Range, is_range> { using type = typename Range::reference; };

        DECLARE_HAS_INNER_TYPE(size_type);
        template <class T, bool has_inner_size_type>
        struct check_range_inner_size_type
        {
            using type = ::std::make_unsigned_t<difference_type<T, is_range>>;
        };
        template <class T>
        struct check_range_inner_size_type<T, true>
        {
            using type = typename T::size_type;
        };

        template <class Range>
        struct size_type_of<Range, is_range> { using type = typename check_range_inner_size_type<Range, HAS_INNER_TYPE(Range, size_type)>::type; };
    }

    // range concepts
    template <class Range> struct is_single_pass_range
    {
        static constexpr auto value = ::std::is_base_of<single_pass_range_tag, range_category<Range>>::value;
    };
    template <class Range> struct is_multi_pass_range
    {
        static constexpr auto value = ::std::is_base_of<multi_pass_range_tag, range_category<Range>>::value;
    };
    template <class Range> struct is_bidirectional_range
    {
        static constexpr auto value = ::std::is_base_of<bidirectional_range_tag, range_category<Range>>::value;
    };
    template <class Range> struct is_random_access_range
    {
        static constexpr auto value = ::std::is_base_of<random_access_range_tag, range_category<Range>>::value;
    };

    // generic range utilities
    template <class Range, REQUIRES(is_single_pass_range<Range>::value)>
    position_type<Range, is_range> next_pos(const Range& range, position_type<Range, is_range> pos)
    {
        return range.inc_pos(pos);
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value)>
    position_type<Range, is_range> prev_pos(const Range& range, position_type<Range, is_range> pos)
    {
        return range.dec_pos(pos);
    }

    namespace detail
    {
        template <class Range>
        position_type<Range, is_range>& advance_pos(const Range& range, position_type<Range, is_range>& pos, difference_type<Range, is_range> n, single_pass_range_tag)
        {
            assert(n >= 0);

            while (n-- != 0)
                range.inc_pos(pos);
            return pos;
        }

        template <class Range>
        position_type<Range, is_range>& advance_pos(const Range& range, position_type<Range, is_range>& pos, difference_type<Range, is_range> n, bidirectional_range_tag)
        {
            for (; n > 0; --n)
                range.inc_pos(pos);
            for (; n < 0; ++n)
                range.dec_pos(pos);
            return pos;
        }

        template <class Range>
        position_type<Range, is_range>& advance_pos(const Range& range, position_type<Range, is_range>& pos, difference_type<Range, is_range> n, random_access_range_tag)
        {
            return range.advance_pos(pos, n);
        }
    }

    template <class Range, REQUIRES(is_single_pass_range<Range>::value)>
    position_type<Range, is_range>& advance_pos(const Range& range, position_type<Range, is_range>& pos, difference_type<Range, is_range> n)
    {
        return detail::advance_pos(range, pos, n, range_category<Range>());
    }

    namespace detail
    {
        template <class Range>
        difference_type<Range, is_range> distance(const Range& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2, multi_pass_range_tag)
        {
            difference_type<Range, is_range> d = 0;
            for (; p1 != p2; range.inc_pos(p1))
                ++d;
            return d;
        }

        template <class Range>
        difference_type<Range, is_range> distance(const Range& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2, random_access_range_tag)
        {
            return range.distance(p1, p2);
        }
    }

    template <class Range, REQUIRES(is_multi_pass_range<Range>::value)>
    difference_type<Range, is_range> distance(const Range& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return detail::distance(range, p1, p2, range_category<Range>());
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    bool is_empty(const Range& range) noexcept
    {
        return range.is_end_pos(range.begin_pos());
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    value_type<Range, is_range> read(const Range& range, position_type<Range, is_range>& pos)
    {
        auto value = range.at_pos(pos);
        range.inc_pos(pos);
        return value;
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    value_type<Range, is_range> consume(const Range& range, position_type<Range, is_range>& pos)
    {
        auto value = ::std::move(range.at_pos(pos));
        range.inc_pos(pos);
        return value;
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    void write(const Range& range, position_type<Range, is_range>& pos, const value_type<Range, is_range>& value)
    {
        range.at_pos(pos) = value;
        range.inc_pos(pos);
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    void write(const Range& range, position_type<Range, is_range>& pos, value_type<Range, is_range>&& value)
    {
        range.at_pos(pos) = ::std::move(value);
        range.inc_pos(pos);
    }

    namespace detail
    {
        template <class Range>
        size_type<Range, is_range> size_before(const Range& range, position_type<Range, is_range> pos, multi_pass_range_tag)
        {
            size_type<Range, is_range> size = 0;
            for (auto p = range.begin_pos(); p != pos; range.inc_pos(p))
                ++size;
            return size;
        }

        template <class Range>
        size_type<Range, is_range> size_before(const Range& range, position_type<Range, is_range> pos, random_access_range_tag)
        {
            return size_type<Range, is_range>(range.distance(range.begin_pos(), pos));
        }

        template <class Range>
        size_type<Range, is_range> size_after(const Range& range, position_type<Range, is_range> pos, multi_pass_range_tag)
        {
            size_type<Range, is_range> size = 0;
            for (auto p = pos; !range.is_end_pos(p); range.inc_pos(p))
                ++size;
            return size;
        }

        template <class Range, REQUIRES(is_delimited_range<Range>::value)>
        size_type<Range, is_range> size_after(const Range& range, position_type<Range, is_range> pos, random_access_range_tag)
        {
            return size_type<Range, is_range>(range.distance(pos, range.end_pos()));
        }
    }

    template <class Range, REQUIRES(is_multi_pass_range<Range>::value)>
    size_type<Range, is_range> size_before(const Range& range, position_type<Range, is_range> pos)
    {
        return detail::size_before(range, pos, range_category<Range>());
    }

    template <class Range, REQUIRES(is_multi_pass_range<Range>::value)>
    size_type<Range, is_range> size_after(const Range& range, position_type<Range, is_range> pos)
    {
        return detail::size_after(range, pos, range_category<Range>());
    }

    template <class Range, REQUIRES(is_single_pass_range<Range>::value)>
    Range& drop_first(Range& range, size_type<Range, is_range> n = 1)
    {
        auto pos = range.begin_pos();
        range.begin_pos(advance_pos(range, pos, n));
        return range;
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    Range& drop_last(Range& range, size_type<Range, is_range> n = 1)
    {
        auto pos = range.end_pos();
        range.end_pos(advance_pos(range, pos, -difference_type<Range, is_range>(n)));
        return range;
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    reference_type<Range, is_range> head(const Range& range)
    {
        return range.at_pos(range.begin_pos());
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    reference_type<Range, is_range> last(const Range& range)
    {
        auto pos = range.end_pos();
        return range.at_pos(range.dec_pos(pos));
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    Range tail(const Range& range)
    {
        Range result = range;
        return drop_first(result);
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value && is_delimited_range<Range>::value)>
    Range init(const Range& range)
    {
        Range result = range;
        return drop_last(result);
    }

    // adapter ranges
    namespace detail
    {
        template <class Range, class TerminationPredicate> class delegated_single_pass_range;
        template <class Range, class TerminationPredicate> class delegated_multi_pass_range;
        template <class Range, class TerminationPredicate> class delegated_bidirectional_range;
        template <class Range, class TerminationPredicate> class delegated_random_access_range;

        template <class Range> class delimited_multi_pass_range;
        template <class Range> class delimited_bidirectional_range;
        template <class Range> class delimited_random_access_range;

        template <class Range> class counted_single_pass_range;
        template <class Range> class counted_multi_pass_range;
        template <class Range> class counted_bidirectional_range;
        template <class Range> class counted_random_access_range;

        template <class Range, class TerminationPredicate> class reverse_bidirectional_range;
        template <class Range, class TerminationPredicate> class reverse_random_access_range;

        template <class Range> class delimited_reverse_bidirectional_range;
        template <class Range> class delimited_reverse_random_access_range;

        template <class Range> class counted_reverse_bidirectional_range;
        template <class Range> class counted_reverse_random_access_range;

        template <class Generator, class TerminationPredicate> class single_pass_generator_range;
        template <class Generator, class TerminationPredicate> class multi_pass_generator_range;
        template <class Generator, class TerminationPredicate> class bidirectional_generator_range;
        template <class Generator, class TerminationPredicate> class random_access_generator_range;

        template <class Generator> class delimited_multi_pass_generator_range;
        template <class Generator> class delimited_bidirectional_generator_range;
        template <class Generator> class delimited_random_access_generator_range;

        template <class GeneratorCategory> struct generator_range_category_map;
        template <> struct generator_range_category_map<single_pass_generator_tag> { using type = single_pass_range_tag; };
        template <> struct generator_range_category_map<multi_pass_generator_tag> { using type = multi_pass_range_tag; };
        template <> struct generator_range_category_map<bidirectional_generator_tag> { using type = bidirectional_range_tag; };
        template <> struct generator_range_category_map<random_access_generator_tag> { using type = random_access_range_tag; };
        template <class GeneratorCategory> using generator_range_category_map_t = typename generator_range_category_map<GeneratorCategory>::type;

        template <class Generator> class counted_single_pass_generator_range;
        template <class Generator> class counted_multi_pass_generator_range;
        template <class Generator> class counted_bidirectional_generator_range;
        template <class Generator> class counted_random_access_generator_range;

        template <class Iterator, class TerminationPredicate> class input_iterator_range;
        template <class Iterator, class TerminationPredicate> class forward_iterator_range;
        template <class Iterator, class TerminationPredicate> class bidirectional_iterator_range;
        template <class Iterator, class TerminationPredicate> class random_access_iterator_range;

        template <class Iterator> class delimited_input_iterator_range;
        template <class Iterator> class delimited_forward_iterator_range;
        template <class Iterator> class delimited_bidirectional_iterator_range;
        template <class Iterator> class delimited_random_access_iterator_range;

        template <class Iterator> class counted_input_iterator_range;
        template <class Iterator> class counted_forward_iterator_range;
        template <class Iterator> class counted_bidirectional_iterator_range;
        template <class Iterator> class counted_random_access_iterator_range;

        template <class IteratorCategory> struct iterator_range_category_map;
        template <> struct iterator_range_category_map<::std::input_iterator_tag> { using type = single_pass_range_tag; };
        template <> struct iterator_range_category_map<::std::forward_iterator_tag> { using type = multi_pass_range_tag; };
        template <> struct iterator_range_category_map<::std::bidirectional_iterator_tag> { using type = bidirectional_range_tag; };
        template <> struct iterator_range_category_map<::std::random_access_iterator_tag> { using type = random_access_range_tag; };
        template <class IteratorCategory> using iterator_range_category_map_t = typename iterator_range_category_map<IteratorCategory>::type;

        template <class Range> class single_pass_range_iterator;
        template <class Range> class multi_pass_range_iterator;
        template <class Range> class bidirectional_range_iterator;
        template <class Range> class random_access_range_iterator;

        template <class RangeCategory> struct range_iterator_category_map;
        template <> struct range_iterator_category_map<single_pass_range_tag> { using type = ::std::input_iterator_tag; };
        template <> struct range_iterator_category_map<multi_pass_range_tag> { using type = ::std::forward_iterator_tag; };
        template <> struct range_iterator_category_map<bidirectional_range_tag> { using type = ::std::bidirectional_iterator_tag; };
        template <> struct range_iterator_category_map<random_access_range_tag> { using type = ::std::random_access_iterator_tag; };
        template <class RangeCategory> using range_iterator_category_map_t = typename range_iterator_category_map<RangeCategory>::type;
    }

    template <class Range, class TerminationPredicate> using delegated_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::delegated_random_access_range<Range, TerminationPredicate>,
            ::std::conditional_t<is_bidirectional_range<Range>::value, detail::delegated_bidirectional_range<Range, TerminationPredicate>,
                ::std::conditional_t<is_multi_pass_range<Range>::value, detail::delegated_multi_pass_range<Range, TerminationPredicate>,
                    ::std::enable_if_t<is_single_pass_range<Range>::value, detail::delegated_single_pass_range<Range, TerminationPredicate>>
                >
            >
        >;

    template <class Range> using delimited_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::delimited_random_access_range<Range>,
            ::std::conditional_t<is_bidirectional_range<Range>::value, detail::delimited_bidirectional_range<Range>,
                ::std::enable_if_t<is_multi_pass_range<Range>::value, detail::delimited_multi_pass_range<Range>>
            >
        >;

    template <class Range> using counted_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::counted_random_access_range<Range>,
            ::std::conditional_t<is_bidirectional_range<Range>::value, detail::counted_bidirectional_range<Range>,
                ::std::conditional_t<is_multi_pass_range<Range>::value, detail::counted_multi_pass_range<Range>,
                    ::std::enable_if_t<is_single_pass_range<Range>::value, detail::counted_single_pass_range<Range>>
                >
            >
        >;

    template <class Range, class TerminationPredicate> using reverse_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::reverse_random_access_range<Range, TerminationPredicate>,
            ::std::enable_if_t<is_bidirectional_range<Range>::value, detail::reverse_bidirectional_range<Range, TerminationPredicate>>
        >;

    template <class Range> using delimited_reverse_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::delimited_reverse_random_access_range<Range>,
            ::std::enable_if_t<is_bidirectional_range<Range>::value, detail::delimited_reverse_bidirectional_range<Range>>
        >;

    template <class Range> using counted_reverse_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::counted_reverse_random_access_range<Range>,
            ::std::enable_if_t<is_bidirectional_range<Range>::value, detail::counted_reverse_bidirectional_range<Range>>
        >;

    template <class Generator, class TerminationPredicate> using generator_range =
        ::std::conditional_t<is_random_access_generator<Generator>::value, detail::random_access_generator_range<Generator, TerminationPredicate>,
            ::std::conditional_t<is_bidirectional_generator<Generator>::value, detail::bidirectional_generator_range<Generator, TerminationPredicate>,
                ::std::conditional_t<is_multi_pass_generator<Generator>::value, detail::multi_pass_generator_range<Generator, TerminationPredicate>,
                    ::std::enable_if_t<is_single_pass_generator<Generator>::value, detail::single_pass_generator_range<Generator, TerminationPredicate>>
                >
            >
        >;

    template <class Generator> using delimited_generator_range =
        ::std::conditional_t<is_random_access_generator<Generator>::value, detail::delimited_random_access_generator_range<Generator>,
            ::std::conditional_t<is_bidirectional_generator<Generator>::value, detail::delimited_bidirectional_generator_range<Generator>,
                ::std::enable_if_t<is_multi_pass_generator<Generator>::value, detail::delimited_multi_pass_generator_range<Generator>>
            >
        >;

    template <class Generator> using counted_generator_range =
        ::std::conditional_t<is_random_access_generator<Generator>::value, detail::counted_random_access_generator_range<Generator>,
            ::std::conditional_t<is_bidirectional_generator<Generator>::value, detail::counted_bidirectional_generator_range<Generator>,
                ::std::conditional_t<is_multi_pass_generator<Generator>::value, detail::counted_multi_pass_generator_range<Generator>,
                    ::std::enable_if_t<is_single_pass_generator<Generator>::value, detail::counted_single_pass_generator_range<Generator>>
                >
            >
        >;

    template <class Iterator, class TerminationPredicate> using iterator_range =
        ::std::conditional_t<is_random_access_iterator<Iterator>::value, detail::random_access_iterator_range<Iterator, TerminationPredicate>,
            ::std::conditional_t<is_bidirectional_iterator<Iterator>::value, detail::bidirectional_iterator_range<Iterator, TerminationPredicate>,
                ::std::conditional_t<is_forward_iterator<Iterator>::value, detail::forward_iterator_range<Iterator,TerminationPredicate>,
                    ::std::enable_if_t<is_input_iterator<Iterator>::value, detail::input_iterator_range<Iterator, TerminationPredicate>>
                >
            >
        >;

    template <class Iterator> using delimited_iterator_range =
        ::std::conditional_t<is_random_access_iterator<Iterator>::value, detail::delimited_random_access_iterator_range<Iterator>,
            ::std::conditional_t<is_bidirectional_iterator<Iterator>::value, detail::delimited_bidirectional_iterator_range<Iterator>,
                ::std::conditional_t<is_forward_iterator<Iterator>::value, detail::delimited_forward_iterator_range<Iterator>,
                    ::std::enable_if_t<is_input_iterator<Iterator>::value, detail::delimited_input_iterator_range<Iterator>>
                >
            >
        >;

    template <class Iterator> using counted_iterator_range =
        ::std::conditional_t<is_random_access_iterator<Iterator>::value, detail::counted_random_access_iterator_range<Iterator>,
            ::std::conditional_t<is_bidirectional_iterator<Iterator>::value, detail::counted_bidirectional_iterator_range<Iterator>,
                ::std::conditional_t<is_forward_iterator<Iterator>::value, detail::counted_forward_iterator_range<Iterator>,
                    ::std::enable_if_t<is_input_iterator<Iterator>::value, detail::counted_input_iterator_range<Iterator>>
                >
            >
        >;

    template <class Range> using range_iterator =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::random_access_range_iterator<Range>,
            ::std::conditional_t<is_bidirectional_range<Range>::value, detail::bidirectional_range_iterator<Range>,
                ::std::conditional_t<is_multi_pass_range<Range>::value, detail::multi_pass_range_iterator<Range>,
                    ::std::enable_if_t<is_single_pass_range<Range>::value, detail::single_pass_range_iterator<Range>>
                >
            >
        >;

    template <class Range, class TerminationPredicate, REQUIRES(is_range<Range>::value)>
    auto make_range(const Range& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return delegated_range<Range, ::std::decay_t<TerminationPredicate>>(range, pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class DelegatedPredicate, class TerminationPredicate>
    auto make_range(const delegated_range<Range, DelegatedPredicate>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return delegated_range<Range, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class TerminationPredicate>
    auto make_range(const delimited_range<Range>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return delegated_range<Range, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class DelegatedPredicate, class TerminationPredicate>
    auto make_range(const reverse_range<Range, DelegatedPredicate>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return reverse_range<Range, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class TerminationPredicate>
    auto make_range(const delimited_reverse_range<Range>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return reverse_range<Range, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Generator, class RangePredicate, class TerminationPredicate>
    auto make_range(const generator_range<Generator, RangePredicate>& range, position_type<Generator, is_generator> pos, TerminationPredicate&& pred)
    {
        return generator_range<Generator, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Generator, class TerminationPredicate>
    auto make_range(const delimited_generator_range<Generator>& range, position_type<Generator, is_generator> pos, TerminationPredicate&& pred)
    {
        return generator_range<Generator, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Iterator, class DelegatedPredicate, class TerminationPredicate>
    auto make_range(const iterator_range<Iterator, DelegatedPredicate>& range, Iterator pos, TerminationPredicate&& pred)
    {
        return iterator_range<Iterator, ::std::decay_t<TerminationPredicate>>(pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Iterator, class TerminationPredicate>
    auto make_range(const delimited_iterator_range<Iterator>& range, Iterator pos, TerminationPredicate&& pred)
    {
        return iterator_range<Iterator, ::std::decay_t<TerminationPredicate>>(pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    auto make_range(const Range& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_range<Range>(range, p1, p2);
    }

    template <class Range, class TerminationPredicate>
    auto make_range(const delegated_range<Range, TerminationPredicate>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_range<Range>(range.base(), p1, p2);
    }

    template <class Range>
    auto make_range(const delimited_range<Range>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_range<Range>(range.base(), p1, p2);
    }

    template <class Range, class TerminationPredicate>
    auto make_range(const reverse_range<Range, TerminationPredicate>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_reverse_range<Range>(range, p1, p2);
    }

    template <class Range>
    auto make_range(const delimited_reverse_range<Range>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_reverse_range<Range>(range, p1, p2);
    }

    template <class Generator, class TerminationPredicate>
    auto make_range(const generator_range<Generator, TerminationPredicate>& range, position_type<Generator, is_generator> p1, position_type<Generator, is_generator> p2)
    {
        return delimited_generator_range<Generator>(range.base(), p1, p2);
    }

    template <class Generator>
    auto make_range(const delimited_generator_range<Generator>& range, position_type<Generator, is_generator> p1, position_type<Generator, is_generator> p2)
    {
        return delimited_generator_range<Generator>(range.base(), p1, p2);
    }

    template <class Iterator, class TerminationPredicate>
    auto make_range(const iterator_range<Iterator, TerminationPredicate>& range, Iterator p1, Iterator p2)
    {
        return delimited_iterator_range<Iterator>(p1, p2);
    }

    template <class Iterator>
    auto make_range(const delimited_iterator_range<Iterator>& range, Iterator p1, Iterator p2)
    {
        return delimited_iterator_range<Iterator>(p1, p2);
    }

    template <class Generator, class TerminationPredicate, REQUIRES(is_generator<Generator>::value)>
    auto make_range(const Generator& gen, TerminationPredicate&& pred)
    {
        return generator_range<Generator, ::std::decay_t<TerminationPredicate>>(gen, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Generator, REQUIRES(is_generator<Generator>::value)>
    auto make_range(const Generator& gen, position_type<Generator, is_generator> p1, position_type<Generator, is_generator> p2)
    {
        return delimited_generator_range<Generator>(gen, p1, p2);
    }

    template <class Iterator, class TerminationPredicate>
    auto make_range(const iterator_generator<Iterator>& gen, Iterator pos, TerminationPredicate&& pred)
    {
        return iterator_range<Iterator, ::std::decay_t<TerminationPredicate>>(pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Iterator>
    auto make_range(const iterator_generator<Iterator>& gen, Iterator p1, Iterator p2)
    {
        return delimited_iterator_range<Iterator>(p1, p2);
    }

    template <class Iterator, class TerminationPredicate, REQUIRES(is_iterator<Iterator>::value)>
    auto make_range(Iterator i, TerminationPredicate&& pred)
    {
        return iterator_range<Iterator, ::std::decay_t<TerminationPredicate>>(i, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Iterator, REQUIRES(is_iterator<Iterator>::value)>
    auto make_range(Iterator i, Iterator j)
    {
        return delimited_iterator_range<Iterator>(i, j);
    }

    template <class Range, class TerminationPredicate>
    auto make_range(range_iterator<Range> i, TerminationPredicate&& pred)
    {
        return make_range(i.base_range(), i.base_pos(), ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range>
    auto make_range(range_iterator<Range> i, range_iterator<Range> j)
    {
        assert(i.base_range() == j.base_range());
        return make_range(i.base_range(), i.base_pos(), j.base_pos());
    }

    template <class Generator, class TerminationPredicate>
    auto make_range(generator_iterator<Generator> i, TerminationPredicate&& pred)
    {
        return make_range(i.base_generator(), i.base_pos(), ::std::forward<TerminationPredicate>(pred));
    }

    template <class Generator>
    auto make_range(generator_iterator<Generator> i, generator_iterator<Generator> j)
    {
        assert(i.base_generator() == j.base_generator());
        return make_range(i.base_generator(), i.base_pos(), j.base_pos());
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    auto make_counted_range(const Range& range, position_type<Range, is_range> pos, size_type<Range, is_range> count)
    {
        return counted_range<Range>(range, pos, count);
    }

    template <class Range, class TerminationPredicate>
    auto make_counted_range(const delegated_range<Range, TerminationPredicate>& range, position_type<Range, is_range> pos, size_type<Range, is_range> count)
    {
        return counted_range<Range>(range.base(), pos, count);
    }

    template <class Range>
    auto make_counted_range(const delimited_range<Range>& range, position_type<Range, is_range> pos, size_type<Range, is_range> count)
    {
        return counted_range<Range>(range.base(), pos, count);
    }

    template <class Range>
    auto make_counted_range(const counted_range<Range>& range, position_type<counted_range<Range>, is_range> pos, size_type<counted_range<Range>, is_range> count)
    {
        return counted_range<Range>(range.base(), range.base_pos(pos), count);
    }

    template <class Range, class TerminationPredicate>
    auto make_counted_range(const reverse_range<Range, TerminationPredicate>& range, position_type<Range, is_range> pos, size_type<Range, is_range> count)
    {
        return counted_reverse_range<Range>(range.base(), pos, count);
    }

    template <class Range>
    auto make_counted_range(const delimited_reverse_range<Range>& range, position_type<Range, is_range> pos, size_type<Range, is_range> count)
    {
        return counted_reverse_range<Range>(range.base(), pos, count);
    }

    template <class Range>
    auto make_counted_range(const counted_reverse_range<Range>& range, position_type<counted_reverse_range<Range>, is_range> pos, size_type<counted_reverse_range<Range>, is_range> count)
    {
        return counted_reverse_range<Range>(range.base(), range.base_pos(pos), count);
    }

    template <class Generator, class TerminationPredicate>
    auto make_counted_range(const generator_range<Generator, TerminationPredicate>& range, position_type<Generator, is_generator> pos, size_type<Generator, is_generator> count)
    {
        return counted_generator_range<Generator>(range.base(), pos, count);
    }

    template <class Generator>
    auto make_counted_range(const delimited_generator_range<Generator>& range, position_type<Generator, is_generator> pos, size_type<Generator, is_generator> count)
    {
        return counted_generator_range<Generator>(range.base(), pos, count);
    }

    template <class Generator>
    auto make_counted_range(const counted_generator_range<Generator>& range, position_type<counted_generator_range<Generator>, is_range> pos, size_type<counted_generator_range<Generator>, is_range> count)
    {
        return counted_generator_range<Generator>(range.base(), range.base_pos(pos), count);
    }

    template <class Iterator, class TerminationPredicate>
    auto make_counted_range(const iterator_range<Iterator, TerminationPredicate>& range, Iterator pos, size_type<Iterator, is_iterator> count)
    {
        return counted_iterator_range<Iterator>(pos, count);
    }

    template <class Iterator>
    auto make_counted_range(const delimited_iterator_range<Iterator>& range, Iterator pos, size_type<Iterator, is_iterator> count)
    {
        return counted_iterator_range<Iterator>(pos, count);
    }

    template <class Iterator>
    auto make_counted_range(const counted_iterator_range<Iterator>& range, position_type<counted_iterator_range<Iterator>, is_range> pos, size_type<counted_iterator_range<Iterator>, is_range> count)
    {
        return counted_iterator_range<Iterator>(range.base_pos(pos), count);
    }

    template <class Generator, REQUIRES(is_generator<Generator>::value)>
    auto make_counted_range(const Generator& gen, position_type<Generator, is_generator> pos, size_type<Generator, is_generator> count)
    {
        return counted_generator_range<Generator>(gen, pos, count);
    }

    template <class Iterator>
    auto make_counted_range(const iterator_generator<Iterator>& gen, Iterator pos, size_type<Iterator, is_iterator> count)
    {
        return counted_iterator_range<Iterator>(pos, count);
    }

    template <class Iterator, REQUIRES(is_iterator<Iterator>::value)>
    auto make_counted_range(Iterator it, size_type<Iterator, is_iterator> count)
    {
        return counted_iterator_range<Iterator>(::std::forward(it), count);
    }

    template <class Range>
    auto make_counted_range(range_iterator<Range> it, size_type<Range, is_range> count)
    {
        return make_counted_range(it.base_range(), it.base_pos(), count);
    }

    template <class Generator>
    auto make_counted_range(generator_iterator<Generator> it, size_type<Generator, is_generator> count)
    {
        return counted_generator_range<Generator>(it.base_generator(), it.base_pos(), count);
    }

    template <class Range, class TerminationPredicate, REQUIRES(is_bidirectional_range<Range>::value)>
    auto make_reverse_range(const Range& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return reverse_range<Range, ::std::decay_t<TerminationPredicate>>(range, pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class DelegatedPredicate, class TerminationPredicate>
    auto make_reverse_range(const delegated_range<Range, DelegatedPredicate>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return reverse_range<Range, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class TerminationPredicate>
    auto make_reverse_range(const delimited_range<Range>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return reverse_range<Range, ::std::decay_t<TerminationPredicate>>(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class DelegatedPredicate, class TerminationPredicate>
    auto make_reverse_range(const reverse_range<Range, DelegatedPredicate>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return make_range(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class TerminationPredicate>
    auto make_reverse_range(const delimited_reverse_range<Range>& range, position_type<Range, is_range> pos, TerminationPredicate&& pred)
    {
        return make_range(range.base(), pos, ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, class TerminationPredicate>
    auto make_reverse_range(const counted_reverse_range<Range>& range, position_type<counted_reverse_range<Range>, is_range> pos, TerminationPredicate&& pred)
    {
        return make_range(range.base(), range.base_pos(pos), ::std::forward<TerminationPredicate>(pred));
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value)>
    auto make_reverse_range(const Range& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_reverse_range<Range>(range, p1, p2);
    }

    template <class Range, class TerminationPredicate>
    auto make_reverse_range(const delegated_range<Range, TerminationPredicate>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_reverse_range<Range>(range.base(), p1, p2);
    }

    template <class Range>
    auto make_reverse_range(const delimited_range<Range>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return delimited_reverse_range<Range>(range.base(), p1, p2);
    }

    template <class Range, class TerminationPredicate>
    auto make_reverse_range(const reverse_range<Range, TerminationPredicate>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return make_range(range.base(), p2, p1);
    }

    template <class Range>
    auto make_reverse_range(const delimited_reverse_range<Range>& range, position_type<Range, is_range> p1, position_type<Range, is_range> p2)
    {
        return make_range(range.base(), p2, p1);
    }

    template <class Range>
    auto make_reverse_range(const counted_reverse_range<Range>& range, position_type<counted_reverse_range<Range>, is_range> p1, position_type<counted_reverse_range<Range>, is_range> p2)
    {
        return make_range(range.base(), range.base_pos(p2), range.base_pos(p1));
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value), REQUIRES(is_delimited_range<Range>::value)>
    auto make_reverse_range(const Range& range)
    {
        return make_reverse_range(range, range.begin_pos(), range.end_pos());
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    auto make_iterator(const Range& range, position_type<Range, is_range> pos)
    {
        return range_iterator<Range>(pos);
    }

    template <class Iterator, class TerminationPredicate>
    auto make_iterator(const iterator_range<Iterator, TerminationPredicate>& range, Iterator pos)
    {
        return pos;
    }

    template <class Iterator>
    auto make_iterator(const delimited_iterator_range<Iterator>& range, Iterator pos)
    {
        return pos;
    }

    template <class Iterator>
    auto make_iterator(const counted_iterator_range<Iterator>& range, position_type<counted_iterator_range<Iterator>, is_range> pos)
    {
        return range.base_pos(pos);
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    auto subrange_from(const Range& range, position_type<Range, is_range> first)
    {
        return make_range(range, first, [&](const auto& pos) { return range.is_end_pos(pos); });
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    auto subrange_to(const Range& range, position_type<Range, is_range> last)
    {
        return make_range(range, range.begin_pos(), last);
    }

    template <class Range, REQUIRES(is_range<Range>::value)>
    auto split_range(const Range& range, position_type<Range, is_range> pos)
    {
        return ::std::make_pair(subrange_to(range, pos), subrange_from(range, pos));
    }

    namespace detail
    {
        // Iterator ranges
        template <class Iterator, class TerminationPredicate>
        class input_iterator_range
        {
        public:
            using range_category = detail::iterator_range_category_map_t<iterator_category<Iterator>>;
            using value_type = value_type<Iterator, is_iterator>;
            using position = Iterator;
            using difference_type = difference_type<Iterator, is_iterator>;
            using reference = reference_type<Iterator, is_iterator>;
            using iterator = Iterator;

            class sentinel
            {
            public:
                explicit sentinel(const input_iterator_range& range) noexcept : range(&range) { }
                friend bool operator == (const iterator& i, const sentinel& s) noexcept
                {
                    return s.range->is_end_pos(i);
                }
                friend bool operator == (const sentinel& s, const iterator& i) noexcept
                {
                    return i == s;
                }
                friend bool operator != (const iterator& i, const sentinel& s) noexcept
                {
                    return !(i == s);
                }
                friend bool operator != (const sentinel& s, const iterator& i) noexcept
                {
                    return !(s == i);
                }
            private:
                input_iterator_range* range;
            };

        public:
            input_iterator_range() noexcept : first(), term() { }
            input_iterator_range(iterator first, const TerminationPredicate& term)
                : first(first), term(term) { }
            input_iterator_range(iterator first, TerminationPredicate&& term)
                : first(first), term(::std::move(term)) { }

        public:
            friend bool operator == (const input_iterator_range& a, const input_iterator_range& b) noexcept
            {
                return a.first == b.first && a.term == b.term;
            }
            friend bool operator != (const input_iterator_range& a, const input_iterator_range& b) noexcept
            {
                return !(a == b);
            }

            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) noexcept(noexcept(first = pos)) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return term(*pos); }

            position& inc_pos(position& pos) const { return ++pos; }
            reference at_pos(position pos) const { return *pos; }

            iterator begin() const { return first; }
            sentinel end() const { return sentinel(*this); }

        private:
            position first;
            TerminationPredicate term;
        };

        template <class Iterator, class TerminationPredicate>
        class forward_iterator_range : public input_iterator_range<Iterator, TerminationPredicate>
        {
        public:
            using input_iterator_range<Iterator, TerminationPredicate>::input_iterator_range;
        };

        template <class Iterator, class TerminationPredicate>
        class bidirectional_iterator_range : public forward_iterator_range<Iterator, TerminationPredicate>
        {
        public:
            using typename forward_iterator_range<Iterator, TerminationPredicate>::position;

        public:
            using forward_iterator_range<Iterator, TerminationPredicate>::forward_iterator_range;

        public:
            position& dec_pos(position& pos) const { return --pos; }
        };

        template <class Iterator, class TerminationPredicate>
        class random_access_iterator_range : public bidirectional_iterator_range<Iterator, TerminationPredicate>
        {
        public:
            using typename bidirectional_iterator_range<Iterator, TerminationPredicate>::position;
            using typename bidirectional_iterator_range<Iterator, TerminationPredicate>::difference_type;
            using typename bidirectional_iterator_range<Iterator, TerminationPredicate>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using bidirectional_iterator_range<Iterator, TerminationPredicate>::bidirectional_iterator_range;

        public:
            position& advance_pos(position& pos, difference_type n) const
            {
                ::std::advance(pos, n);
                return pos;
            }

            difference_type distance(position p1, position p2) const noexcept { return ::std::distance(p1, p2); }

            reference operator [] (size_type n) const { return this->begin_pos()[n]; }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return this->begin_pos()[n];
            }
        };

        template <class Iterator>
        class delimited_input_iterator_range
        {
        public:
            using range_category = detail::iterator_range_category_map_t<iterator_category<Iterator>>;
            using value_type = value_type<Iterator, is_iterator>;
            using position = Iterator;
            using difference_type = difference_type<Iterator, is_iterator>;
            using reference = reference_type<Iterator, is_iterator>;
            using iterator = Iterator;

        public:
            delimited_input_iterator_range() noexcept : first(), last() { }
            delimited_input_iterator_range(iterator first, iterator last) noexcept(noexcept(iterator(first)))
                : first(first), last(last) { }

        public:
            friend bool operator == (const delimited_input_iterator_range& a, const delimited_input_iterator_range& b) noexcept
            {
                return a.first == b.first && a.last == b.last;
            }
            friend bool operator != (const delimited_input_iterator_range& a, const delimited_input_iterator_range& b) noexcept
            {
                return !(a == b);
            }

            position begin_pos() const { return first; }
            void begin_pos(position pos) { first = pos; }

            position end_pos() const { return last; }
            void end_pos(position pos) { last = pos; }

            bool is_end_pos(position pos) const noexcept { return pos == last; }

            position& inc_pos(position& pos) const { return ++pos; }
            reference at_pos(position pos) const { return *pos; }

            iterator begin() const { return first; }
            iterator end() const { return last; }

        private:
            position first;
            position last;
        };

        template <class Iterator>
        class delimited_forward_iterator_range : public delimited_input_iterator_range<Iterator>
        {
        public:
            using delimited_input_iterator_range<Iterator>::delimited_input_iterator_range;
        };

        template <class Iterator>
        class delimited_bidirectional_iterator_range : public delimited_forward_iterator_range<Iterator>
        {
        public:
            using typename delimited_forward_iterator_range<Iterator>::position;

        public:
            using delimited_forward_iterator_range<Iterator>::delimited_forward_iterator_range;

        public:
            position& dec_pos(position& pos) const { return --pos; }
        };

        template <class Iterator>
        class delimited_random_access_iterator_range : public delimited_bidirectional_iterator_range<Iterator>
        {
        public:
            using typename delimited_bidirectional_iterator_range<Iterator>::position;
            using typename delimited_bidirectional_iterator_range<Iterator>::difference_type;
            using typename delimited_bidirectional_iterator_range<Iterator>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using delimited_bidirectional_iterator_range<Iterator>::delimited_bidirectional_iterator_range;

        public:
            position& advance_pos(position& pos, difference_type n) const
            {
                ::std::advance(pos, n);
                return pos;
            }

            difference_type distance(position p1, position p2) const noexcept { return ::std::distance(p1, p2); }
            size_type size() const noexcept { return size_type(distance(this->begin_pos(), this->end_pos())); }
            void resize(size_type n) { this->end_pos(this->begin_pos() + n); }

            reference operator [] (size_type n) const { return this->begin_pos()[n]; }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return this->begin_pos()[n];
            }
        };

        template <class Iterator>
        class counted_input_iterator_range
        {
        public:
            using range_category = detail::iterator_range_category_map_t<iterator_category<Iterator>>;
            using value_type = value_type<Iterator, is_iterator>;
            using difference_type = difference_type<Iterator, is_iterator>;
            using reference = reference_type<Iterator, is_iterator>;
            using iterator = Iterator;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<iterator, size_type>;

        public:
            counted_input_iterator_range() : first(), count(0) { }
            counted_input_iterator_range(iterator i, size_type count) : first(i, 0), count(count) { }

            friend bool operator == (const counted_input_iterator_range& a, const counted_input_iterator_range& b) noexcept
            {
                return a.first == b.first
                    && a.count == b.count;
            }
            friend bool operator != (const counted_input_iterator_range& a, const counted_input_iterator_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) noexcept(noexcept(first = pos)) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return pos.second() == count; }

            position& inc_pos(position& pos) const
            {
                ++pos.first();
                ++pos.second();
                return pos;
            }

            reference at_pos(position pos) const { return *pos.first(); }

            size_type size() const noexcept { return count - first.second(); }
            void resize(size_type n) noexcept { count = first.second() + n; }

            iterator base_pos(position pos) const { return pos.first(); }

        private:
            position first;
            size_type count;
        };

        template <class Iterator>
        class counted_forward_iterator_range : public counted_input_iterator_range<Iterator>
        {
        public:
            using counted_input_iterator_range<Iterator>::counted_input_iterator_range;
        };

        template <class Iterator>
        class counted_bidirectional_iterator_range : public counted_forward_iterator_range<Iterator>
        {
        public:
            using typename counted_forward_iterator_range<Iterator>::position;
            using typename counted_forward_iterator_range<Iterator>::difference_type;

        public:
            using counted_forward_iterator_range<Iterator>::counted_forward_iterator_range;

        public:
            position& dec_pos(position& pos) const
            {
                --pos.first();
                --pos.second();
                return pos;
            }
        };

        template <class Iterator>
        class counted_random_access_iterator_range : public counted_bidirectional_iterator_range<Iterator>
        {
        public:
            using typename counted_bidirectional_iterator_range<Iterator>::position;
            using typename counted_bidirectional_iterator_range<Iterator>::difference_type;
            using typename counted_bidirectional_iterator_range<Iterator>::reference;
            using typename counted_bidirectional_iterator_range<Iterator>::size_type;

        public:
            using counted_bidirectional_iterator_range<Iterator>::counted_bidirectional_iterator_range;

        public:
            position end_pos() const
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, this->size());
                return pos;
            }
            void end_pos(position pos) noexcept
            {
                this->resize(pos.second() - this->begin_pos().second());
            }

            position& advance_pos(position& pos, difference_type n) const
            {
                ::std::advance(pos.first(), n);
                pos.second() += n;
                return pos;
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return difference_type(p2.second() - p1.second());
            }

            reference operator [] (size_type n) const { return *(first.first() + n); }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        // Range iterators
        template <class Range>
        class single_pass_range_iterator
        {
        public:
            using iterator_category = detail::range_iterator_category_map<range_category<Range>>;
            using value_type = value_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using pointer = value_type*;
            using reference = reference_type<Range, is_range>;
            using position = position_type<Range, is_range>;
            using range = Range;

        public:
            single_pass_range_iterator() : r(nullptr), p() { }
            single_pass_range_iterator(const range& r, position pos) noexcept(noexcept(position(pos)))
                : r(&r), p(pos) { }

            friend bool operator == (const single_pass_range_iterator& a, const single_pass_range_iterator& b) noexcept
            {
                return (a.r == b.r || (a.r != nullptr && b.r != nullptr && *a.r == *b.r))
                    && a.p == b.p;
            }
            friend bool operator != (const single_pass_range_iterator& a, const single_pass_range_iterator& b) noexcept
            {
                return !(a == b);
            }

        public:
            reference operator * () const { return r->at_pos(p); }
            pointer operator -> () const { return ::std::addressof(r->at_pos(p)); }
            single_pass_range_iterator& operator ++ ()
            {
                r->inc_pos(p);
                return *this;
            }
            iterator_proxy<single_pass_range_iterator> operator ++ (int)
            {
                iterator_proxy<single_pass_range_iterator> result(r->at_pos(p));
                ++*this;
                return result;
            }

            const range& base_range() const noexcept { return *r; }
            position base_pos() const noexcept { return p; }

            void swap(single_pass_range_iterator& other)
                noexcept(noexcept(swap(p, other.p)))
            {
                swap(r, other.r);
                swap(p, other.p);
            }

            friend void swap(single_pass_range_iterator& a, single_pass_range_iterator& b)
                noexcept(noexcept(a.swap(b)))
            {
                a.swap(b);
            }

        private:
            friend class multi_pass_range_iterator<Range>;
            friend class bidirectional_range_iterator<Range>;
            friend class random_access_range_iterator<Range>;

            const range* r;
            position p;
        };

        template <class Range>
        class multi_pass_range_iterator : public single_pass_range_iterator<Range>
        {
        public:
            using single_pass_range_iterator<Range>::single_pass_range_iterator;

        public:
            using single_pass_range_iterator<Range>::operator ++;
            multi_pass_range_iterator operator ++ (int)
            {
                multi_pass_range_iterator tmp = *this;
                ++*this;
                return tmp;
            }
        };

        template <class Range>
        class bidirectional_range_iterator : public multi_pass_range_iterator<Range>
        {
        public:
            using multi_pass_range_iterator<Range>::multi_pass_range_iterator;

        public:
            bidirectional_range_iterator& operator -- ()
            {
                this->r->dec_pos(this->p);
                return *this;
            }
            bidirectional_range_iterator operator -- (int)
            {
                bidirectional_range_iterator tmp = *this;
                --*this;
                return tmp;
            }
        };

        template <class Range>
        class random_access_range_iterator : public bidirectional_range_iterator<Range>
        {
        public:
            using typename bidirectional_range_iterator<Range>::difference_type;
            using typename bidirectional_range_iterator<Range>::reference;
            using typename bidirectional_range_iterator<Range>::position;

        public:
            using bidirectional_range_iterator<Range>::bidirectional_range_iterator;

            friend bool operator <  (const random_access_range_iterator& a, const random_access_range_iterator& b) noexcept
            {
                if (a.r == nullptr)
                    return b.r != nullptr;
                if (b.r == nullptr)
                    return false;

                assert(*a.r == *b.r);
                return a.r->distance(a.p, b.p) < 0;
            }
            friend bool operator >  (const random_access_range_iterator& a, const random_access_range_iterator& b) noexcept
            {
                return b < a;
            }
            friend bool operator <= (const random_access_range_iterator& a, const random_access_range_iterator& b) noexcept
            {
                return !(b < a);
            }
            friend bool operator >= (const random_access_range_iterator& a, const random_access_range_iterator& b) noexcept
            {
                return !(a < b);
            }

        public:
            random_access_range_iterator& operator += (difference_type n)
            {
                this->r->advance_pos(this->p, n);
                return *this;
            }
            random_access_range_iterator& operator -= (difference_type n)
            {
                this->r->advance_pos(this->p, -n);
                return *this;
            }

            reference operator [] (difference_type n) const
            {
                position pos = this->p;
                this->r->advance_pos(pos, n);
                return this->r->at_pos(pos);
            }

            friend random_access_range_iterator operator + (random_access_range_iterator it, difference_type n)
            {
                return it += n;
            }
            friend random_access_range_iterator operator + (difference_type n, random_access_range_iterator it)
            {
                return it += n;
            }
            friend random_access_range_iterator operator - (random_access_range_iterator it, difference_type n)
            {
                return it -= n;
            }
            friend difference_type operator - (const random_access_range_iterator& a, const random_access_range_iterator& b)
            {
                if (a.r == nullptr && b.r == nullptr)
                    return 0;

                assert(*a.r == *b.r);
                return a.r->distance(a.p, b.p);
            }
        };

        // Generator ranges
        template <class Generator, class TerminationPredicate>
        class single_pass_generator_range
        {
        public:
            using range_category = detail::generator_range_category_map_t<Generator>;
            using value_type = value_type<Generator, is_generator>;
            using position = position_type<Generator, is_generator>;
            using difference_type = difference_type<Generator, is_generator>;
            using reference = reference_type<Generator, is_generator>;
            using generator = Generator;

        public:
            single_pass_generator_range() : gen(nullptr), first(), term() { }
            single_pass_generator_range(const generator& g, position_type<generator, is_generator> p, const TerminationPredicate& term)
                : gen(&g), first(p), term(term) { }
            single_pass_generator_range(const generator& g, position_type<generator, is_generator> p, TerminationPredicate&& term)
                : gen(&g), first(p), term(::std::move(term)) { }

            friend bool operator == (const single_pass_generator_range& a, const single_pass_generator_range& b) noexcept
            {
                return (a.gen == b.gen || (a.gen != nullptr && b.gen != nullptr && *a.gen == *b.gen))
                    && a.first == b.first
                    && a.term == b.term;
            }
            friend bool operator != (const single_pass_generator_range& a, const single_pass_generator_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) noexcept(noexcept(first = pos)) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return gen == nullptr || term(gen.at_pos(pos)); }

            position& inc_pos(position& pos) const
            {
                return gen->inc_pos(pos);
            }

            reference at_pos(position pos) const
            {
                return gen->at_pos(pos);
            }

            const generator& base() const noexcept { return *gen; }

        private:
            const generator* gen;
            position first;
            TerminationPredicate term;
        };

        template <class Generator, class TerminationPredicate>
        class multi_pass_generator_range : public single_pass_generator_range<Generator, TerminationPredicate>
        {
        public:
            using single_pass_generator_range<Generator, TerminationPredicate>::single_pass_generator_range;
        };

        template <class Generator, class TerminationPredicate>
        class bidirectional_generator_range : public multi_pass_generator_range<Generator, TerminationPredicate>
        {
        public:
            using typename multi_pass_generator_range<Generator, TerminationPredicate>::position;
            using typename multi_pass_generator_range<Generator, TerminationPredicate>::difference_type;

        public:
            using multi_pass_generator_range<Generator, TerminationPredicate>::multi_pass_generator_range;

        public:
            position& dec_pos(position& pos, difference_type n) const
            {
                return this->base().dec_pos(pos);
            }
        };

        template <class Generator, class TerminationPredicate>
        class random_access_generator_range : public bidirectional_generator_range<Generator, TerminationPredicate>
        {
        public:
            using typename bidirectional_generator_range<Generator, TerminationPredicate>::position;
            using typename bidirectional_generator_range<Generator, TerminationPredicate>::difference_type;
            using typename bidirectional_generator_range<Generator, TerminationPredicate>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using bidirectional_generator_range<Generator, TerminationPredicate>::bidirectional_generator_range;

        public:
            position& advance_pos(position& pos, difference_type n) const
            {
                return this->base().advance_pos(pos, n);
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return this->base().distance(p1, p2);
            }

            reference operator [] (size_type n) const
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, n);
                return this->at_pos(pos);
            }
        };

        // Delimited generator ranges
        template <class Generator>
        class delimited_multi_pass_generator_range
        {
        public:
            using range_category = detail::generator_range_category_map_t<generator_category<Generator>>;
            using value_type = value_type<Generator, is_generator>;
            using position = position_type<Generator, is_generator>;
            using difference_type = difference_type<Generator, is_generator>;
            using reference = reference_type<Generator, is_generator>;
            using generator = Generator;

        public:
            delimited_multi_pass_generator_range() : gen(nullptr), first(), last() { }
            delimited_multi_pass_generator_range(const generator& g, position_type<generator, is_generator> f, position_type<generator, is_generator> l)
                : gen(&g), first(f), last(l) { }

            friend bool operator == (const delimited_multi_pass_generator_range& a, const delimited_multi_pass_generator_range& b) noexcept
            {
                return (a.gen == b.gen || (a.gen != nullptr && b.gen != nullptr && *a.gen == *b.gen))
                    && a.first == b.first
                    && a.last == b.last;
            }
            friend bool operator != (const delimited_multi_pass_generator_range& a, const delimited_multi_pass_generator_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) noexcept(noexcept(first = pos)) { first = pos; }

            position end_pos() const noexcept { return last; }
            void end_pos(position pos) noexcept(noexcept(last = pos)) { last = pos; }

            bool is_end_pos(position pos) const noexcept { return pos == last; }

            position& inc_pos(position& pos) const
            {
                return gen->inc_pos(pos);
            }

            reference at_pos(position pos) const
            {
                return gen->at_pos(pos);
            }

            const generator& base() const noexcept { return *gen; }

        private:
            const generator* gen;
            position first, last;
        };

        template <class Generator>
        class delimited_bidirectional_generator_range : public delimited_multi_pass_generator_range<Generator>
        {
        public:
            using typename delimited_multi_pass_generator_range<Generator>::position;
            using typename delimited_multi_pass_generator_range<Generator>::difference_type;

        public:
            using delimited_multi_pass_generator_range<Generator>::delimited_multi_pass_generator_range;

        public:
            position& dec_pos(position& pos) const { return this->base().dec_pos(pos); }
        };

        template <class Generator>
        class delimited_random_access_generator_range : public delimited_bidirectional_generator_range<Generator>
        {
        public:
            using typename delimited_bidirectional_generator_range<Generator>::position;
            using typename delimited_bidirectional_generator_range<Generator>::difference_type;
            using typename delimited_bidirectional_generator_range<Generator>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using delimited_bidirectional_generator_range<Generator>::delimited_bidirectional_generator_range;

        public:
            position& advance_pos(position& pos, difference_type n) const { return this->base().advance_pos(pos, n); }
            difference_type distance(position p1, position p2) const noexcept { return this->base().distance(p1, p2); }
            size_type size() const noexcept { return size_type(distance(this->begin_pos(), this->end_pos())); }
            void resize(size_type n)
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, n);
                this->end_pos(::std::move(pos));
            }

            reference operator [] (size_type n) const
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, n);
                return this->at_pos(pos, n);
            }

            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        // delegated ranges
        template <class Range, class TerminationPredicate>
        class delegated_single_pass_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using position = position_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

        public:
            delegated_single_pass_range() : r(nullptr), first(), term() { }
            delegated_single_pass_range(const range& r, position first, const TerminationPredicate& term)
                : r(&r), first(first), term(term) { }
            delegated_single_pass_range(const range& r, position first, TerminationPredicate&& term)
                : r(&r), first(first), term(::std::move(term)) { }

            friend bool operator == (const delegated_single_pass_range& a, const delegated_single_pass_range& b) noexcept
            {
                return (a.r == b.r || (a.r != nullptr && b.r != nullptr && *a.r == *b.r))
                    && a.first == b.first
                    && a.term == b.term;
            }
            friend bool operator != (const delegated_single_pass_range& a, const delegated_single_pass_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) { first = pos; }
            bool is_end_pos(position pos) const noexcept { return r != nullptr && term(r->at_pos(pos)); }
            position& inc_pos(position& pos) const { return r->inc_pos(pos); }
            reference at_pos(position pos) const { return r->at_pos(pos); }
            const range& base() const noexcept { return *r; }

        private:
            friend class delegated_multi_pass_range<Range, TerminationPredicate>;
            friend class delegated_bidirectional_range<Range, TerminationPredicate>;
            friend class delegated_random_access_range<Range, TerminationPredicate>;

            const range* r;
            position first;
            TerminationPredicate term;
        };

        template <class Range, class TerminationPredicate>
        class delegated_multi_pass_range : public delegated_single_pass_range<Range, TerminationPredicate>
        {
        public:
            using delegated_single_pass_range<Range, TerminationPredicate>::delegated_single_pass_range;
        };

        template <class Range, class TerminationPredicate>
        class delegated_bidirectional_range : public delegated_multi_pass_range<Range, TerminationPredicate>
        {
        public:
            using typename delegated_multi_pass_range<Range, TerminationPredicate>::position;
            using typename delegated_multi_pass_range<Range, TerminationPredicate>::difference_type;

        public:
            using delegated_multi_pass_range<Range, TerminationPredicate>::delegated_multi_pass_range;

        public:
            position& dec_pos(position& pos) const { return this->r->dec_pos(pos); }
        };

        template <class Range, class TerminationPredicate>
        class delegated_random_access_range : public delegated_bidirectional_range<Range, TerminationPredicate>
        {
        public:
            using typename delegated_bidirectional_range<Range, TerminationPredicate>::position;
            using typename delegated_bidirectional_range<Range, TerminationPredicate>::difference_type;
            using typename delegated_bidirectional_range<Range, TerminationPredicate>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using delegated_bidirectional_range<Range, TerminationPredicate>::delegated_bidirectional_range;

        public:
            position& advance_pos(position& pos, difference_type n) const { return this->r->advance_pos(pos, n); }
            difference_type distance(position p1, position p2) const noexcept { return this->r->distance(p1, p2); }

            reference operator [] (size_type n) const
            {
                position pos = this->first;
                this->r->advance_pos(pos, n);
                return this->r->at_pos(n);
            }
        };

        // delimited ranges
        template <class Range>
        class delimited_multi_pass_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using position = position_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

        public:
            delimited_multi_pass_range() : r(nullptr), first(), last() { }
            delimited_multi_pass_range(const range& r, position first, position last)
                : r(&r), first(first), last(last) { }

            friend bool operator == (const delimited_multi_pass_range& a, const delimited_multi_pass_range& b) noexcept
            {
                return (a.r == b.r || (a.r != nullptr && b.r !== nullptr && *a.r == *b.r))
                    && a.first == b.first
                    && a.last == b.last;
            }
            friend bool operator != (const delimited_multi_pass_range& a, const delimited_multi_pass_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) { first = pos; }

            position end_pos() const noexcept { return last; }
            void end_pos(position pos) { last = pos; }

            bool is_end_pos(position pos) const noexcept { return pos == last; }

            position& inc_pos(position& pos) const { return r->inc_pos(pos); }
            reference at_pos(position pos) const { return r->at_pos(pos); }

            const range& base() const noexcept { return *r; }

        private:
            friend class delimited_bidirectional_range<Range>;
            friend class delimited_random_access_range<Range>;

            range* r;
            position first, last;
        };

        template <class Range>
        class delimited_bidirectional_range : public delimited_multi_pass_range<Range>
        {
        public:
            using typename delimited_multi_pass_range<Range>::position;
            using typename delimited_multi_pass_range<Range>::difference_type;

        public:
            using delimited_multi_pass_range<Range>::delimited_multi_pass_range;

        public:
            position& dec_pos(position& pos) const { return this->r->dec_pos(pos); }
        };

        template <class Range>
        class delimited_random_access_range : public delimited_bidirectional_range<Range>
        {
        public:
            using typename delimited_bidirectional_range<Range>::position;
            using typename delimited_bidirectional_range<Range>::difference_type;
            using typename delimited_bidirectional_range<Range>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using delimited_bidirectional_range<Range>::delimited_bidirectional_range;

        public:
            position& advance_pos(position& pos, difference_type n) const { return this->r->advance_pos(pos, n); }
            difference_type distance(position p1, position p2) const noexcept { return this->r->distance(p1, p2); }
            size_type size() const noexcept { return size_type(this->r->distance(this->first, this->last)); }
            void resize(size_type n)
            {
                this->last = this->first;
                this->r->advance_pos(last, n);
            }

            reference operator [] (size_type n) const
            {
                position pos = this->first;
                this->r->advance_pos(pos, n);
                return this->r->at_pos(n);
            }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        // counted ranges
        template <class Range>
        class counted_single_pass_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<position_type<Range, is_range>, size_type>;

        public:
            counted_single_pass_range() : underlying(nullptr), first(), count(0) { }
            counted_single_pass_range(const range& r, position_type<Range, is_range> pos, size_type count) : underlying(&r), first(pos, 0), count(count) { }

            friend bool operator == (const counted_single_pass_range& a, const counted_single_pass_range& b) noexcept
            {
                return (a.underlying == b.underlying || (a.underlying != nullptr && b.underlying != nullptr && *a.underlying == *b.underlying))
                    && a.count == b.count;
            }
            friend bool operator != (const counted_single_pass_range& a, const counted_single_pass_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) noexcept(noexcept(first = pos)) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return pos.second() == count; }

            position& inc_pos(position& pos) const
            {
                underlying->inc_pos(pos.first());
                ++pos.second();
                return pos;
            }

            reference at_pos(position pos) const
            {
                return underlying->at_pos(pos.first());
            }

            const range& base() const noexcept { return underlying; }
            position_type<range, is_range> base_pos(position pos) { return pos.first(); }

            size_type size() const noexcept { return count - first.second(); }
            void resize(size_type n) noexcept { count = first.second() + n; }

        private:
            const range* underlying;
            position first;
            size_type count;
        };

        template <class Range>
        class counted_multi_pass_range : public counted_single_pass_range<Range>
        {
        public:
            using counted_single_pass_range<Range>::counted_single_pass_range;
        };

        template <class Range>
        class counted_bidirectional_range : public counted_multi_pass_range<Range>
        {
        public:
            using typename counted_multi_pass_range<Range>::position;
            using typename counted_multi_pass_range<Range>::difference_type;

        public:
            using counted_multi_pass_range<Range>::counted_multi_pass_range;

        public:
            position& dec_pos(position& pos) const
            {
                this->base().dec_pos(pos.first());
                --pos.second();
                return pos;
            }
        };

        template <class Range>
        class counted_random_access_range : public counted_bidirectional_range<Range>
        {
        public:
            using typename counted_bidirectional_range<Range>::position;
            using typename counted_bidirectional_range<Range>::difference_type;
            using typename counted_bidirectional_range<Range>::reference;
            using typename counted_bidirectional_range<Range>::size_type;

        public:
            using counted_bidirectional_range<Range>::counted_bidirectional_range;

        public:
            position end_pos() const
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, this->size());
                return pos;
            }
            void end_pos(position pos) noexcept
            {
                this->resize(pos.second() - this->begin_pos().second());
            }

            position& advance_pos(position& pos, difference_type n) const
            {
                this->base().advance_pos(pos.first(), n);
                pos.second() += n;
                return pos;
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return p2.second() - p1.second();
            }

            reference operator [] (size_type n) const
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, n);
                this->at_pos(pos);
            }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        // counted generator ranges
        template <class Generator>
        class counted_single_pass_generator_range
        {
        public:
            using range_category = detail::generator_range_category_map_t<generator_category<Generator>>;
            using value_type = value_type<Generator, is_generator>;
            using difference_type = difference_type<Generator, is_generator>;
            using reference = reference_type<Generator, is_generator>;
            using generator = Generator;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<position_type<Generator, is_generator>, size_type>;

        public:
            counted_single_pass_generator_range() : gen(nullptr), first(), count(0) { }
            counted_single_pass_generator_range(const generator& g, position_type<Generator, is_generator> pos, size_type count)
                : gen(&g), first(pos, 0), count(count) { }

            friend bool operator == (const counted_single_pass_generator_range& a, const counted_single_pass_generator_range& b) noexcept
            {
                return (a.gen == b.gen || (a.gen != nullptr && b.gen != nullptr && *a.gen == *b.gen))
                    && a.first == b.first
                    && a.count == b.count;
            }
            friend bool operator != (const counted_single_pass_generator_range& a, const counted_single_pass_generator_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) noexcept(noexcept(first = pos)) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return pos.second() == count; }

            position& inc_pos(position& pos) const
            {
                gen->inc_pos(pos.first());
                ++pos.second();
                return pos;
            }

            reference at_pos(position pos) const
            {
                return gen->at_pos(pos.first());
            }

            size_type size() const noexcept { return count = first.second(); }
            void resize(size_type n) noexcept { count = first.second() + n; }

            const generator& base() const noexcept { return *gen; }

        private:
            const generator* gen;
            position first;
            size_type count;
        };

        template <class Generator>
        class counted_multi_pass_generator_range : public counted_single_pass_generator_range<Generator>
        {
        public:
            using counted_single_pass_generator_range<Generator>::counted_single_pass_generator_range;
        };

        template <class Generator>
        class counted_bidirectional_generator_range : public counted_multi_pass_generator_range<Generator>
        {
        public:
            using typename counted_multi_pass_generator_range<Generator>::position;
            using typename counted_multi_pass_generator_range<Generator>::difference_type;

        public:
            using counted_multi_pass_generator_range<Generator>::counted_multi_pass_generator_range;

        public:
            position& dec_pos(position& pos) const
            {
                this->base().dec_pos(pos.first());
                --pos.second();
                return pos;
            }
        };

        template <class Generator>
        class counted_random_access_generator_range : public counted_bidirectional_generator_range<Generator>
        {
        public:
            using typename counted_bidirectional_generator_range<Generator>::position;
            using typename counted_bidirectional_generator_range<Generator>::difference_type;
            using typename counted_bidirectional_generator_range<Generator>::reference;
            using typename counted_bidirectional_generator_range<Generator>::size_type;

        public:
            using counted_bidirectional_generator_range<Generator>::counted_bidirectional_generator_range;

        public:
            position end_pos() const
            {
                position pos = this->begin_pos();
                this->advance_pos(pos, this->size());
                return pos;
            }
            void end_pos(position pos) noexcept
            {
                this->resize(pos.second() - this->begin_pos().second());
            }

            position& advance_pos(position& pos, difference_type n) const
            {
                this->base().advance_pos(pos.first(), n);
                pos.second() += n;
                return pos;
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return difference_type(p2.second() - p1.second());
            }

            reference operator [] (size_type n) const
            {
                auto pos = this->begin_pos().first();
                this->base().advance_pos(pos, n);
                return this->base().at_pos(pos);
            }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        // reverse ranges
        template <class Range, class TerminationPredicate>
        class reverse_bidirectional_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using position = position_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

        public:
            reverse_bidirectional_range() : r(nullptr), first(), term() { }
            reverse_bidirectional_range(const Range& range, position_type<Range, is_range> first, const TerminationPredicate& term)
                : r(&range), first(last), term(term) { }
            reverse_bidirectional_range(const Range& range, position_type<Range, is_range> first, TerminationPredicate&& term)
                : r(&range), first(last), term(::std::move(term)) { }

            friend bool operator == (const reverse_bidirectional_range& a, const reverse_bidirectional_range& b) noexcept
            {
                return (a.r == b.r || (a.r != nullptr && b.r != nullptr && *a.r == *b.r))
                    && a.first == b.first
                    && a.term == b.term;
            }
            friend bool operator != (const reverse_bidirectional_range& a, const reverse_bidirectional_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return r != nullptr && term(r->at_pos(pos)); }

            position& inc_pos(position& pos) const { return r->dec_pos(pos); }
            position& dec_pos(position& pos) const { return r->inc_pos(pos); }
            reference at_pos(position pos) const { return r->at_pos(prev_pos(*r, pos)); }

            const range& base() { return *r; }

        private:
            friend class reverse_random_access_range<Range, TerminationPredicate>;

            const range* r;
            position first;
            TerminationPredicate term;
        };

        template <class Range, class TerminationPredicate>
        class reverse_random_access_range : public reverse_bidirectional_range<Range, TerminationPredicate>
        {
        public:
            using position = typename reverse_bidirectional_range<Range, TerminationPredicate>::position;
            using difference_type = typename reverse_bidirectional_range<Range, TerminationPredicate>::difference_type;
            using reference = typename reverse_bidirectional_range<Range, TerminationPredicate>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using reverse_bidirectional_range<Range, TerminationPredicate>::reverse_bidirectional_range;

        public:
            position& advance_pos(position& pos, difference_type n) const
            {
                return r->advance_pos(pos, -n);
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return this->r->distance(p2, p1);
            }

            reference operator [] (size_type n)
            {
                auto pos = this->begin_pos();
                this->advance_pos(pos, n);
                return this->at_pos(pos);
            }
            reference at(size_type n)
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        template <class Range>
        class delimited_reverse_bidirectional_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using position = position_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

        public:
            delimited_reverse_bidirectional_range() : r(nullptr), first(), last() { }
            delimited_reverse_bidirectional_range(const Range& range, position_type<Range, is_range> first, position_type<Range, is_range> last)
                : r(&range), first(first), last(last) { }

            friend bool operator == (const delimited_reverse_bidirectional_range& a, const delimited_reverse_bidirectional_range& b) noexcept
            {
                return (a.r == b.r || (a.r != nullptr && b.r != nullptr && *a.r == *b.r))
                    && a.first == b.first
                    && a.last == b.last;
            }
            friend bool operator != (const delimited_reverse_bidirectional_range& a, const delimited_reverse_bidirectional_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) { first = pos; }

            position end_pos() const noexcept { return last; }
            void end_pos(position pos) { last = pos; }

            bool is_end_pos(position pos) const noexcept { return pos == last; }

            position& inc_pos(position& pos) const { return r->dec_pos(pos); }
            position& dec_pos(position& pos) const { return r->inc_pos(pos); }
            reference at_pos(position pos) const { return r->at_pos(prev_pos(*r, pos)); }

            const range& base() { return *r; }

        private:
            friend class delimited_reverse_random_access_range<Range>;

            const range* r;
            position first, last;
        };

        template <class Range>
        class delimited_reverse_random_access_range : public delimited_reverse_bidirectional_range<Range>
        {
        public:
            using position = typename delimited_reverse_bidirectional_range<Range>::position;
            using difference_type = typename delimited_reverse_bidirectional_range<Range>::difference_type;
            using reference = typename delimited_reverse_bidirectional_range<Range>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using delimited_reverse_bidirectional_range<Range>::delimited_reverse_bidirectional_range;

        public:
            position& advance_pos(position& pos, difference_type n) const
            {
                return r->advance_pos(pos, -n);
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return this->r->distance(p2, p1);
            }

            size_type size() const noexcept
            {
                return size_type(distance(this->first, this->last));
            }

            void resize(size_type n)
            {
                this->last = this->first;
                advance_pos(this->last, n);
            }

            reference operator [] (size_type n)
            {
                auto pos = this->begin_pos();
                this->advance_pos(pos, n);
                return this->at_pos(pos);
            }
            reference at(size_type n)
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };

        template <class Range>
        class counted_reverse_bidirectional_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<position_type<Range, is_range>, size_type>;

        public:
            counted_reverse_bidirectional_range() : r(nullptr), first(), count(0) { }
            counted_reverse_bidirectional_range(const Range& range, position_type<Range, is_range> first, size_type count)
                : r(&range), first(first, 0), count(count) { }

            friend bool operator == (const counted_reverse_bidirectional_range& a, const counted_reverse_bidirectional_range& b) noexcept
            {
                return (a.r == b.r || (a.r != nullptr && b.r != nullptr && *a.r == *b.r))
                    && a.first == b.first
                    && a.count == b.count;
            }
            friend bool operator != (const counted_reverse_bidirectional_range& a, const counted_reverse_bidirectional_range& b) noexcept
            {
                return !(a == b);
            }

        public:
            position begin_pos() const noexcept { return first; }
            void begin_pos(position pos) { first = pos; }

            bool is_end_pos(position pos) const noexcept { return pos.second() == count; }

            position& inc_pos(position& pos) const
            {
                r->dec_pos(pos.first());
                ++pos.second();
                return pos;
            }

            position& dec_pos(position& pos) const
            {
                r->inc_pos(pos.first());
                --pos.second();
                return pos;
            }

            reference at_pos(position pos) const { return r->at_pos(prev_pos(*r, pos.first())); }

            size_type size() const noexcept { return count - first.second(); }
            void resize(size_type n) { count = first.second() + n; }

            const range& base() const noexcept { return *r; }
            position_type<Range, is_range> base_pos(position pos) const { return pos.second(); }

        private:
            friend class delimited_reverse_random_access_range<Range>;

            const range* r;
            position first;
            size_type count;
        };

        template <class Range>
        class counted_reverse_random_access_range : public counted_reverse_bidirectional_range<Range>
        {
        public:
            using position = typename counted_reverse_bidirectional_range<Range>::position;
            using difference_type = typename counted_reverse_bidirectional_range<Range>::difference_type;
            using reference = typename counted_reverse_bidirectional_range<Range>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using counted_reverse_bidirectional_range<Range>::counted_reverse_bidirectional_range;

        public:
            position& advance_pos(position& pos, difference_type n) const
            {
                r->advance_pos(pos.first(), -n);
                pos.second() += n;
                return pos;
            }

            difference_type distance(position p1, position p2) const noexcept
            {
                return p2.second() - p1.second();
            }

            position end_pos() const
            {
                auto pos = this->begin_pos();
                advance_pos(pos, size());
                return pos;
            }

            void end_pos(position pos)
            {
                this->resize(pos.second() - this->begin_pos().second())
            }

            reference operator [] (size_type n)
            {
                auto pos = this->begin_pos();
                advance_pos(pos, n);
                return this->at_pos(pos);
            }
            reference at(size_type n)
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return (*this)[n];
            }
        };
    }
}

#endif