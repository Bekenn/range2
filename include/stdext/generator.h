#ifndef STDEXT_GENERATOR_INCLUDED
#define STDEXT_GENERATOR_INCLUDED
#pragma once

#include "concept.h"
#include "iterator.h"

#include <cassert>


// Generators are input iterators that know when the underlying sequence is exhausted. The user
// can test for this by applying a conversion to bool. To distinguish generators from other
// iterators, all generators should declare the member type generator_category, which should be
// an alias for basic_generator_tag.
namespace stdext
{
    namespace detail
    {
        DECLARE_HAS_INNER_TYPE(generator_category);
    }
    template <class Generator> struct is_generator
        : ::std::conditional_t<detail::HAS_INNER_TYPE(Generator, generator_category),
            ::std::true_type,
            ::std::false_type>
    { };

    // The one and only generator category.
    struct basic_generator_tag { };

    template <class Generator> using generator_category = typename Generator::generator_category;

    namespace detail
    {
        template <class Generator>
        struct value_type_of<Generator, is_generator> { using type = typename Generator::value_type; };
        template <class Generator>
        struct difference_type_of<Generator, is_generator> { using type = typename Generator::difference_type; };
        template <class Generator>
        struct pointer_type_of<Generator, is_generator> { using type = typename Generator::pointer; };
        template <class Generator>
        struct reference_type_of<Generator, is_generator> { using type = typename Generator::reference; };
    }

    template <class Function>
    class function_generator
    {
    public:
        using iterator_category = ::std::input_iterator_tag;
        using value_type = const ::std::remove_cv_t<decltype(::std::declval<Function>()())>;
        using difference_type = ::std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        function_generator() : f(), value() { }
        function_generator(const Function& f) : f(f), value(this->f()) { }
        function_generator(Function&& f) : f(::std::move(f)), value(this->f()) { }

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
            using ::std::swap;
            swap(a.f, b.f);
            swap(a.value, b.value);
        }

    public:
        reference operator * () { return value; }
        pointer operator -> () { return &value; }
        function_generator& operator ++ () { value = f(); return *this; }
        iterator_proxy<function_generator> operator ++ (int)
        {
            iterator_proxy<function_generator> proxy(value);
            ++*this;
            return proxy;
        }
        explicit operator bool () { return true; }

    private:
        Function f;
        ::std::remove_const_t<value_type> value;
    };

    template <class T>
    class constant_generator
    {
    public:
        using iterator_category = ::std::input_iterator_tag;
        using value_type = const T;
        using difference_type = ::std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using generator_category = basic_generator_tag;

    public:
        constant_generator() : v() { }
        explicit constant_generator(const value_type& v)
            noexcept(::std::is_nothrow_copy_constructible<value_type>::value)
            : v(v) { }
        explicit constant_generator(value_type&& v)
            noexcept(::std::is_nothrow_move_constructible<value_type>::value)
            : v(::std::move(v)) { }

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
            using ::std::swap;
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

    template <class T>
    auto make_constant_generator(T&& v)
    {
        return constant_generator<::std::decay_t<T>>(::std::forward<T>(v));
    }
}

#endif
