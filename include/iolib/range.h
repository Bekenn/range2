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

#include "generator.h"
#include "utility.h"

#include <stdexcept>


namespace iolib
{
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
        template <class T>
        struct check_end_pos
        {
            static constexpr bool value = check_end_pos_getter<T>::value
                && check_end_pos_setter<T>::value;
        };
    }
    template <class T> struct is_delimited_range
        : ::std::conditional_t<is_range<T>::value && detail::check_end_pos<T>::value,
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
        struct pointer_type_of<Range, is_range> { using type = typename Range::pointer; };
        template <class Range>
        struct reference_type_of<Range, is_range> { using type = typename Range::reference; };
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

    // basic range
    template <class Category, class T, class Position, class Distance = ::std::ptrdiff_t, class Pointer = T*, class Reference = T&>
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
    template <class Range, REQUIRES(is_single_pass_range<Range>::value)>
    auto next_pos(const Range& range, position_type<Range, is_range> pos)
    {
        return range.advance_pos(pos);
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value)>
    auto prev_pos(const Range& range, position_type<Range, is_range> pos)
    {
        return range.advance_pos(pos, -1);
    }

    template <class Range, REQUIRES(is_delimited_range<Range>::value)>
    auto set_range(Range range, const position_type<Range, is_range>& first, const position_type<Range, is_range>& last) noexcept
    {
        range.begin_pos(first);
        range.end_pos(last);
        return range;
    }

    template <class Range, REQUIRES(is_single_pass_range<Range>::value)>
    Range& drop_first(Range& range, ::std::make_unsigned_t<difference_type<Range, is_range>> n = 1)
    {
        auto pos = range.begin_pos();
        range.advance_pos(pos, n);
        range.begin_pos(::std::move(pos));
        return range;
    }

    template <class Range, REQUIRES(is_bidirectional_range<Range>::value)>
    Range& drop_last(Range& range, ::std::make_unsigned_t<difference_type<Range, is_range>> n = 1)
    {
        auto pos = range.end_pos();
        range.advance_pos(pos, -difference_type<Range, is_range>(n));
        range.end_pos(::std::move(pos));
        return range;
    }

    namespace detail
    {
        template <class Iterator> class input_iterator_range;
        template <class Iterator> class forward_iterator_range;
        template <class Iterator> class bidirectional_iterator_range;
        template <class Iterator> class random_access_iterator_range;

        template <class IteratorCategory> struct iterator_range_category_map;
        template <> struct iterator_range_category_map<::std::input_iterator_tag> { using type = single_pass_range_tag; };
        template <> struct iterator_range_category_map<::std::forward_iterator_tag> { using type = multi_pass_range_tag; };
        template <> struct iterator_range_category_map<::std::bidirectional_iterator_tag> { using type = bidirectional_range_tag; };
        template <> struct iterator_range_category_map<::std::random_access_iterator_tag> { using type = random_access_range_tag; };
        template <class IteratorCategory> using iterator_range_category_map_t = typename iterator_range_category_map<IteratorCategory>::type;
    }

    template <class Iterator> using iterator_range =
        ::std::conditional_t<is_random_access_iterator<Iterator>::value, detail::random_access_iterator_range<Iterator>,
            ::std::conditional_t<is_bidirectional_iterator<Iterator>::value, detail::bidirectional_iterator_range<Iterator>,
                ::std::conditional_t<is_forward_iterator<Iterator>::value, detail::forward_iterator_range<Iterator>,
                    ::std::enable_if_t<is_input_iterator<Iterator>::value, detail::input_iterator_range<Iterator>>
                >
            >
        >;

    namespace detail
    {
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
    }

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

    namespace detail
    {
        template <class Range> class counted_single_pass_range;
        template <class Range> class counted_multi_pass_range;
        template <class Range> class counted_bidirectional_range;
        template <class Range> class counted_random_access_range;

        template <class Iterator> class counted_input_iterator_range;
        template <class Iterator> class counted_forward_iterator_range;
        template <class Iterator> class counted_bidirectional_iterator_range;
        template <class Iterator> class counted_random_access_iterator_range;

        template <class Generator> class counted_single_pass_generator_range;
        template <class Generator> class counted_multi_pass_generator_range;
        template <class Generator> class counted_bidirectional_generator_range;
        template <class Generator> class counted_random_access_generator_range;
    }

    template <class Range> using counted_range =
        ::std::conditional_t<is_random_access_range<Range>::value, detail::counted_random_access_range<Range>,
            ::std::conditional_t<is_bidirectional_range<Range>::value, detail::counted_bidirectional_range<Range>,
                ::std::conditional_t<is_multi_pass_range<Range>::value, detail::counted_multi_pass_range<Range>,
                    ::std::enable_if_t<is_single_pass_range<Range>::value, detail::counted_single_pass_range<Range>>
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

    template <class Generator> using counted_generator_range =
        ::std::conditional_t<is_random_access_generator<Generator>::value, detail::counted_random_access_generator_range<Generator>,
            ::std::conditional_t<is_bidirectional_generator<Generator>::value, detail::counted_bidirectional_generator_range<Generator>,
                ::std::conditional_t<is_multi_pass_generator<Generator>::value, detail::counted_multi_pass_generator_range<Generator>,
                    ::std::enable_if_t<is_single_pass_generator<Generator>::value, detail::counted_single_pass_generator_range<Generator>>
                >
            >
        >;

    namespace detail
    {
        template <class Range> class bidirectional_reverse_range;
        template <class Range> class random_access_reverse_range;
    }

    template <class Range> using reverse_range =
        ::std::enable_if_t<is_delimited_range<Range>::value,
            ::std::conditional_t<is_random_access_range<Range>::value, detail::random_access_reverse_range<Range>,
                ::std::enable_if_t<::std::is_base_of<bidirectional_range_tag, range_category<Range>>::value, detail::bidirectional_reverse_range<Range>>
            >
        >;

    namespace detail
    {
        // Iterator ranges
        template <class Iterator>
        class input_iterator_range
        {
        public:
            using range_category = detail::iterator_range_category_map_t<iterator_category<Iterator>>;
            using value_type = value_type<Iterator, is_iterator>;
            using position = Iterator;
            using difference_type = difference_type<Iterator, is_iterator>;
            using pointer = pointer_type<Iterator, is_iterator>;
            using reference = reference_type<Iterator, is_iterator>;
            using iterator = Iterator;

        public:
            input_iterator_range() noexcept : first(), last() { }
            input_iterator_range(const iterator& first, const iterator& last) noexcept(noexcept(iterator(first)))
                : first(first), last(last) { }
            input_iterator_range(iterator&& first, iterator&& last) noexcept(noexcept(iterator(::std::move(first))))
                : first(::std::move(first)), last(::std::move(last)) { }

        public:
            friend bool operator == (const input_iterator_range& a, const input_iterator_range& b) noexcept
            {
                return a.first == b.first && a.last == b.last;
            }
            friend bool operator != (const input_iterator_range& a, const input_iterator_range& b) noexcept
            {
                return !(a == b);
            }

            const position& begin_pos() const noexcept { return first; }
            void begin_pos(const position& pos) noexcept(noexcept(first = pos)) { first = pos; }
            void begin_pos(position&& pos) noexcept(noexcept(first = ::std::move(pos))) { first = ::std::move(pos); }

            const position& end_pos() const noexcept { return last; }
            void end_pos(const position& pos) noexcept(noexcept(last = pos)) { last = pos; }
            void end_pos(position&& pos) noexcept(noexcept(last = ::std::move(pos))) { last = ::std::move(pos); }

            bool is_end_pos(const position& pos) const noexcept { return pos == last; }

            position& advance_pos(position& pos) const { return ++pos; }
            reference at_pos(const position& pos) const { return *pos; }

            bool empty() const noexcept { return first == last; }
            reference front() const { return *first; }

            iterator begin() const noexcept { return first; }
            iterator end() const noexcept { return last; }

        private:
            position first;
            position last;
        };

        template <class Iterator>
        class forward_iterator_range : public input_iterator_range<Iterator>
        {
        public:
            using input_iterator_range<Iterator>::input_iterator_range;
        };

        template <class Iterator>
        class bidirectional_iterator_range : public forward_iterator_range<Iterator>
        {
        public:
            using typename forward_iterator_range<Iterator>::position;
            using typename forward_iterator_range<Iterator>::difference_type;
            using typename forward_iterator_range<Iterator>::reference;
            using reverse_iterator = ::std::reverse_iterator<Iterator>;

        public:
            using forward_iterator_range<Iterator>::forward_iterator_range;

        public:
            using forward_iterator_range<Iterator>::advance_pos;
            position& advance_pos(position& pos, difference_type n) const
            {
                ::std::advance(pos, n);
                return pos;
            }

            reverse_iterator rbegin() const noexcept { return reverse_iterator(this->end()); }
            reverse_iterator rend() const noexcept { return reverse_iterator(this->begin()); }

            reference back() const { return *::std::prev(this->end()); }
        };

        template <class Iterator>
        class random_access_iterator_range : public bidirectional_iterator_range<Iterator>
        {
        public:
            using typename bidirectional_iterator_range<Iterator>::position;
            using typename bidirectional_iterator_range<Iterator>::difference_type;
            using typename bidirectional_iterator_range<Iterator>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using bidirectional_iterator_range<Iterator>::bidirectional_iterator_range;

        public:
            difference_type distance(const position& p1, const position& p2) const noexcept { return ::std::distance(p1, p2); }
            size_type size() const noexcept { return size_type(distance(this->begin(), this->end())); }

            reference operator [] (size_type n) const { return this->begin()[n]; }
            reference at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return this->begin()[n];
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
            using pointer = pointer_type<Generator, is_generator>;
            using reference = reference_type<Generator, is_generator>;
            using generator = Generator;

        public:
            single_pass_generator_range()
                : gen(nullptr), first(), term()
            { }
            single_pass_generator_range(const generator& g, const position_type<generator, is_generator>& p, const TerminationPredicate& term)
                : gen(&g), first(p), term(term)
            { }
            single_pass_generator_range(const generator& g, const position_type<generator, is_generator>& p, TerminationPredicate&& term)
                : gen(&g), first(p), term(::std::move(term))
            { }
            single_pass_generator_range(const generator& g, position_type<generator, is_generator>&& p, const TerminationPredicate& term)
                : gen(&g), first(::std::move(p)), term(term)
            { }
            single_pass_generator_range(const generator& g, position_type<generator, is_generator>&& p, TerminationPredicate&& term)
                : gen(&g), first(::std::move(p)), term(::std::move(term))
            { }

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
            const position& begin_pos() const noexcept { return first; }
            void begin_pos(const position& pos) noexcept(noexcept(first = pos)) { first = pos; }
            void begin_pos(position&& pos) noexcept(noexcept(first = ::std::move(pos))) { first = ::std::move(pos); }

            bool is_end_pos(const position& pos) const noexcept { return gen == nullptr || term(*gen, pos); }

            position& advance_pos(position& pos) const
            {
                return gen->advance_pos(pos);
            }

            position& at_pos(const position& pos) const
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
            using multi_pass_generator_range<Generator, TerminationPredicate>::advance_pos;
            position& advance_pos(position& pos, difference_type n) const
            {
                return this->base().advance_pos(pos, n);
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
            difference_type distance(const position& p1, const position& p2) const noexcept
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
            using pointer = pointer_type<Generator, is_generator>;
            using reference = reference_type<Generator, is_generator>;
            using generator = Generator;

        public:
            delimited_multi_pass_generator_range()
                : gen(nullptr), first(), last()
            {
            }
            delimited_multi_pass_generator_range(const generator& g, const position_type<generator, is_generator>& f, const position_type<generator, is_generator>& l)
                : gen(&g), first(f), last(l)
            {
            }
            delimited_multi_pass_generator_range(const generator& g, position_type<generator, is_generator>&& f, position_type<generator, is_generator>&& l)
                : gen(&g), first(::std::move(f)), last(::std::move(l))
            {
            }

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
            const position& begin_pos() const noexcept { return first; }
            void begin_pos(const position& pos) noexcept(noexcept(first = pos)) { first = pos; }
            void begin_pos(position&& pos) noexcept(noexcept(first = ::std::move(pos))) { first = ::std::move(pos); }

            const position& end_pos() const noexcept { return last; }
            void end_pos(const position& pos) noexcept(noexcept(last = pos)) { last = pos; }
            void end_pos(position&& pos) noexcept(noexcept(last = ::std::move(pos))) { last = ::std::move(pos); }

            bool is_end_pos(const position& pos) const noexcept { return pos == last; }

            position& advance_pos(position& pos) const
            {
                return gen->advance_pos(pos);
            }

            position& at_pos(const position& pos) const
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
            using delimited_multi_pass_generator_range<Generator>::advance_pos;
            position& advance_pos(position& pos, difference_type n) { return this->base().advance_pos(pos, n); }
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
            difference_type distance(const position& p1, const position& p2) const noexcept { return this->base().distance(p1, p2); }
            size_type size() const noexcept { return size_type(distance(this->begin_pos(), this->end_pos())); }

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

        // counted ranges
        template <class Range>
        class counted_single_pass_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using pointer = pointer_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<position_type<Range, is_range>, size_type>;

        public:
            counted_single_pass_range() : underlying(nullptr), first(), count(0) { }
            counted_single_pass_range(const range& r, size_type count) : underlying(&r), first(r.begin_pos(), 0), count(count) { }

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
            const position& begin_pos() const noexcept { return first; }
            void begin_pos(const position& pos) noexcept(noexcept(first = pos)) { first = pos; }
            void begin_pos(position&& pos) noexcept(noexcept(first = ::std::move(pos))) { first = ::std::move(pos); }

            bool is_end_pos(const position& pos) const noexcept { return pos.second() == count; }

            position& advance_pos(position& pos) const
            {
                underlying->advance_pos(pos.first());
                ++pos.second();
                return pos;
            }

            reference at_pos(const position& pos) const
            {
                return underlying->at_pos(pos.first());
            }

            const range& base() const noexcept { return underlying; }

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
            using counted_multi_pass_range<Range>::advance_pos;
            position& advance_pos(position& pos, difference_type n) const
            {
                this->base().advance_pos(pos.first(), n);
                pos.second() += n;
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
            void end_pos(const position& pos) noexcept
            {
                this->resize(pos.second() - this->begin_pos().second());
            }

            difference_type distance(const position& p1, const position& p2) const noexcept
            {
                return p2.second() - p1.second();
            }

            reference operator [] (size_type n) const { return base()[n]; }
            reference at(size_type n) const { return base().at(n); }
        };

        // counted iterator ranges
        template <class Iterator>
        class counted_input_iterator_range
        {
        public:
            using range_category = detail::iterator_range_category_map_t<iterator_category<Iterator>>;
            using value_type = value_type<Iterator, is_iterator>;
            using difference_type = difference_type<Iterator, is_iterator>;
            using pointer = pointer_type<Iterator, is_iterator>;
            using reference = reference_type<Iterator, is_iterator>;
            using iterator = Iterator;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<iterator, size_type>;

        public:
            counted_input_iterator_range() : first(), count(0) { }
            counted_input_iterator_range(const iterator& i, size_type count) : first(i, 0), count(count) { }
            counted_input_iterator_range(iterator&& i, size_type count) : first(::std::move(i), 0), count(count) { }

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
            const position& begin_pos() const noexcept { return first; }
            void begin_pos(const position& pos) noexcept(noexcept(first = pos)) { first = pos; }
            void begin_pos(position&& pos) noexcept(noexcept(first = ::std::move(pos))) { first = ::std::move(pos); }

            bool is_end_pos(const position& pos) const noexcept { return pos.second() == count; }

            position& advance_pos(position& pos) const
            {
                ++pos.first();
                ++pos.second();
                return pos;
            }

            reference at_pos(const position& pos) const { return *pos.first(); }

            size_type size() const noexcept { return count - first.second(); }
            void resize(size_type n) noexcept { count = first.second() + n; }

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
            using counted_forward_iterator_range<Iterator>::advance_pos;
            position& advance_pos(position& pos, difference_type n) const
            {
                ::std::advance(pos.first(), n);
                pos.second() += n;
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
            void end_pos(const position& pos) noexcept
            {
                this->resize(pos.second() - this->begin_pos().second());
            }

            difference_type distance(const position& p1, const position& p2) const noexcept
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

        // counted generator ranges
        template <class Generator>
        class counted_single_pass_generator_range
        {
        public:
            using range_category = detail::generator_range_category_map_t<generator_category<Generator>>;
            using value_type = value_type<Generator, is_generator>;
            using difference_type = difference_type<Generator, is_generator>;
            using pointer = pointer_type<Generator, is_generator>;
            using reference = reference_type<Generator, is_generator>;
            using generator = Generator;

            using size_type = ::std::make_unsigned_t<difference_type>;
            using position = compressed_pair<position_type<Generator, is_generator>, size_type>;

        public:
            counted_single_pass_generator_range() : gen(nullptr), first(), count(0) { }
            counted_single_pass_generator_range(const generator& g, const position_type<Generator, is_generator>& pos, size_type count)
                : gen(&g), first(pos, 0), count(count) { }
            counted_single_pass_generator_range(const generator& g, position_type<Generator, is_generator>&& pos, size_type count)
                : gen(&g), first(::std::move(pos), 0), count(count) { }

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
            const position& begin_pos() const noexcept { return first; }
            void begin_pos(const position& pos) noexcept(noexcept(first = pos)) { first = pos; }
            void begin_pos(position&& pos) noexcept(noexcept(first = ::std::move(pos))) { first = ::std::move(pos); }

            bool is_end_pos(const position& pos) const noexcept { return pos.second() == count; }

            position& advance_pos(position& pos) const
            {
                gen->advance_pos(pos.first());
                ++pos.second();
                return pos;
            }

            reference at_pos(const position& pos) const
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
            using counted_multi_pass_generator_range<Generator>::advance_pos;
            position& advance_pos(position& pos, difference_type n) const
            {
                this->base().advance_pos(pos.first(), n);
                pos.second() += n;
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
            void end_pos(const position& pos) noexcept
            {
                this->resize(pos.second() - this->begin_pos().second());
            }

            difference_type distance(const position& p1, const position& p2) const noexcept
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

        // Reverse ranges
        template <class Range>
        class bidirectional_reverse_range
        {
        public:
            using range_category = range_category<Range>;
            using value_type = value_type<Range, is_range>;
            using position = position_type<Range, is_range>;
            using difference_type = difference_type<Range, is_range>;
            using pointer = pointer_type<Range, is_range>;
            using reference = reference_type<Range, is_range>;
            using range = Range;

        public:
            bidirectional_reverse_range() : underlying() { }
            bidirectional_reverse_range(const range& r) noexcept(noexcept(range(r))) : underlying(r) { }

        public:
            friend bool operator == (const bidirectional_reverse_range& a, const bidirectional_reverse_range& b) noexcept
            {
                return a.underlying == b.underlying || (a.underlying != nullptr && b.underlying != nullptr && *a.underlying == *b.underlying);
            }
            friend bool operator != (const bidirectional_reverse_range& a, const bidirectional_reverse_range& b) noexcept
            {
                return !(a == b);
            }

            const position& begin_pos() const noexcept { return underlying->end_pos(); }
            void begin_pos(const position& pos) noexcept { underlying->end_pos(pos); }
            void begin_pos(position&& pos) noexcept { underlying->end_pos(::std::move(pos)); }

            const position& end_pos() const noexcept { return underlying->begin_pos(); }
            void end_pos(const position& pos) noexcept { underlying->begin_pos(pos); }
            void end_pos(position&& pos) noexcept { underlying->begin_pos(::std::move(pos)); }

            bool is_end_pos(const position& pos) const noexcept { return pos = underlying->begin_pos(); }

            position& advance_pos(position& pos, difference_type n = 1) const
            {
                return underlying->advance_pos(pos, -n);
            }

            position& at_pos(const position& pos) const
            {
                return underlying->at_pos(prev_pos(underlying, pos));
            }

            const range& base() const noexcept { return *underlying; }

        private:
            const range* underlying;
        };

        template <class Range>
        class random_access_reverse_range : public bidirectional_reverse_range<Range>
        {
        public:
            using typename bidirectional_reverse_range<Range>::position;
            using typename bidirectional_reverse_range<Range>::difference_type;
            using typename bidirectional_reverse_range<Range>::reference;
            using size_type = ::std::make_unsigned_t<difference_type>;

        public:
            using bidirectional_reverse_range<Range>::bidirectional_reverse_range;

        public:
            difference_type distance(const position& p1, const position& p2) const noexcept { return this->base().distance(p2, p1); }
            size_type size() const noexcept { return this->base().size(); }

            decltype(auto) operator [] (size_type n) const
            {
                return this->base()[size() - n - 1];
            }
            decltype(auto) at(size_type n) const
            {
                if (n >= size())
                    throw ::std::out_of_range("element index out of range");
                return this->base()[size() - n - 1];
            }
        };
    }
}

#endif
