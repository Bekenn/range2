#ifndef IOLIB_GENERATOR_INCLUDED
#define IOLIB_GENERATOR_INCLUDED
#pragma once

#include "concept.h"
#include "iterator.h"


namespace iolib
{
    // generator categories
    struct single_pass_generator_tag { };
    struct multi_pass_generator_tag : single_pass_generator_tag { };
    struct bidirectional_generator_tag : multi_pass_generator_tag { };
    struct random_access_generator_tag : bidirectional_generator_tag { };

    // generator traits
    DECLARE_HAS_INNER_TYPE(generator_category);
    template <class T> struct is_generator
        : ::std::conditional_t<HAS_INNER_TYPE(T, generator_category),
            ::std::true_type,
            ::std::false_type>
    { };
    template <class Generator> using generator_category = typename Generator::generator_category;
    namespace detail
    {
        template <class Generator>
        struct value_type_of<Generator, is_generator> { using type = typename Generator::value_type; };
        template <class Generator>
        struct position_type_of<Generator, is_generator> { using type = typename Generator::position; };
        template <class Generator>
        struct difference_type_of<Generator, is_generator> { using type = typename Generator::difference_type; };
        template <class Generator>
        struct pointer_type_of<Generator, is_generator> { using type = typename Generator::pointer; };
        template <class Generator>
        struct reference_type_of<Generator, is_generator> { using type = typename Generator::reference; };
    }

    // generator concepts
    template <class Generator> struct is_single_pass_generator
    {
        static constexpr auto value = ::std::is_base_of<single_pass_generator_tag, generator_category<Generator>>::value;
    };
    template <class Generator> struct is_multi_pass_generator
    {
        static constexpr auto value = ::std::is_base_of<multi_pass_generator_tag, generator_category<Generator>>::value;
    };
    template <class Generator> struct is_bidirectional_generator
    {
        static constexpr auto value = ::std::is_base_of<bidirectional_generator_tag, generator_category<Generator>>::value;
    };
    template <class Generator> struct is_random_access_generator
    {
        static constexpr auto value = ::std::is_base_of<random_access_generator_tag, generator_category<Generator>>::value;
    };

    // basic generator
    template <class Category, class T, class Position, class Distance = ::std::ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct generator
    {
        using generator_category = Category;
        using value_type = T;
        using position = Position;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };

    namespace detail
    {
        template <class IteratorCategory> struct iterator_generator_category_map;
        template <> struct iterator_generator_category_map<::std::input_iterator_tag> { using type = single_pass_generator_tag; };
        template <> struct iterator_generator_category_map<::std::forward_iterator_tag> { using type = multi_pass_generator_tag; };
        template <> struct iterator_generator_category_map<::std::bidirectional_iterator_tag> { using type = bidirectional_generator_tag; };
        template <> struct iterator_generator_category_map<::std::random_access_iterator_tag> { using type = random_access_generator_tag; };
        template <class IteratorCategory> using iterator_generator_category_map_t = typename iterator_generator_category_map<IteratorCategory>::type;

        template <class Iterator> class input_iterator_generator;
        template <class Iterator> class forward_iterator_generator;
        template <class Iterator> class bidirectional_iterator_generator;
        template <class Iterator> class random_access_iterator_generator;
    }

    template <class Iterator> using iterator_generator =
        ::std::conditional_t<::std::is_base_of<::std::random_access_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
            detail::random_access_iterator_generator<Iterator>,
            ::std::conditional_t<::std::is_base_of<::std::bidirectional_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
                detail::bidirectional_iterator_generator<Iterator>,
                ::std::conditional_t<::std::is_base_of<::std::forward_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
                    detail::forward_iterator_generator<Iterator>,
                    ::std::enable_if_t<::std::is_base_of<::std::input_iterator_tag, typename ::std::iterator_traits<Iterator>::iterator_category>::value,
                        detail::input_iterator_generator<Iterator>
                    >
                >
            >
        >;

    template <class T>
    class constant_generator
    {
    public:
        using generator_category = random_access_generator_tag;
        using value_type = T;
        using difference_type = ::std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        struct position
        {
            friend bool constexpr operator == (const position&, const position&) noexcept { return true; }
            friend bool constexpr operator != (const position&, const position&) noexcept { return false; }
        };

    public:
        constexpr constant_generator() : v() { }
        explicit constexpr constant_generator(const value_type& v)
            noexcept(::std::is_nothrow_copy_constructible<value_type>::value)
            : v(v) { }
        explicit constexpr constant_generator(value_type&& v)
            noexcept(::std::is_nothrow_move_constructible<value_type>::value)
            : v(::std::move(v)) { }

        friend constexpr bool operator == (const constant_generator& a, const constant_generator& b) noexcept
        {
            return a.v == b.v;
        }
        friend constexpr bool operator != (const constant_generator& a, const constant_generator& b) noexcept
        {
            return !(a == b);
        }

    public:
        constexpr position origin_pos() const noexcept { return { }; }
        constexpr position& advance_pos(position& pos, difference_type n = 1) const noexcept { return pos; }
        constexpr reference at_pos(const position& pos) const noexcept { return v; }

    private:
        value_type v;
    };

    namespace detail
    {
        template <class Iterator>
        class input_iterator_generator
        {
        public:
            using generator_category = detail::iterator_generator_category_map_t<typename ::std::iterator_traits<Iterator>::iterator_category>;
            using value_type = typename ::std::iterator_traits<Iterator>::value_type;
            using position = Iterator;
            using difference_type = typename ::std::iterator_traits<Iterator>::difference_type;
            using pointer = typename ::std::iterator_traits<Iterator>::pointer;
            using reference = typename ::std::iterator_traits<Iterator>::reference;
            using iterator = Iterator;

        public:
            input_iterator_generator() noexcept : i(nullptr) { }
            explicit input_iterator_generator(iterator i) noexcept : i(i) { }

            friend bool operator == (const input_iterator_generator& a, const input_iterator_generator& b) noexcept
            {
                return a.i == b.i;
            }
            friend bool operator != (const input_iterator_generator& a, const input_iterator_generator& b) noexcept
            {
                return !(a == b);
            }

        public:
            const position& origin_pos() const noexcept { return i; }
            position& advance_pos(position& pos) const { return ++pos; }
            reference at_pos(const position& pos) const { return *pos; }

        private:
            iterator i;
        };

        template <class Iterator>
        class forward_iterator_generator : public input_iterator_generator<Iterator>
        {
        public:
            using input_iterator_generator<Iterator>::input_iterator_generator;
        };

        template <class Iterator>
        class bidirectional_iterator_generator : public forward_iterator_generator<Iterator>
        {
        public:
            using position = typename forward_iterator_generator<Iterator>::position;
            using difference_type = typename forward_iterator_generator<Iterator>::difference_type;

        public:
            using forward_iterator_generator<Iterator>::forward_iterator_generator;

        public:
            using forward_iterator_generator<Iterator>::advance_pos;
            position& advance_pos(position& pos, difference_type n)
            {
                ::std::advance(pos, n);
                return pos;
            }
        };

        template <class Iterator>
        class random_access_iterator_generator : public bidirectional_iterator_generator<Iterator>
        {
        public:
            using bidirectional_iterator_generator<Iterator>::bidirectional_iterator_generator;
        };
    }
}

#endif
