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

#include <stdext/consumer.h>
#include <stdext/optional.h>

#include <cassert>


// Generators are a category of input iterators that can report whether or not the underlying
// sequence is exhausted.  The user can test for exhaustion by applying a contextual conversion
// to bool.
//
// Given a type `X` and a value `a` of type `X`, `X` satisfies the requirements of a generator
// if:
//  * `X` satisfies the requirements of an input iterator ([input.iterators]), and
//  * the expression `bool(a)` is valid, and is `true` if and only if `a` is dereferenceable.
//
// The type `std::iterator_traits<X>::iterator_category` shall be or be derived from the type
// `stdext::generator_tag`.

namespace stdext
{
    struct generator_tag : std::input_iterator_tag { };

    namespace _private
    {
        template <typename X, typename Traits = std::iterator_traits<X>, typename = typename Traits::value_type>
        std::conjunction<
            is_input_iterator<X>,
            std::is_constructible<bool, X>,
            std::is_base_of<generator_tag, typename Traits::iterator_category>>
        test_is_generator(int);
        template <typename X> false_type test_is_generator(...);
    }
    template <typename T> struct is_generator : decltype(_private::test_is_generator<T>(0)) { };
    template <> struct is_generator<void*> : false_type { };
    template <> struct is_generator<const void*> : false_type { };
    template <> struct is_generator<volatile void*> : false_type { };
    template <> struct is_generator<const volatile void*> : false_type { };
    template <typename T> constexpr auto is_generator_v = is_generator<T>::value;

    template <typename T> using generator_value_type = std::remove_cv_t<typename std::iterator_traits<T>::value_type>;
    template <typename T> using generator_difference_type = typename std::iterator_traits<T>::difference_type;
    template <typename T> using generator_pointer_type = typename std::iterator_traits<T>::pointer;
    template <typename T> using generator_reference_type = typename std::iterator_traits<T>::reference;
    template <typename T> using generator_size_type = std::make_unsigned_t<generator_difference_type<T>>;

    template <typename Iterator, typename Sentinel = Iterator>
    class delimited_iterator_generator
    {
        static_assert(is_input_iterator_v<Iterator>);
        static_assert(is_sentinel_for_v<Sentinel, Iterator>);
        static_assert(!std::is_const_v<Iterator>);

    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type<Iterator>;
        using difference_type = iterator_difference_type<Iterator>;
        using pointer = iterator_pointer_type<Iterator>;
        using reference = iterator_reference_type<Iterator>;
        using iterator = Iterator;
        using sentinel = Sentinel;

    public:
        explicit delimited_iterator_generator(const iterator& i, const sentinel& j) : _i(i), _j(j) { }
        template <typename T1, typename T2,
            STDEXT_REQUIRES(std::is_constructible_v<iterator, T1> && std::is_constructible_v<sentinel, T2>)>
        explicit delimited_iterator_generator(T1&& i, T2&& j) : _i(stdext::forward<T1>(i)), _j(stdext::forward<T2>(j)) { }

    public:
        friend bool operator == (const delimited_iterator_generator& a, const delimited_iterator_generator& b)
        {
            return a._i == b._i && a._j == b._j;
        }
        friend bool operator != (const delimited_iterator_generator& a, const delimited_iterator_generator& b)
        {
            return a._i != b._i || a._j != b._j;
        }

        friend void swap(delimited_iterator_generator& a, delimited_iterator_generator& b)
            noexcept(std::is_nothrow_swappable_v<iterator> && std::is_nothrow_swappable_v<sentinel>)
        {
            swap(a._i, b._i);
            swap(a._j, b._j);
        }

    public:
        reference operator * () const { assert(_i != _j); return *_i; }
        const iterator& operator -> () const { assert(_i != _j); return _i; }
        delimited_iterator_generator& operator ++ () { assert(_i != _j); ++_i; return *this; }
        decltype(auto) operator ++ (int) { assert(_i != _j); return _i++; }
        explicit operator bool () const { return _i != _j; }
        const iterator& base() const noexcept { return _i; }
        const sentinel& end() const noexcept { return _j; }

    private:
        iterator _i;
        sentinel _j;
    };

    template <typename Iterator>
    class counted_iterator_generator
    {
        static_assert(is_input_iterator_v<Iterator>);
        static_assert(!std::is_const_v<Iterator>);

    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type<Iterator>;
        using difference_type = iterator_difference_type<Iterator>;
        using pointer = iterator_pointer_type<Iterator>;
        using reference = iterator_reference_type<Iterator>;
        using iterator = Iterator;
        using size_type = iterator_size_type<Iterator>;

    public:
        explicit counted_iterator_generator(const iterator& i, size_type n) : _i(i), _n(n) { }
        template <typename T, STDEXT_REQUIRES(std::is_constructible_v<iterator, T>)>
        explicit counted_iterator_generator(T&& i, size_type n) : _i(stdext::forward<T>(i)), _n(n) { }

    public:
        friend bool operator == (const counted_iterator_generator& a, const counted_iterator_generator& b)
        {
            return a._i == b._i && a._n == b._n;
        }
        friend bool operator != (const counted_iterator_generator& a, const counted_iterator_generator& b)
        {
            return a._i != b._i || a._n != b._n;
        }

        friend void swap(counted_iterator_generator& a, counted_iterator_generator& b)
            noexcept(std::is_nothrow_swappable_v<iterator>)
        {
            swap(a._i, b._i);
            swap(a._n, b._n);
        }

    public:
        reference operator * () const { assert(_n != 0); return *_i; }
        const iterator& operator -> () const { assert(_n != 0); return _i; }
        counted_iterator_generator& operator ++ () { assert(_n != 0); ++_i; --_n; return *this; }
        decltype(auto) operator ++ (int) { assert(_n != 0); auto&& r = _i++; --_n; return stdext::forward<decltype(r)>(r); }
        explicit operator bool () const noexcept { return _n != 0; }
        const iterator& base() const noexcept { return _i; }
        size_type count() const noexcept { return _n; }

    private:
        iterator _i;
        size_type _n;
    };

    template <typename Function, typename R = decltype(std::declval<Function&>()())>
    class function_generator
    {
        static_assert(!std::is_void_v<R>);

    public:
        using iterator_category = generator_tag;
        using value_type = std::remove_cv_t<R>;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        explicit function_generator(const Function& f) : _f(f), _value(_f()) { }
        template <typename T = Function, STDEXT_REQUIRES(std::is_constructible_v<Function, T>)>
        explicit function_generator(T&& f) : _f(stdext::forward<T>(f)), _value(_f()) { }

    public:
        friend bool operator == (const function_generator& a, const function_generator& b)
        {
            return a._f == b._f && a._value == b._value;
        }
        friend bool operator != (const function_generator& a, const function_generator& b)
        {
            return a._f != b._f || a._value != b._value;
        }

        friend void swap(function_generator&& a, function_generator&& b)
            noexcept(std::is_nothrow_swappable_v<Function> && std::is_nothrow_swappable_v<value_type>)
        {
            swap(a._f, b._f);
            swap(a._value, b._value);
        }

    public:
        reference operator * () const noexcept { return _value; }
        pointer operator -> () const noexcept { return addressof(_value); }
        function_generator& operator ++ () { _value = _f(); return *this; }
        iterator_proxy<function_generator> operator ++ (int) { return exchange(_value, _f()); }
        explicit operator bool () const noexcept { return true; }

    private:
        Function _f;
        value_type _value;
    };

    template <typename Function, typename R>
    class function_generator<Function, optional<R>>
    {
    public:
        using iterator_category = generator_tag;
        using value_type = std::remove_cv_t<R>;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        explicit function_generator(const Function& f) : _f(f), _opt(_f()) { }
        template <typename T = Function, STDEXT_REQUIRES(std::is_constructible_v<Function, T>)>
        explicit function_generator(T&& f) : _f(stdext::forward<T>(f)), _opt(_f()) { }

    public:
        friend bool operator == (const function_generator& a, const function_generator& b)
        {
            return a._f == b._f && a._opt == b._opt;
        }
        friend bool operator != (const function_generator& a, const function_generator& b)
        {
            return a._f != b._f || a._opt != b._opt;
        }

        friend void swap(function_generator&& a, function_generator&& b)
            noexcept(std::is_nothrow_swappable_v<Function> && std::is_nothrow_swappable_v<optional<value_type>>)
        {
            swap(a._f, b._f);
            swap(a._opt, b._opt);
        }

    public:
        reference operator * () const noexcept { assert(_opt.has_value()); return _opt.value(); }
        pointer operator -> () const noexcept { assert(_opt.has_value()); return addressof(_opt.value()); }
        function_generator& operator ++ () { assert(_opt.has_value()); _opt = _f(); return *this; }
        iterator_proxy<function_generator> operator ++ (int) { assert(_opt.has_value()); return _opt.exchange(_f()); }
        explicit operator bool() const noexcept { return _opt.has_value(); }

    private:
        Function _f;
        optional<value_type> _opt;
    };

    template <typename T>
    class constant_generator
    {
        static_assert(!std::is_const_v<T>);
        static_assert(!std::is_volatile_v<T>);
        static_assert(!std::is_reference_v<T>);

    public:
        using iterator_category = generator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        explicit constant_generator(const value_type& v) : _v(v) { }
        template <typename U = T, STDEXT_REQUIRES(std::is_constructible_v<value_type, U>)>
        explicit constant_generator(U&& v) : _v(stdext::forward<U>(v)) { }

    public:
        friend bool operator == (const constant_generator& a, const constant_generator& b)
        {
            return a._v == b._v;
        }
        friend bool operator != (const constant_generator& a, const constant_generator& b)
        {
            return a._v != b._v;
        }

        friend void swap(constant_generator& a, constant_generator& b)
            noexcept(std::is_nothrow_swappable_v<value_type>)
        {
            swap(a._v, b._v);
        }

    public:
        reference operator * () const noexcept { return _v; }
        pointer operator -> () const noexcept { return addressof(_v); }
        constant_generator& operator ++ () noexcept { return *this; }
        constant_generator operator ++ (int) noexcept { return *this; }
        explicit operator bool () const noexcept { return true; }

    private:
        value_type _v;
    };

    template <typename Iterator, typename TerminationPredicate>
    class terminated_generator
    {
        static_assert(is_iterator_v<Iterator>);
        static_assert(!std::is_const_v<Iterator>);
        static_assert(std::is_invocable_r_v<bool, TerminationPredicate, iterator_value_type<Iterator>>);

    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type<Iterator>;
        using difference_type = iterator_difference_type<Iterator>;
        using pointer = iterator_pointer_type<Iterator>;
        using reference = iterator_reference_type<Iterator>;
        using iterator = Iterator;

    public:
        explicit terminated_generator(const iterator& i, const TerminationPredicate& term)
            : _i(i), _term(term) { }
        template <typename T1, typename T2,
            STDEXT_REQUIRES(std::is_constructible_v<iterator, T1> && std::is_constructible_v<TerminationPredicate, T2>)>
        explicit terminated_generator(T1&& i, T2&& term)
            : _i(stdext::forward<T1>(i)), _term(stdext::forward<T2>(term)) { }

    public:
        friend bool operator == (const terminated_generator& a, const terminated_generator& b)
        {
            return a._i == b._i && a._term == b._term;
        }

        friend bool operator != (const terminated_generator& a, const terminated_generator& b)
        {
            return a._i != b._i || a._term != b._term;
        }

        friend void swap(terminated_generator& a, terminated_generator& b)
            noexcept(std::is_nothrow_swappable_v<iterator> && std::is_nothrow_swappable_v<TerminationPredicate>)
        {
            swap(a._i, b._i);
            swap(a._term, b._term);
        }

    public:
        reference operator * () const { assert(!_term(*_i)); return *_i; }
        const iterator& operator -> () const { assert(!_term(*_i)); return _i; }
        terminated_generator& operator ++ () { assert(!_term(*_i)); ++_i; return *this; }
        decltype(auto) operator ++ (int) { assert(!_term(*_i)); return _i++; }
        explicit operator bool () const { return !_term(*_i); }
        const iterator& base() const noexcept { return _i; }
        const TerminationPredicate& predicate() const noexcept { return _term; }

    private:
        iterator _i;
        TerminationPredicate _term;
    };

    template <typename Iterator, typename Sentinel, typename I = std::decay_t<Iterator>, typename S = std::decay_t<Sentinel>,
        STDEXT_REQUIRES(is_iterator_v<I> && is_sentinel_for_v<S, I>)>
    auto make_generator(Iterator&& i, Sentinel&& j)
    {
        return delimited_iterator_generator<I, S>(stdext::forward<Iterator>(i), stdext::forward<Sentinel>(j));
    }

    template <typename Range,
        STDEXT_REQUIRES(is_std_input_range_v<Range>)>
    auto make_generator(Range& r)
    {
        return delimited_iterator_generator(begin(r), end(r));
    }

    template <typename Iterator, typename I = std::decay_t<Iterator>,
        STDEXT_REQUIRES(is_iterator_v<I>)>
    auto make_generator(Iterator&& i, iterator_size_type<I> n)
    {
        return counted_iterator_generator<I>(stdext::forward<Iterator>(i), n);
    }

    template <typename Function, typename F = std::decay_t<Function>, typename R = decltype(std::declval<F&>()()),
        STDEXT_REQUIRES(!std::is_void_v<R> && !is_std_input_range_v<Function>)>
    auto make_generator(Function&& function)
    {
        return function_generator<F>(stdext::forward<Function>(function));
    }

    template <typename T, typename U = remove_cvref_t<T>>
    auto make_constant_generator(T&& v)
    {
        return constant_generator<U>(stdext::forward<T>(v));
    }

    template <typename Iterator, typename TerminationPredicate, typename I = std::decay_t<Iterator>, typename P = std::decay_t<TerminationPredicate>,
        STDEXT_REQUIRES(is_iterator_v<I> && std::is_invocable_r_v<bool, P, iterator_value_type<I>>)>
    auto make_terminated_generator(Iterator&& i, TerminationPredicate&& term)
    {
        return terminated_generator<I, P>(stdext::forward<Iterator>(i), stdext::forward<TerminationPredicate>(term));
    }

    namespace _private
    {
        template <typename T> true_type test_generator_adaptable(decltype(make_generator(declval<T>()))*);
        template <typename T> false_type test_generator_adaptable(...);
    };
    template <typename T> constexpr auto is_generator_adaptable_v = decltype(_private::test_generator_adaptable<T>(nullptr))::value;
    template <typename T> struct is_generator_adaptable : bool_constant<is_generator_adaptable_v<T>> { };

    template <typename T> struct can_generate : std::disjunction<is_generator<remove_cvref_t<T>>, is_generator_adaptable<T>> { };
    template <typename T> constexpr auto can_generate_v = can_generate<T>::value;

    template <typename T, STDEXT_REQUIRES(is_generator_v<remove_cvref_t<T>>)>
    T&& as_generator(T&& g)
    {
        return stdext::forward<T>(g);
    }

    template <typename T, STDEXT_REQUIRES(is_generator_adaptable_v<T>)>
    auto as_generator(T&& g)
    {
        return make_generator(stdext::forward<T>(g));
    }

    template <typename T> using generator_type = std::remove_reference_t<decltype(as_generator(declval<T>()))>;
}

template <typename Producer, typename Consumer,
    STDEXT_REQUIRES(stdext::can_generate_v<Producer> && stdext::is_consumer_v<stdext::remove_cvref_t<Consumer>, stdext::generator_value_type<stdext::generator_type<Producer>>>)>
bool operator >> (Producer&& p, Consumer&& c)
{
    for (auto&& gen = stdext::as_generator(stdext::forward<Producer>(p)); gen; ++gen)
    {
        if (!c(*gen))
            return false;
    }

    return true;
}

#endif
