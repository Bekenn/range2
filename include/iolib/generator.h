#ifndef IOLIB_GENERATOR_INCLUDED
#define IOLIB_GENERATOR_INCLUDED
#pragma once

#include "concept.h"
#include "iterator.h"

#include <cassert>


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
        ::std::conditional_t<is_random_access_iterator<Iterator>::value, detail::random_access_iterator_generator<Iterator>,
            ::std::conditional_t<is_bidirectional_iterator<Iterator>::value, detail::bidirectional_iterator_generator<Iterator>,
                ::std::conditional_t<is_forward_iterator<Iterator>::value, detail::forward_iterator_generator<Iterator>,
                    ::std::enable_if_t<is_input_iterator<Iterator>::value, detail::input_iterator_generator<Iterator>>
                >
            >
        >;

    template <class Iterator, REQUIRES(is_iterator<::std::decay_t<Iterator>>::value)>
    auto make_generator(Iterator&& it)
    {
        return iterator_generator<::std::decay_t<Iterator>>(::std::forward<Iterator>(it));
    }

    namespace detail
    {
        template <class GeneratorCategory> struct generator_iterator_category_map;
        template <> struct generator_iterator_category_map<single_pass_generator_tag> { using type = ::std::input_iterator_tag; };
        template <> struct generator_iterator_category_map<multi_pass_generator_tag> { using type = ::std::forward_iterator_tag; };
        template <> struct generator_iterator_category_map<bidirectional_generator_tag> { using type = ::std::bidirectional_iterator_tag; };
        template <> struct generator_iterator_category_map<random_access_generator_tag> { using type = ::std::random_access_iterator_tag; };
        template <class GeneratorCategory> using generator_iterator_category_map_t = typename generator_iterator_category_map<GeneratorCategory>::type;

        template <class Generator> class single_pass_generator_iterator;
        template <class Generator> class multi_pass_generator_iterator;
        template <class Generator> class bidirectional_generator_iterator;
        template <class Generator> class random_access_generator_iterator;
    }

    template <class Generator> using generator_iterator =
        ::std::conditional_t<is_random_access_generator<Generator>::value, detail::random_access_generator_iterator<Generator>,
            ::std::conditional_t<is_bidirectional_generator<Generator>::value, detail::bidirectional_generator_iterator<Generator>,
                ::std::conditional_t<is_multi_pass_generator<Generator>::value, detail::multi_pass_generator_iterator<Generator>,
                    ::std::enable_if_t<is_single_pass_generator<Generator>::value, detail::single_pass_generator_iterator<Generator>>
                >
            >
        >;

    template <class Generator, REQUIRES(is_generator<Generator>::value)>
    auto make_iterator(const Generator& gen, position_type<Generator, is_generator> pos)
    {
        return generator_iterator<Generator>(gen, pos);
    }

    template <class T>
    class constant_generator
    {
    public:
        using generator_category = random_access_generator_tag;
        using value_type = T;
        using difference_type = ::std::ptrdiff_t;
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
        constexpr reference at_pos(position pos) const noexcept { return v; }

    private:
        value_type v;
    };

    template <class T>
    auto make_constant_generator(T&& v)
    {
        return constant_generator<::std::decay_t<T>>(::std::forward<T>(v));
    }

    namespace detail
    {
        // iterator generators
        template <class Iterator>
        class input_iterator_generator
        {
        public:
            using generator_category = detail::iterator_generator_category_map_t<typename ::std::iterator_traits<Iterator>::iterator_category>;
            using value_type = typename ::std::iterator_traits<Iterator>::value_type;
            using position = Iterator;
            using difference_type = typename ::std::iterator_traits<Iterator>::difference_type;
            using reference = typename ::std::iterator_traits<Iterator>::reference;
            using iterator = Iterator;

        public:
            input_iterator_generator() noexcept : i(nullptr) { }
            explicit input_iterator_generator(const iterator& i) noexcept : i(i) { }
            explicit input_iterator_generator(iterator&& i) noexcept : i(::std::move(i)) { }

            friend bool operator == (const input_iterator_generator& a, const input_iterator_generator& b) noexcept
            {
                return a.i == b.i;
            }
            friend bool operator != (const input_iterator_generator& a, const input_iterator_generator& b) noexcept
            {
                return !(a == b);
            }

        public:
            position origin_pos() const noexcept { return i; }
            position& advance_pos(position& pos) const { return ++pos; }
            reference at_pos(position pos) const { return *pos; }

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

        // generator iterators
        template <class Generator>
        class single_pass_generator_iterator
        {
        public:
            using iterator_category = detail::generator_iterator_category_map_t<generator_category<Generator>>;
            using value_type = value_type<Generator, is_generator>;
            using difference_type = difference_type<Generator, is_generator>;
            using pointer = value_type*;
            using reference = reference_type<Generator, is_generator>;
            using position = position_type<Generator, is_generator>;
            using generator = Generator;

        public:
            single_pass_generator_iterator() noexcept : gen(nullptr), pos() { }
            single_pass_generator_iterator(const generator& gen, position pos)
                : gen(&gen), pos(pos) { }

            friend bool operator == (const single_pass_generator_iterator& a, const single_pass_generator_iterator& b) noexcept
            {
                return (a.gen == b.gen || (a.gen != nullptr && b.gen != nullptr && *a.gen == *b.gen))
                    && a.pos == b.pos;
            }
            friend bool operator != (const single_pass_generator_iterator& a, const single_pass_generator_iterator& b) noexcept
            {
                return !(a == b);
            }

        public:
            reference operator * () const { return gen->at_pos(pos); }
            pointer operator -> () const { return ::std::addressof(gen->at_pos(pos)); }
            single_pass_generator_iterator& operator ++ ()
                noexcept(noexcept(gen->advance_pos(pos)))
            {
                gen->advance_pos(pos);
                return *this;
            }
            iterator_proxy<single_pass_generator_iterator> operator ++ (int)
            {
                iterator_proxy<single_pass_generator_iterator> proxy(gen->at_pos(pos));
                ++*this;
                return proxy;
            }

            const generator& base_generator() const noexcept { return *gen; }
            position base_position() const noexcept { return pos; }

        private:
            friend class multi_pass_generator_iterator<Generator>;
            friend class bidirectional_generator_iterator<Generator>;
            friend class random_access_generator_iterator<Generator>;

            const generator* gen;
            position pos;
        };

        template <class Generator>
        class multi_pass_generator_iterator : public single_pass_generator_iterator<Generator>
        {
        public:
            using single_pass_generator_iterator<Generator>::single_pass_generator_iterator;

        public:
            using single_pass_generator_iterator<Generator>::operator ++;
            multi_pass_generator_iterator operator ++ (int)
            {
                multi_pass_generator_iterator tmp = *this;
                ++*this;
                return tmp;
            }
        };

        template <class Generator>
        class bidirectional_generator_iterator : public multi_pass_generator_iterator<Generator>
        {
        public:
            using multi_pass_generator_iterator<Generator>::multi_pass_generator_iterator;

        public:
            bidirectional_generator_iterator& operator -- ()
            {
                this->gen->advance_pos(this->pos, -1);
                return *this
            }
            bidirectional_generator_iterator operator -- (int)
            {
                bidirectional_generator_iterator tmp = *this;
                --*this;
                return tmp;
            }
        };

        template <class Generator>
        class random_access_generator_iterator : public bidirectional_generator_iterator<Generator>
        {
        public:
            using typename bidirectional_generator_iterator<Generator>::difference_type;
            using typename bidirectional_generator_iterator<Generator>::reference;
            using typename bidirectional_generator_iterator<Generator>::position;

        public:
            using bidirectional_generator_iterator<Generator>::bidirectional_generator_iterator;

            friend bool operator <  (const random_access_generator_iterator& a, const random_access_generator_iterator& b) noexcept
            {
                if (a.gen == nullptr)
                    return b.gen != nullptr;
                if (b.gen == nullptr)
                    return false;

                assert(*a.gen == *b.gen)
                return a.gen->distance(a.pos, b.pos) < 0;
            }
            friend bool operator >  (const random_access_generator_iterator& a, const random_access_generator_iterator& b) noexcept
            {
                return b < a;
            }
            friend bool operator <= (const random_access_generator_iterator& a, const random_access_generator_iterator& b) noexcept
            {
                return !(b < a);
            }
            friend bool operator >= (const random_access_generator_iterator& a, const random_access_generator_iterator& b) noexcept
            {
                return !(a < b);
            }

        public:
            random_access_generator_iterator& operator += (difference_type n)
            {
                this->gen->advance_pos(this->pos, n);
                return *this;
            }
            random_access_generator_iterator& operator -= (difference_type n)
            {
                this->gen->advance_pos(this->pos, -n);
                return *this;
            }

            reference operator [] (difference_type n) const
            {
                position p = this->pos;
                this->gen->advance_pos(p, n);
                return this->gen->at_pos(p);
            }

            friend random_access_generator_iterator operator + (random_access_generator_iterator it, difference_type n)
            {
                return it += n;
            }
            friend random_access_generator_iterator operator + (difference_type n, random_access_generator_iterator it)
            {
                return it += n;
            }
            friend random_access_generator_iterator operator - (random_access_generator_iterator it, difference_type n)
            {
                return it -= n;
            }
            friend difference_type operator - (const random_access_generator_iterator& a, const random_access_generator_iterator& b) noexcept
            {
                assert(a.base_generator() == b.base_generator());
                return a.base_generator().distance(a.base_position(), b.base_position());
            }
        };
    }
}

#endif
