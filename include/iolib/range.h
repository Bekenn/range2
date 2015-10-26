//
//  range.h
//  iolib
//
//  Created by James Touton on 10/19/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_RANGE_INCLUDED
#define IOLIB_RANGE_INCLUDED
#pragma once

#include <iolib/concept.h>

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace iolib
{
    // range categories
    struct single_pass_range_tag { };
    struct multi_pass_range_tag { };
    struct bidirectional_range_tag { };
    struct random_access_range_tag { };

    // range traits
    template <class Range> using range_category = typename Range::range_category;
    template <class Range> using value_type = typename Range::value_type;
    template <class Range> using position_type = typename Range::position;
    template <class Range> using difference_type = typename Range::difference_type;
    template <class Range> using pointer_type = typename Range::pointer;
    template <class Range> using reference_type = typename Range::reference;

    // range concepts
    template <class Range> constexpr auto is_single_pass_range =
        ::std::is_base_of<single_pass_range_tag, range_category<Range>>::value;
    template <class Range> constexpr auto is_multi_pass_range =
        ::std::is_base_of<multi_pass_range_tag, range_category<Range>>::value;
    template <class Range> constexpr auto is_bidirectional_range =
        ::std::is_base_of<bidirectional_range_tag, range_category<Range>>::value;
    template <class Range> constexpr auto is_random_access_range =
        ::std::is_base_of<random_access_range_tag, range_category<Range>>::value;

    // basic range
    template <class Category, class T, class Position, class Distance, class Pointer, class Reference>
    struct range
    {
        using range_category = Category;
        using value_type = T;
        using position = Position;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };

    // range utilities
    template <class Range, REQUIRES(is_single_pass_range<Range>)>
    auto next_pos(const Range& range, position_type<Range> pos)
    {
        return range.advance_pos(pos);
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>)>
    auto prev_pos(const Range& range, position_type<Range> pos)
    {
        return range.advance_pos(pos, -1);
    }

    template <class Range, REQUIRES(is_single_pass_range<Range>)>
    auto set_range(Range range, const position_type<Range>& first, const position_type<Range>& last) noexcept
    {
        range.begin(first);
        range.end(last);
        return range;
    }

    template <class Range, REQUIRES(is_single_pass_range<Range>)>
    Range& drop_first(Range& range, ::std::make_unsigned_t<difference_type<Range>> n = 1)
    {
        auto pos = range.begin_pos();
        range.advance_pos(pos, n);
        range.begin_pos(::std::move(pos));
        return range;
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>)>
    Range& drop_last(Range& range, ::std::make_unsigned_t<difference_type<Range>> n = 1)
    {
        auto pos = range.end_pos();
        range.advance_pos(pos, -difference_type<Range>(n));
        range.end_pos(::std::move(pos));
        return range;
    }

    // reverse ranges
    template <class Range>
    class bidirectional_reverse_range
    {
    public:
        using range_category = range_category<Range>;
        using value_type = value_type<Range>;
        using position = position_type<Range>;
        using difference_type = difference_type<Range>;
        using pointer = pointer_type<Range>;
        using reference = reference_type<Range>;

    public:
        bidirectional_reverse_range() = default;
        bidirectional_reverse_range(const Range& r) noexcept(noexcept(Range(r))) : underlying(r) { }

    public:
        friend auto operator == (const bidirectional_reverse_range& a, const bidirectional_reverse_range& b) noexcept
        {
            return a.underlying == b.underlying;
        }
        friend auto operator != (const bidirectional_reverse_range& a, const bidirectional_reverse_range& b) noexcept
        {
            return !(a == b);
        }

        auto begin_pos() const noexcept { return underlying.end_pos(); }
        void begin_pos(const position& pos) noexcept { underlying.end_pos(pos); }
        void begin_pos(position&& pos) noexcept { underlying.end_pos(::std::move(pos)); }

        auto end_pos() const noexcept { return underlying.begin_pos(); }
        void end_pos(const position& pos) noexcept { underlying.begin_pos(pos); }
        void end_pos(position&& pos) noexcept { underlying.begin_pos(::std::move(pos)); }

        auto empty() const noexcept { return underlying.empty(); }

        decltype(auto) advance_pos(position& pos, difference_type n = 1) const
        {
            return underlying.advance_pos(pos, -n);
        }
        decltype(auto) at_pos(const position& pos) const
        {
            return underlying.at_pos(prev_pos(underlying, pos));
        }

        decltype(auto) front() const { return underlying.back(); }
        decltype(auto) back() const { return underlying.front(); }

    protected:
        Range underlying;
    };

    template <class Range>
    class random_access_reverse_range : public bidirectional_reverse_range<Range>
    {
        using bidirectional_reverse_range<Range>::underlying;

    public:
        using difference_type = typename bidirectional_reverse_range<Range>::difference_type;
        using size_type = ::std::make_unsigned_t<difference_type>;

    public:
        using bidirectional_reverse_range<Range>::bidirectional_reverse_range;

    public:
        friend auto operator <  (const random_access_reverse_range& a, const random_access_reverse_range& b) noexcept
        {
            return b.underlying < a.underlying;
        }
        friend auto operator >  (const random_access_reverse_range& a, const random_access_reverse_range& b) noexcept
        {
            return b < a;
        }
        friend auto operator <= (const random_access_reverse_range& a, const random_access_reverse_range& b) noexcept
        {
            return b.underlying <= a.underlying;
        }
        friend auto operator >= (const random_access_reverse_range& a, const random_access_reverse_range& b) noexcept
        {
            return b <= a;
        }

        auto size() const noexcept { return underlying.size(); }

        decltype(auto) operator [] (size_type n) const
        {
            return underlying[underlying.size() - n - 1];
        }
        decltype(auto) at(size_type n) const
        {
            if (n >= underlying.size())
                throw ::std::out_of_range("element index out of range");
            return underlying[underlying.size() - n - 1];
        }
    };

    template <class Range> using reverse_range = ::std::conditional_t<
        ::std::is_base_of<random_access_range_tag, range_category<Range>>::value,
            random_access_reverse_range<Range>,
            ::std::enable_if_t<::std::is_base_of<bidirectional_range_tag, range_category<Range>>::value,
                bidirectional_reverse_range<Range>
            >
        >;

    namespace detail
    {
        template <class IteratorCategory> struct category_map;
        template <> struct category_map<::std::input_iterator_tag>
        {
            using type = single_pass_range_tag;
        };
        template <> struct category_map<::std::forward_iterator_tag>
        {
            using type = multi_pass_range_tag;
        };
        template <> struct category_map<::std::bidirectional_iterator_tag>
        {
            using type = bidirectional_range_tag;
        };
        template <> struct category_map<::std::random_access_iterator_tag>
        {
            using type = random_access_range_tag;
        };
        template <class IteratorCategory> using category_map_t = typename category_map<IteratorCategory>::type;
    }

    template <class Iterator>
    class input_iterator_range
    {
    public:
        using range_category = detail::category_map_t<typename ::std::iterator_traits<Iterator>::iterator_category>;
        using value_type = typename ::std::iterator_traits<Iterator>::value_type;
        using position = Iterator;
        using difference_type = typename ::std::iterator_traits<Iterator>::difference_type;
        using pointer = typename ::std::iterator_traits<Iterator>::pointer;
        using reference = typename ::std::iterator_traits<Iterator>::reference;
        using iterator = Iterator;

    public:
        input_iterator_range() noexcept : first(), last() { }
        input_iterator_range(const iterator& first, const iterator& last) noexcept
            : first(first), last(last) { }
        input_iterator_range(iterator&& first, iterator&& last) noexcept
            : first(::std::move(first)), last(::std::move(last)) { }

    public:
        friend auto operator == (const input_iterator_range& a, const input_iterator_range& b) noexcept
        {
            return a.first == b.first && a.last == b.last;
        }
        friend auto operator != (const input_iterator_range& a, const input_iterator_range& b) noexcept
        {
            return !(a == b);
        }

        auto begin_pos() const noexcept { return first; }
        void begin_pos(const position& pos) noexcept { first = pos; }
        void begin_pos(position&& pos) noexcept { first = ::std::move(pos); }

        auto end_pos() const noexcept { return last; }

        decltype(auto) advance_pos(position& pos) const
        {
            return ++pos;
        }

        decltype(auto) at_pos(const position& pos) const
        {
            return *pos;
        }

        auto empty() const noexcept { return first == last; }

        decltype(auto) front() const
        {
            return *first;
        }

        auto begin() const noexcept { return first; }
        auto end() const noexcept { return last; }

    private:
        position first;
        position last;
    };

    template <class Iterator>
    class forward_iterator_range : public input_iterator_range<Iterator>
    {
    public:
        using typename input_iterator_range<Iterator>::position;

    public:
        using input_iterator_range<Iterator>::input_iterator_range;

    public:
        using input_iterator_range<Iterator>::end_pos;
        void end_pos(const position& pos)
            noexcept(noexcept(this->last = pos))
        {
            this->last = pos;
        }
        void end_pos(position&& pos)
            noexcept(noexcept(this->last = ::std::move(pos)))
        {
            this->last = ::std::move(pos);
        }
    };

    template <class Iterator>
    class bidirectional_iterator_range : public forward_iterator_range<Iterator>
    {
    public:
        using typename forward_iterator_range<Iterator>::position;
        using typename forward_iterator_range<Iterator>::difference_type;
        using reverse_iterator = ::std::reverse_iterator<Iterator>;

    public:
        using forward_iterator_range<Iterator>::forward_iterator_range;

    public:
        using forward_iterator_range<Iterator>::advance_pos;
        decltype(auto) advance_pos(position& pos, difference_type n)
        {
            ::std::advance(pos, n);
            return pos;
        }

        auto rbegin() const noexcept(noexcept(reverse_iterator(this->last)))
            { return reverse_iterator(this->last); }
        auto rend() const noexcept(noexcept(reverse_iterator(this->first)))
            { return reverse_iterator(this->first); }

        decltype(auto) back() const { return *::std::prev(this->last); }
    };

    template <class Iterator>
    class random_access_iterator_range : public bidirectional_iterator_range<Iterator>
    {
    public:
        using difference_type = typename bidirectional_iterator_range<Iterator>::difference_type;
        using size_type = ::std::make_unsigned_t<difference_type>;

    public:
        using bidirectional_iterator_range<Iterator>::bidirectional_iterator_range;

    public:
        friend auto operator <  (const random_access_iterator_range& a, const random_access_iterator_range& b) noexcept
        {
            if (a.first > b.first)
                return false;
            if (a.first == b.first)
                return a.last < b.last;
            return true;
        }
        friend auto operator >  (const random_access_iterator_range& a, const random_access_iterator_range& b) noexcept
        {
            return b < a;
        }
        friend auto operator <= (const random_access_iterator_range& a, const random_access_iterator_range& b) noexcept
        {
            return !(a > b);
        }
        friend auto operator >= (const random_access_iterator_range& a, const random_access_iterator_range& b) noexcept
        {
            return !(a < b);
        }

        auto size() const noexcept { return size_type(::std::distance(this->first, this->last)); }

        decltype(auto) operator [] (size_type n) const { return this->first[n]; }
        decltype(auto) at(size_type n) const
        {
            if (n >= size())
                throw ::std::out_of_range("element index out of range");
            return this->first[n];
        }
    };

    template <class Iterator> using iterator_range = ::std::conditional_t<
        ::std::is_base_of<::std::random_access_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
            random_access_iterator_range<Iterator>,
            ::std::conditional_t<::std::is_base_of<::std::bidirectional_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
                bidirectional_iterator_range<Iterator>,
                ::std::conditional_t<::std::is_base_of<::std::forward_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
                    forward_iterator_range<Iterator>,
                    ::std::enable_if_t<::std::is_base_of<::std::input_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
                        input_iterator_range<Iterator>
                    >
                >
            >
        >;

}

#endif
