//
//  generator.h
//  stdext
//
//  Created by James Touton on 10/31/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_GENERATOR_INCLUDED
#define STDEXT_GENERATOR_INCLUDED
#pragma once

#include "concept.h"
#include "consumer.h"

#include <cassert>


// Generators are input iterators that know when the underlying sequence is exhausted. The user
// can test for this by applying a conversion to bool. To distinguish generators from other
// iterators, all generators should have an iterator_category of generator_tag.
namespace stdext
{
    struct generator_tag : ::std::forward_iterator_tag { };

    namespace detail
    {
        template <class T, bool = is_iterator<T>::value>
        struct is_generator_helper { static constexpr bool value = false; };
        template <class T>
        struct is_generator_helper<T, true>
        {
            static constexpr bool value = ::std::is_base_of<iterator_category<T>, generator_tag>::value;
        };
    }

    template <class T> struct is_generator
        : ::std::conditional_t<detail::is_generator_helper<T>::value, true_type, false_type>
    { };

    template <class T> struct is_generator_adaptable
    {
        template <class U> static true_type test(decltype(make_generator(declval<U&>()))*);
        template <class U> static false_type test(...);
        static constexpr bool value = decltype(test<T>(nullptr))::value;
    };

    template <class T> struct can_generate
        : ::std::conditional_t<is_generator<T>::value || is_generator_adaptable<T>::value,
            true_type,
            false_type>
    { };

    template <class T, REQUIRES(is_generator<::std::decay_t<T>>::value)>
    decltype(auto) as_generator(T&& g)
    {
        return forward<T>(g);
    }

    template <class T, REQUIRES(is_generator_adaptable<::std::decay_t<T>>::value)>
    auto as_generator(T&& g)
    {
        return make_generator(forward<T>(g));
    }

    namespace detail
    {
        template <class Generator>
        struct value_type_of<Generator, is_generator, true> { using type = typename Generator::value_type; };
        template <class Generator>
        struct difference_type_of<Generator, is_generator, true> { using type = typename Generator::difference_type; };
        template <class Generator>
        struct pointer_type_of<Generator, is_generator, true> { using type = typename Generator::pointer; };
        template <class Generator>
        struct reference_type_of<Generator, is_generator, true> { using type = typename Generator::reference; };

        template <class Generator>
        struct value_type_of<Generator, is_generator_adaptable, true> { using type = value_type_t<decltype(make_generator(declval<Generator&>())), is_generator>; };
        template <class Generator>
        struct difference_type_of<Generator, is_generator_adaptable, true> { using type = difference_type_t<decltype(make_generator(declval<Generator&>())), is_generator>; };
        template <class Generator>
        struct pointer_type_of<Generator, is_generator_adaptable, true> { using type = pointer_type_t<decltype(make_generator(declval<Generator&>())), is_generator>; };
        template <class Generator>
        struct reference_type_of<Generator, is_generator_adaptable, true> { using type = reference_type_t<decltype(make_generator(declval<Generator&>())), is_generator>; };
        template <class Generator>
        struct generator_type_of<Generator, is_generator_adaptable, true> { using type = decltype(make_generator(declval<Generator&>())); };

        template <class Generator>
        struct value_type_of<Generator, can_generate, true> : value_type_of<Generator, is_generator>, value_type_of<Generator, is_generator_adaptable> { };
        template <class Generator>
        struct difference_type_of<Generator, can_generate, true> : difference_type_of<Generator, is_generator>, difference_type_of<Generator, is_generator_adaptable> { };
        template <class Generator>
        struct pointer_type_of<Generator, can_generate, true> : pointer_type_of<Generator, is_generator>, pointer_type_of<Generator, is_generator_adaptable> { };
        template <class Generator>
        struct reference_type_of<Generator, can_generate, true> : reference_type_of<Generator, is_generator>, reference_type_of<Generator, is_generator_adaptable> { };
        template <class Generator>
        struct generator_type_of<Generator, can_generate, true> { using type = ::std::decay_t<decltype(as_generator(declval<Generator&>()))>; };
    }

    template <class Generator> using generator_value_type = value_type<Generator, is_generator>;
    template <class Generator> using generator_value_type_t = value_type_t<Generator, is_generator>;
    template <class Generator> using generator_difference_type = difference_type<Generator, is_generator>;
    template <class Generator> using generator_difference_type_t = difference_type_t<Generator, is_generator>;
    template <class Generator> using generator_pointer_type = pointer_type<Generator, is_generator>;
    template <class Generator> using generator_pointer_type_t = pointer_type_t<Generator, is_generator>;
    template <class Generator> using generator_reference_type = generator_type<Generator, is_generator>;
    template <class Generator> using generator_reference_type_t = generator_type_t<Generator, is_generator>;

    template <class Generator, class Consumer, REQUIRES(is_consumer<::std::decay_t<Consumer>(value_type_t<::std::decay_t<Generator>, can_generate>)>::value)>
    size_t operator >> (Generator&& g, Consumer&& c)
    {
        size_t count = 0;
        for (decltype(auto) gen = as_generator(forward<Generator>(g)); gen; ++gen)
        {
            if (!c(*gen))
                break;
            ++count;
        }

        return count;
    }

    template <class Iterator>
    class iterator_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type_t<Iterator>;
        using difference_type = iterator_difference_type_t<Iterator>;
        using pointer = iterator_pointer_type_t<Iterator>;
        using reference = iterator_reference_type_t<Iterator>;
        using iterator = Iterator;

    public:
        iterator_generator() : i() { }
        explicit iterator_generator(const iterator& i) : i(i) { }
        explicit iterator_generator(iterator&& i) : i(move(i)) { }

    public:
        friend bool operator == (const iterator_generator& a, const iterator_generator& b) noexcept
        {
            return a.i == b.i;
        }
        friend bool operator != (const iterator_generator& a, const iterator_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(iterator_generator& a, iterator_generator& b)
        {
            swap(a.i, b.i);
        }

    public:
        reference operator * () const { return *i; }
        pointer operator -> () const { return i.operator -> (); }
        iterator_generator& operator ++ () { ++i; return *this; }
        decltype(auto) operator ++ (int) { return i++; }
        explicit operator bool () const { return true; }
        const iterator& base() const { return i; }

    private:
        iterator i;
    };

    template <class Iterator, class Sentinel>
    class delimited_iterator_generator : public iterator_generator<Iterator>
    {
    public:
        using typename iterator_generator<Iterator>::iterator;
        using sentinel = Sentinel;

    public:
        delimited_iterator_generator() : iterator_generator<Iterator>(), j() { }
        delimited_iterator_generator(const iterator& i, const sentinel& j) : iterator_generator<Iterator>(i), j(j) { }
        delimited_iterator_generator(const iterator& i, sentinel&& j) : iterator_generator<Iterator>(i), j(move(j)) { }
        delimited_iterator_generator(iterator&& i, const sentinel& j) : iterator_generator<Iterator>(move(i)), j(j) { }
        delimited_iterator_generator(iterator&& i, sentinel&& j) : iterator_generator<Iterator>(move(i)), j(move(j)) { }

    public:
        explicit operator bool () const { return this->base() != j; }

    private:
        sentinel j;
    };

    template <class Function>
    class function_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = const ::std::remove_cv_t<decltype(::std::declval<Function>()())>;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        function_generator() : f(), value() { }
        explicit function_generator(const Function& f) : f(f), value(this->f()) { }
        explicit function_generator(Function&& f) : f(move(f)), value(this->f()) { }

    public:
        friend bool operator == (const function_generator& a, const function_generator& b) noexcept
        {
            return a.f == b.f
                && a.value == b.value;
        }
        friend bool operator != (const function_generator& a, const function_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(function_generator&& a, function_generator&& b)
        {
            swap(a.f, b.f);
            swap(a.value, b.value);
        }

    public:
        reference operator * () const { return value; }
        pointer operator -> () const { return &value; }
        function_generator& operator ++ () { value = f(); return *this; }
        iterator_proxy<function_generator> operator ++ (int)
        {
            iterator_proxy<function_generator> proxy(value);
            ++*this;
            return proxy;
        }
        explicit operator bool () const { return true; }

    private:
        Function f;
        ::std::remove_const_t<value_type> value;
    };

    template <class T>
    class constant_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = const T;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        constant_generator() : v() { }
        explicit constant_generator(const value_type& v)
            noexcept(::std::is_nothrow_copy_constructible<value_type>::value)
            : v(v) { }
        explicit constant_generator(value_type&& v)
            noexcept(::std::is_nothrow_move_constructible<value_type>::value)
            : v(move(v)) { }

    public:
        friend bool operator == (const constant_generator& a, const constant_generator& b) noexcept
        {
            return a.v == b.v;
        }
        friend bool operator != (const constant_generator& a, const constant_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(constant_generator& a, constant_generator& b)
        {
            swap(a.v, b.v);
        }

    public:
        reference operator * () const { return v; }
        pointer operator -> () const { return &v; }
        constant_generator& operator ++ () { return *this; }
        constant_generator operator ++ (int) { return *this; }
        explicit operator bool () const { return true; }

    private:
        value_type v;
    };

    template <class Iterator, class TerminationPredicate>
    class terminated_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type_t<Iterator>;
        using difference_type = iterator_difference_type_t<Iterator>;
        using pointer = iterator_pointer_type_t<Iterator>;
        using reference = iterator_reference_type_t<Iterator>;
        using iterator = Iterator;

    public:
        terminated_generator() : i(), term() { }
        terminated_generator(const iterator& g, const TerminationPredicate& term)
            : i(g), term(term) { }
        terminated_generator(const iterator& g, TerminationPredicate&& term)
            : i(g), term(move(term)) { }
        terminated_generator(iterator&& g, const TerminationPredicate& term)
            : i(move(g)), term(term) { }
        terminated_generator(iterator&& g, TerminationPredicate&& term)
            : i(::std::move(g)), term(move(term)) { }

    public:
        friend void swap(terminated_generator& a, terminated_generator& b)
        {
            swap(a.i, b.i);
            swap(a.term, b.term);
        }

    public:
        reference operator * () const { return *i; }
        pointer operator -> () const { return i.operator -> (); }
        terminated_generator& operator ++ () { ++i; return *this; }
        auto operator ++ (int) { return i++; }
        explicit operator bool () const { return !term(*i); }
        const iterator& base() const { return i; }

    private:
        iterator i;
        TerminationPredicate term;
    };

    template <class Iterator, REQUIRES(is_iterator<::std::decay_t<Iterator>>::value && !is_generator<::std::decay_t<Iterator>>::value)>
    auto make_generator(Iterator&& i)
    {
        return iterator_generator<::std::decay_t<Iterator>>(forward<Iterator>(i));
    }

    template <class Iterator, class Sentinel,
        REQUIRES(is_iterator<::std::decay_t<Iterator>>::value && is_equality_comparable<::std::decay_t<Iterator>, ::std::decay_t<Sentinel>>::value)>
    auto make_generator(Iterator&& i, Sentinel&& j)
    {
        return delimited_iterator_generator<::std::decay_t<Iterator>, ::std::decay_t<Sentinel>>(forward<Iterator>(i), forward<Sentinel>(j));
    }

    template <class Function, REQUIRES(is_callable<::std::decay_t<Function>>::value)>
    auto make_generator(Function&& function)
    {
        return function_generator<::std::decay_t<Function>>(forward<Function>(function));
    }

    template <class T>
    auto make_constant_generator(T&& v)
    {
        return constant_generator<::std::decay_t<T>>(forward<T>(v));
    }

    template <class Iterator, class TerminationPredicate,
    REQUIRES(is_iterator<::std::decay_t<Iterator>>::value && is_callable<::std::decay_t<TerminationPredicate>(iterator_value_type_t<::std::decay_t<Iterator>>), bool>::value)>
    auto make_terminated_generator(Iterator&& i, TerminationPredicate&& term)
    {
        return terminated_generator<::std::decay_t<Iterator>, ::std::decay_t<TerminationPredicate>>
            (forward<Iterator>(i), forward<TerminationPredicate>(term));
    }
}

#endif
