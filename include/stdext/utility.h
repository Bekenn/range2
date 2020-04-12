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

#include <stdext/detail/config.h>

#include <exception>
#include <type_traits>
#include <utility>

#include <cstddef>


#define STDEXT_CONCAT_(a, b) a ## b
#define STDEXT_CONCAT(a, b) STDEXT_CONCAT_(a, b)

namespace stdext
{
    // Useful declarations
    using std::swap;
    using std::move;
    using std::forward;
    using std::exchange;
    using std::declval;

    // Discards a value; can be used to silence warnings about unused entities.
    template <class... Ts>
    constexpr void discard(Ts&&...) noexcept
    {
    }

    // Returns the length of an array.
    template <class T, size_t size>
    constexpr size_t lengthof(T (&)[size]) noexcept
    {
        return size;
    }

    // Run-time invocation of unreachable results in undefined behavior.
    [[noreturn]] inline void unreachable() noexcept
    {
#ifndef NDEBUG
        std::abort();
#endif
#if STDEXT_COMPILER_GCC
        __builtin_unreachable();
#endif
    }

    template <class Callback>
    class scope_guard
    {
    public:
        explicit scope_guard(const Callback& cb) : _cb(cb), _valid(true) { }
        explicit scope_guard(Callback&& cb) : _cb(move(cb)), _valid(true) { }

        scope_guard(const scope_guard&) = delete;
        scope_guard& operator = (const scope_guard&) = delete;

        scope_guard(scope_guard&& other)
            : _cb(move(other._cb)), _valid(exchange(other._valid, false))
        {
        }

        scope_guard& operator = (scope_guard&& other)
        {
            _cb = move(other._cb);
            _valid = exchange(other._valid, false);
        }

        ~scope_guard() noexcept(noexcept(_cb()))
        {
            if (_valid)
                _cb();
        }

    public:
        void invalidate() noexcept
        {
            _valid = false;
        }

    private:
        Callback _cb;
        bool _valid;
    };

    template <class Callback>
    class scope_error_guard : public scope_guard<Callback>
    {
    public:
        explicit scope_error_guard(const Callback& cb, bool call_on_error)
            : scope_guard<Callback>(cb), _exception_count(std::uncaught_exceptions()), _call_on_error(call_on_error)
        {
        }

        explicit scope_error_guard(Callback&& cb, bool call_on_error)
            : scope_guard<Callback>(move(cb)), _exception_count(std::uncaught_exceptions()), _call_on_error(call_on_error)
        {
        }

        scope_error_guard(scope_error_guard&&) = default;
        scope_error_guard& operator = (scope_error_guard&&) = default;

        ~scope_error_guard() noexcept(noexcept(std::declval<Callback>()()))
        {
            bool error = std::uncaught_exceptions() != _exception_count;
            if (_call_on_error != error)
                this->invalidate();
        }

    private:
        int _exception_count;
        bool _call_on_error;
    };

    template <class Callback>
    auto make_scope_guard(Callback&& cb)
    {
        return scope_guard<std::decay_t<Callback>>(forward<Callback>(cb));
    }

    template <class Callback>
    auto make_scope_error_guard(Callback&& cb, bool call_on_error)
    {
        return scope_error_guard<std::decay_t<Callback>>(forward<Callback>(cb), call_on_error);
    }

#define at_scope_exit(...) auto STDEXT_CONCAT(scope_guard_, __COUNTER__) = ::stdext::make_scope_guard(__VA_ARGS__)
#define at_scope_success(...) auto STDEXT_CONCAT(scope_success_, __COUNTER__) = ::stdext::make_scope_error_guard(__VA_ARGS__, false)
#define at_scope_error(...) auto STDEXT_CONCAT(scope_error_, __COUNTER__) = ::stdext::make_scope_error_guard(__VA_ARGS__, true)

    template <class T, bool = std::is_empty<T>::value>
    class compressed_base;

    template <class T1, class T2, bool = std::is_empty<T1>::value, bool = std::is_empty<T2>::value>
    class compressed_pair;

    template <class T>
    class compressed_base<T, false>
    {
    public:
        using type = T;

    public:
        compressed_base() = default;
        explicit compressed_base(const T& v) noexcept(std::is_nothrow_copy_constructible<T>::value) : value(v) { }
        explicit compressed_base(T&& v) noexcept(std::is_nothrow_move_constructible<T>::value) : value(std::move(v)) { }

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
        compressed_pair(const T1& v1, T2&& v2) : v1(v1), v2(std::move(v2)) { }
        compressed_pair(T1&& v1, const T2& v2) : v1(std::move(v1)), v2(v2) { }
        compressed_pair(T1&& v1, T2&& v2) : v1(std::move(v1)), v2(std::move(v2)) { }

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
        compressed_pair(const T1& v1, T2&& v2) : T2(std::move(v2)), v1(v1) { }
        compressed_pair(T1&& v1, const T2& v2) : T2(v2), v1(std::move(v1)) { }
        compressed_pair(T1&& v1, T2&& v2) : T2(std::move(v2)), v1(std::move(v1)) { }

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
        compressed_pair(const T1& v1, T2&& v2) : T1(v1), v2(std::move(v2)) { }
        compressed_pair(T1&& v1, const T2& v2) : T1(std::move(v1)), v2(v2) { }
        compressed_pair(T1&& v1, T2&& v2) : T1(std::move(v1)), v2(std::move(v2)) { }

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
        compressed_pair(const T1& v1, T2&& v2) : T1(v1), T2(std::move(v2)) { }
        compressed_pair(T1&& v1, const T2& v2) : T1(std::move(v1)), T2(v2) { }
        compressed_pair(T1&& v1, T2&& v2) : T1(std::move(v1)), T2(std::move(v2)) { }

        T1& first() noexcept { return static_cast<T1&>(*this); }
        const T1& first() const noexcept { return static_cast<const T1&>(*this); }
        T2& second() noexcept { return static_cast<T2&>(*this); }
        const T2& second() const noexcept { return static_cast<const T2&>(*this); }

        void swap(compressed_pair& p) noexcept { }
    };

    namespace detail
    {
        template <class T, int>
        struct unique_base : T
        {
            using T::T;
        };
    }

    template <class T>
    class compressed_pair<T, T, true, true> : detail::unique_base<T, 1>, detail::unique_base<T, 2>
    {
    public:
        using first_type = T;
        using second_type = T;
    private:
        using base1 = detail::unique_base<T, 1>;
        using base2 = detail::unique_base<T, 2>;

    public:
        compressed_pair() : base1(), base2() { }
        compressed_pair(const T& v1, const T& v2) : base1(v1), base2(v2) { }
        compressed_pair(const T& v1, T&& v2) : base1(v1), base2(std::move(v2)) { }
        compressed_pair(T&& v1, const T& v2) : base1(std::move(v1)), base2(v2) { }
        compressed_pair(T&& v1, T&& v2) : base1(std::move(v1)), base2(std::move(v2)) { }

        T& first() noexcept { return static_cast<base1&>(*this); }
        const T& first() const noexcept { return static_cast<const base1&>(*this); }
        T& second() noexcept { return static_cast<base2&>(*this); }
        const T& second() const noexcept { return static_cast<const base2&>(*this); }

        void swap(compressed_pair& p) noexcept { }
    };

    template <class T1, class T2>
    void swap(compressed_pair<T1, T2>& a, compressed_pair<T1, T2>& b) noexcept(noexcept(a.swap(b)))
    {
        a.swap(b);
    }
}

#endif
