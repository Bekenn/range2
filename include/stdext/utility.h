//
//  utility.h
//  stdext
//
//  Created by James Touton on 12/14/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_UTILITY_INCLUDED
#define STDEXT_UTILITY_INCLUDED
#pragma once

#include "concept.h"

#include <utility>


namespace stdext
{
    using ::std::swap;

    template <class T, bool = ::std::is_empty<T>::value>
    class compressed_base;

    template <class T1, class T2, bool = ::std::is_empty<T1>::value, bool = ::std::is_empty<T2>::value>
    class compressed_pair;

    template <class T>
    class compressed_base<T, false>
    {
    public:
        using type = T;

    public:
        compressed_base() = default;
        explicit compressed_base(const T& v) noexcept(::std::is_nothrow_copy_constructible<T>::value) : value(v) { }
        explicit compressed_base(T&& v) noexcept(::std::is_nothrow_move_constructible<T>::value) : value(::std::move(v)) { }

        friend bool operator == (const compressed_base& a, const compressed_base& b) noexcept
        {
            return a.value == b.value;
        }
        friend bool operator != (const compressed_base& a, const compressed_base& b) noexcept
        {
            return !(a == b);
        }

    public:
        const T& get() const noexcept { return value; }
        T& get() noexcept { return value; }

    private:
        T value;
    };

    template <class T>
    class compressed_base<T, true> : private T
    {
    public:
        using type = T;

    public:
        compressed_base() = default;
        explicit compressed_base(const T& v) : T(v) { }
        explicit compressed_base(T&& v) : T(v) { }

        friend bool operator == (const compressed_base& a, const compressed_base& b) noexcept
        {
            return static_cast<const T&>(a) == static_cast<const T&>(b);
        }
        friend bool operator != (const compressed_base& a, const compressed_base& b) noexcept
        {
            return !(a == b);
        }

    public:
        const T& get() const noexcept { return static_cast<const T&>(*this); }
        T&& get() noexcept { return static_cast<T&>(*this); }
    };

    template <class T1, class T2>
    class compressed_pair<T1, T2, false, false>
    {
    public:
        using first_type = T1;
        using second_type = T2;

    public:
        compressed_pair() : v1(), v2() { }
        compressed_pair(const T1& v1, const T2& v2) : v1(v1), v2(v2) { }
        compressed_pair(const T1& v1, T2&& v2) : v1(v1), v2(::std::move(v2)) { }
        compressed_pair(T1&& v1, const T2& v2) : v1(::std::move(v1)), v2(v2) { }
        compressed_pair(T1&& v1, T2&& v2) : v1(::std::move(v1)), v2(::std::move(v2)) { }

        T1& first() noexcept { return v1; }
        const T1& first() const noexcept { return v1; }
        T2& second() noexcept { return v2; }
        const T2& second() const noexcept { return v2; }

        void swap(compressed_pair& p)
            noexcept(noexcept(swap(v1, p.v1)) && noexcept(swap(v2, p.v2)))
        {
            swap(v1, p.v1);
            swap(v2, p.v2);
        }

    private:
        T1 v1;
        T2 v2;
    };

    template <class T1, class T2>
    class compressed_pair<T1, T2, false, true> : T2
    {
    public:
        using first_type = T1;
        using second_type = T2;

    public:
        compressed_pair() : T2(), v1() { }
        compressed_pair(const T1& v1, const T2& v2) : T2(v2), v1(v1) { }
        compressed_pair(const T1& v1, T2&& v2) : T2(::std::move(v2)), v1(v1) { }
        compressed_pair(T1&& v1, const T2& v2) : T2(v2), first(::std::v1(v1)) { }
        compressed_pair(T1&& v1, T2&& v2) : T2(::std::move(v2)), v1(::std::move(v1)) { }

        T1& first() noexcept { return v1; }
        const T1& first() const noexcept { return v1; }
        T2& second() noexcept { return static_cast<T2&>(*this); }
        const T2& second() const noexcept { return static_cast<const T2&>(*this); }

        void swap(compressed_pair& p)
            noexcept(noexcept(swap(v1, p.v1)))
        {
            swap(v1, p.v1);
        }

    private:
        T1 v1;
    };

    template <class T1, class T2>
    class compressed_pair<T1, T2, true, false> : T1
    {
    public:
        using first_type = T1;
        using second_type = T2;

    public:
        compressed_pair() : T1(), v2() { }
        compressed_pair(const T1& v1, const T2& v2) : T1(v1), v2(v2) { }
        compressed_pair(const T1& v1, T2&& v2) : T1(v1), v2(::std::move(v2)) { }
        compressed_pair(T1&& v1, const T2& v2) : T1(::std::move(v1)), v2(v2) { }
        compressed_pair(T1&& v1, T2&& v2) : T1(::std::move(v1)), v2(::std::move(v2)) { }

        T1& first() noexcept { return static_cast<T1&>(*this); }
        const T1& first() const noexcept { return static_cast<const T1&>(*this); }
        T2& second() noexcept { return v2; }
        const T2& second() const noexcept { return v2; }

        void swap(compressed_pair& p)
            noexcept(noexcept(swap(v2, p.v2)))
        {
            swap(v2, p.v2);
        }

    private:
        T2 v2;
    };

    template <class T1, class T2>
    class compressed_pair<T1, T2, true, true> : T1, T2
    {
    public:
        using first_type = T1;
        using second_type = T2;

    public:
        compressed_pair() : T1(), T2() { }
        compressed_pair(const T1& v1, const T2& v2) : T1(v1), T2(v2) { }
        compressed_pair(const T1& v1, T2&& v2) : T1(v1), T2(::std::move(v2)) { }
        compressed_pair(T1&& v1, const T2& v2) : T1(::std::move(v1)), T2(v2) { }
        compressed_pair(T1&& v1, T2&& v2) : T1(::std::move(v1)), T2(::std::move(v2)) { }

        T1& first() noexcept { return static_cast<T1&>(*this); }
        const T1& first() const noexcept { return static_cast<const T1&>(*this); }
        T2& second() noexcept { return static_cast<T2&>(*this); }
        const T2& second() const noexcept { return static_cast<const T2&>(*this); }

        void swap(compressed_pair& p) noexcept { }
    };

    template <class T>
    class compressed_pair<T, T, true, true> : T
    {
    public:
        using first_type = T;
        using second_type = T;

    public:
        compressed_pair() : T(), T() { }
        compressed_pair(const T& v1, const T& v2) : T(v1), T(v2) { }
        compressed_pair(const T& v1, T&& v2) : T(v1), T(::std::move(v2)) { }
        compressed_pair(T&& v1, const T& v2) : T(::std::move(v1)), T(v2) { }
        compressed_pair(T&& v1, T&& v2) : T(::std::move(v1)), T(::std::move(v2)) { }

        T& first() noexcept { return static_cast<T&>(*this); }
        const T& first() const noexcept { return static_cast<const T&>(*this); }
        T& second() noexcept { return static_cast<T&>(*this); }
        const T& second() const noexcept { return static_cast<const T&>(*this); }

        void swap(compressed_pair& p) noexcept { }
    };

    template <class T1, class T2>
    void swap(compressed_pair<T1, T2>& a, compressed_pair<T1, T2>& b) noexcept(noexcept(a.swap(b)))
    {
        a.swap(b);
    }
}

#endif
