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


// Generators are input iterators that know when the underlying sequence is exhausted. The user
// can test for this by applying a conversion to bool. To distinguish generators from other
// iterators, all generators should have an iterator_category of generator_tag.
namespace stdext
{
    struct generator_tag : std::forward_iterator_tag { };

    namespace _private
    {
        template <typename T, bool = is_iterator<T>::value>
        struct is_generator_helper { static constexpr bool value = false; };
        template <typename T>
        struct is_generator_helper<T, true>
        {
            static constexpr bool value = std::is_base_of<iterator_category<T>, generator_tag>::value;
        };
    }

    template <typename T> struct is_generator
        : std::conditional_t<_private::is_generator_helper<T>::value, true_type, false_type>
    { };
    template <typename T> constexpr auto is_generator_v = is_generator<T>::value;

    template <typename T> struct is_generator_adaptable
    {
        template <typename U> static true_type test(decltype(make_generator(declval<U>()))*);
        template <typename U> static false_type test(...);
        static constexpr bool value = decltype(test<T>(nullptr))::value;
    };
    template <typename T> constexpr auto is_generator_adaptable_v = is_generator_adaptable<T>::value;

    template <typename T> struct can_generate
        : std::conditional_t<is_generator<remove_cvref_t<T>>::value || is_generator_adaptable<T>::value,
            true_type,
            false_type>
    { };
    template <typename T> constexpr auto can_generate_v = can_generate<T>::value;

    template <typename T, STDEXT_REQUIRES(is_generator<remove_cvref_t<T>>::value)>
    decltype(auto) as_generator(T&& g)
    {
        return stdext::forward<T>(g);
    }

    template <typename T, STDEXT_REQUIRES(is_generator_adaptable<T>::value)>
    auto as_generator(T&& g)
    {
        return make_generator(stdext::forward<T>(g));
    }

    template <typename T> using generator_type = std::remove_reference_t<decltype(as_generator(declval<T>()))>;

    template <typename T> using generator_value_type = std::remove_cv_t<typename std::iterator_traits<T>::value_type>;
    template <typename T> using generator_difference_type = typename std::iterator_traits<T>::difference_type;
    template <typename T> using generator_pointer_type = typename std::iterator_traits<T>::pointer;
    template <typename T> using generator_reference_type = typename std::iterator_traits<T>::reference;

    template <typename Generator, typename Consumer, STDEXT_REQUIRES(is_consumer_v<std::decay_t<Consumer>, generator_value_type<generator_type<Generator>>>)>
    bool operator >> (Generator&& g, Consumer&& c)
    {
        for (decltype(auto) gen = as_generator(stdext::forward<Generator>(g)); gen; ++gen)
        {
            if (!c(*gen))
                return false;
        }

        return true;
    }

    template <typename Generator, typename ValueType>
    class basic_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = ValueType;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        reference operator * () const { return self().get(); }
        pointer operator -> () const { return &self().get(); }
        Generator& operator ++ () { self().next(); return self(); }
        auto operator ++ (int) { iterator_proxy<Generator> temp(self().get()); self().next(); return temp; }
        explicit operator bool() const { return !self().done(); }

    private:
        const Generator& self() const noexcept { return static_cast<const Generator&>(*this); }
        Generator& self() noexcept { return static_cast<Generator&>(*this); }
    };

    template <typename Iterator, typename Sentinel>
    class delimited_iterator_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type<Iterator>;
        using difference_type = iterator_difference_type<Iterator>;
        using pointer = iterator_pointer_type<Iterator>;
        using reference = iterator_reference_type<Iterator>;
        using iterator = Iterator;
        using sentinel = Sentinel;

    public:
        delimited_iterator_generator() : i(), j() { }
        delimited_iterator_generator(const iterator& i, const sentinel& j) : i(i), j(j) { }
        delimited_iterator_generator(const iterator& i, sentinel&& j) : i(i), j(stdext::move(j)) { }
        delimited_iterator_generator(iterator&& i, const sentinel& j) : i(stdext::move(i)), j(j) { }
        delimited_iterator_generator(iterator&& i, sentinel&& j) : i(stdext::move(i)), j(stdext::move(j)) { }

    public:
        friend bool operator == (const delimited_iterator_generator& a, const delimited_iterator_generator& b) noexcept
        {
            return a.i == b.i && a.j == b.j;
        }
        friend bool operator != (const delimited_iterator_generator& a, const delimited_iterator_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(delimited_iterator_generator& a, delimited_iterator_generator& b)
        {
            swap(a.i, b.i);
            swap(a.j, b.j);
        }

    public:
        reference operator * () const { return *i; }
        pointer operator -> () const { return i.operator -> (); }
        delimited_iterator_generator& operator ++ () { ++i; return *this; }
        decltype(auto) operator ++ (int) { return i++; }
        explicit operator bool () const { return i != j; }
        const iterator& base() const { return i; }

    private:
        iterator i;
        sentinel j;
    };

    template <typename Function, typename R = decltype(std::declval<Function&>()())>
    class function_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = std::remove_cv_t<R>;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        function_generator() : f(), value() { }
        explicit function_generator(const Function& f) : f(f), value(this->f()) { }
        explicit function_generator(Function&& f) : f(stdext::move(f)), value(this->f()) { }

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
        value_type value;
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
        function_generator() : f(), opt() { }
        explicit function_generator(const Function& f) : f(f), opt(this->f()) { }
        explicit function_generator(Function&& f) : f(stdext::move(f)), opt(this->f()) { }

    public:
        friend bool operator == (const function_generator& a, const function_generator& b) noexcept
        {
            return a.f == b.f
                && a.opt == b.opt;
        }
        friend bool operator != (const function_generator& a, const function_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(function_generator&& a, function_generator&& b)
        {
            swap(a.f, b.f);
            swap(a.opt, b.opt);
        }

    public:
        reference operator * () const { return opt.value(); }
        pointer operator -> () const { return &opt.value(); }
        function_generator& operator ++ () { opt = f(); return *this; }
        iterator_proxy<function_generator> operator ++ (int)
        {
            iterator_proxy<function_generator> proxy(opt.value());
            ++*this;
            return proxy;
        }
        explicit operator bool() const { return opt.has_value(); }

    private:
        Function f;
        optional<value_type> opt;
    };

    template <typename T>
    class constant_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        constant_generator() : v() { }
        explicit constant_generator(const value_type& v)
            noexcept(std::is_nothrow_copy_constructible<value_type>::value)
            : v(v) { }
        explicit constant_generator(value_type&& v)
            noexcept(std::is_nothrow_move_constructible<value_type>::value)
            : v(stdext::move(v)) { }

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

    template <typename Iterator, typename TerminationPredicate>
    class terminated_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = iterator_value_type<Iterator>;
        using difference_type = iterator_difference_type<Iterator>;
        using pointer = iterator_pointer_type<Iterator>;
        using reference = iterator_reference_type<Iterator>;
        using iterator = Iterator;

    public:
        terminated_generator() : i(), term() { }
        terminated_generator(const iterator& g, const TerminationPredicate& term)
            : i(g), term(term) { }
        terminated_generator(const iterator& g, TerminationPredicate&& term)
            : i(g), term(stdext::move(term)) { }
        terminated_generator(iterator&& g, const TerminationPredicate& term)
            : i(stdext::move(g)), term(term) { }
        terminated_generator(iterator&& g, TerminationPredicate&& term)
            : i(stdext::move(g)), term(stdext::move(term)) { }

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

    template <typename Iterator, typename Sentinel,
        STDEXT_REQUIRES(is_iterator<std::decay_t<Iterator>>::value && is_equality_comparable_with<std::decay_t<Iterator>, std::decay_t<Sentinel>>::value)>
    auto make_generator(Iterator&& i, Sentinel&& j)
    {
        return delimited_iterator_generator<std::decay_t<Iterator>, std::decay_t<Sentinel>>(stdext::forward<Iterator>(i), stdext::forward<Sentinel>(j));
    }

    template <typename Function, STDEXT_REQUIRES(std::is_invocable_v<std::decay_t<Function>>)>
    auto make_generator(Function&& function)
    {
        return function_generator<std::decay_t<Function>>(stdext::forward<Function>(function));
    }

    template <typename T>
    auto make_constant_generator(T&& v)
    {
        return constant_generator<std::decay_t<T>>(stdext::forward<T>(v));
    }

    template <typename Iterator, typename TerminationPredicate,
        STDEXT_REQUIRES(is_iterator<std::decay_t<Iterator>>::value && std::is_invocable_r_v<bool, std::decay_t<TerminationPredicate>, iterator_value_type<std::decay_t<Iterator>>>)>
    auto make_terminated_generator(Iterator&& i, TerminationPredicate&& term)
    {
        return terminated_generator<std::decay_t<Iterator>, std::decay_t<TerminationPredicate>>
            (stdext::forward<Iterator>(i), stdext::forward<TerminationPredicate>(term));
    }
}

#endif
