//
//  optional.h
//  stdext
//
//  Created by James Touton on 10/31/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_OPTIONAL_INCLUDED
#define STDEXT_OPTIONAL_INCLUDED
#pragma once

#include <stdext/utility.h>

#include <initializer_list>
#include <new>
#include <stdexcept>

#include <cstddef>

#if STDEXT_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4814)   // constexpr does not imply const in C++14
#endif


namespace stdext
{
    // 5.3, optional for object types
    template <typename T> class optional;

    // 5.4, In-place construction
    struct in_place_t { };
    constexpr in_place_t in_place;

    // 5.5, No-value state indicator
    struct nullopt_t { };
    constexpr nullopt_t nullopt;

    // 5.6, Class bad_optional_access
    class bad_optional_access : public std::logic_error
    {
    public:
        bad_optional_access() : logic_error("attempted to access empty optional") { }
    };

    template <typename T, bool TriviallyDestructible>
    struct optional_base
    {
        ~optional_base()
        {
            if (initialized)
                object.~T();
        }

        union
        {
            nullopt_t empty;
            T object;
        };
        bool initialized;
    };
    template <typename T>
    struct optional_base<T, true>
    {
        union
        {
            nullopt_t empty;
            T object;
        };
        bool initialized;
    };

    template <typename T>
    class optional : private optional_base<T, std::is_trivially_destructible<T>::value>
    {
        using optional_base<T, std::is_trivially_destructible<T>::value>::initialized;
    public:
        typedef T value_type;

        // 5.3.1, Constructors
        constexpr optional() noexcept { initialized = false; }
        constexpr optional(nullopt_t) noexcept { initialized = false; }
        optional(const optional& rhs)
        {
            initialized = rhs.initialized;
            if (initialized)
                construct(rhs.value());
        }
        optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible<T>::value)
        {
            initialized = stdext::move(rhs.initialized);
            if (initialized)
                construct(stdext::move(rhs.value()));
        }
        constexpr optional(const T& v)
        {
            initialized = true;
            construct(v);
        }
        constexpr optional(T&& v)
        {
            initialized = true;
            construct(stdext::move(v));
        }
        template <typename... Args>
        constexpr explicit optional(in_place_t, Args&&... args)
        {
            initialized = true;
            construct(stdext::forward<Args>(args)...);
        }
        template <typename U, typename... Args, STDEXT_REQUIRES(std::is_constructible<T, std::initializer_list<U>, Args&&...>::value)>
        constexpr explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
        {
            initialized = true;
            construct(il, stdext::forward<Args>(args)...);
        }

        // 5.3.3, Assignment
        optional& operator=(nullopt_t) noexcept
        {
            if (initialized)
            {
                destroy();
                initialized = false;
            }
            return *this;
        }
        optional& operator=(const optional& rhs)
        {
            if (initialized)
            {
                if (rhs.initialized)
                    value() = rhs.value();
                else
                {
                    destroy();
                    initialized = false;
                }
            }
            else if (rhs.initialized)
            {
                construct(rhs.value());
                initialized = true;
            }
            return *this;
        }
        optional& operator=(optional&& rhs) noexcept(std::is_nothrow_move_assignable<T>::value)
        {
            if (initialized)
            {
                if (rhs.initialized)
                    value() = stdext::move(rhs.value());
                else
                {
                    destroy();
                    initialized = false;
                }
            }
            else if (rhs.initialized)
            {
                construct(stdext::move(rhs.value()));
                initialized = true;
            }
            return *this;
        }

        template <typename U> optional& operator=(U&& rhs)
        {
            if (initialized)
                value() = stdext::move(rhs);
            else
            {
                construct(stdext::move(rhs));
                initialized = true;
            }
        }

        template <typename... Args> void emplace(Args&&... args)
        {
            if (initialized)
                destroy();
            construct(stdext::forward<Args>(args)...);
            initialized = true;
        }

        template <typename U, typename... Args>
        void emplace(std::initializer_list<U> il, Args&&... args)
        {
            emplace(il, stdext::forward<Args>(args)...);
        }

        // 5.3.4, Swap
        void swap(optional& rhs)
            noexcept(std::is_nothrow_move_constructible<T>::value
                && noexcept(swap(declval<T&>(), declval<T&>())))
        {
            if (initialized)
            {
                if (rhs.initialized)
                    swap(value(), rhs.value());
                else
                {
                    rhs.emplace(stdext::move(value()));
                    destroy();
                    initialized = false;
                }
            }
            else if (rhs.initialized)
            {
                emplace(stdext::move(rhs.value()));
                rhs.destroy();
                rhs.initialized = false;
            }
        }

        // 5.3.5, Observers
        constexpr bool has_value() const { return initialized; }
        constexpr const T& value() const &
        {
            if (!initialized)
                throw bad_optional_access();
            return this->object;
        }
        constexpr T& value() &
        {
            if (!initialized)
                throw bad_optional_access();
            return this->object;
        }
        constexpr T&& value() &&
        {
            if (!initialized)
                throw bad_optional_access();
            return stdext::move(this->object);
        }
        constexpr const T&& value() const &&
        {
            if (!initialized)
                throw bad_optional_access();
            return stdext::move(this->object);
        }
        template <typename U, STDEXT_REQUIRES(std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)>
        constexpr T value_or(U&& v) const &
        {
            return initialized ? this->object : static_cast<T>(stdext::forward<U>(v));
        }
        template <typename U, STDEXT_REQUIRES(std::is_move_constructible<T>::value && std::is_convertible<U&&, T>::value)>
        constexpr T value_or(U&& v) &&
        {
            return initialized ? stdext::move(this->object) : static_cast<T>(stdext::forward<U>(v));
        }

        // 5.7, Relational operators
        friend constexpr bool operator==(const optional& x, const optional& y)
        {
            return x.has_value() != y.has_value() ? false
                : !x.has_value() ? true
                : x.value() == y.value();
        }
        friend constexpr bool operator!=(const optional& x, const optional& y)
        {
            return !(x == y);
        }
        friend constexpr bool operator<(const optional& x, const optional& y)
        {
            return !y.has_value() ? false
                : !x.has_value() ? true
                : x.value() < y.value();
        }
        friend constexpr bool operator>(const optional& x, const optional& y)
        {
            return y < x;
        }
        friend constexpr bool operator<=(const optional& x, const optional& y)
        {
            return !x.has_value() ? true
                : !y.has_value() ? false
                : x.value() <= y.value();
        }
        friend constexpr bool operator>=(const optional& x, const optional& y)
        {
            return y <= x;
        }

        // 5.8, Comparison with nullopt
        friend constexpr bool operator==(const optional& x, nullopt_t) noexcept
        {
            return !x.has_value();
        }
        friend constexpr bool operator==(nullopt_t, const optional& x) noexcept
        {
            return !x.has_value();
        }
        friend constexpr bool operator!=(const optional& x, nullopt_t) noexcept
        {
            return x.has_value();
        }
        friend constexpr bool operator!=(nullopt_t, const optional& x) noexcept
        {
            return x.has_value();
        }
        friend constexpr bool operator<(const optional& x, nullopt_t) noexcept
        {
            return false;
        }
        friend constexpr bool operator<(nullopt_t, const optional& x) noexcept
        {
            return x.has_value();
        }
        friend constexpr bool operator<=(const optional& x, nullopt_t) noexcept
        {
            return !x.has_value();
        }
        friend constexpr bool operator<=(nullopt_t, const optional& x) noexcept
        {
            return true;
        }
        friend constexpr bool operator>(const optional& x, nullopt_t) noexcept
        {
            return x.has_value();
        }
        friend constexpr bool operator>(nullopt_t, const optional& x) noexcept
        {
            return false;
        }
        friend constexpr bool operator>=(const optional& x, nullopt_t) noexcept
        {
            return true;
        }
        friend constexpr bool operator>=(nullopt_t, const optional& x) noexcept
        {
            return x.has_value();
        }

        // 5.9, Comparison with T
        friend constexpr bool operator==(const optional& x, const T& v)
        {
            return !x.has_value() || x.value() == v;
        }
        friend constexpr bool operator==(const T& v, const optional& x)
        {
            return x == v;
        }
        friend constexpr bool operator!=(const optional& x, const T& v)
        {
            return !(x == v);
        }
        friend constexpr bool operator!=(const T& v, const optional& x)
        {
            return !(x == v);
        }
        friend constexpr bool operator<(const optional& x, const T& v)
        {
            return !x.has_value() || x.value() < v;
        }
        friend constexpr bool operator<(const T& v, const optional& x)
        {
            return x < v;
        }
        friend constexpr bool operator<=(const optional& x, const T& v)
        {
            return !x.has_value() || x.value() <= v;
        }
        friend constexpr bool operator<=(const T& v, const optional& x)
        {
            return x <= v;
        }
        friend constexpr bool operator>(const optional& x, const T& v)
        {
            return v < x;
        }
        friend constexpr bool operator>(const T& v, const optional& x)
        {
            return x < v;
        }
        friend constexpr bool operator>=(const optional& x, const T& v)
        {
            return v <= x;
        }
        friend constexpr bool operator>=(const T& v, const optional& x)
        {
            return x <= v;
        }

        // 5.10, Specialized algorithms
        friend void swap(optional& x, optional& y) noexcept(noexcept(x.swap(y)))
        {
            x.swap(y);
        }

    private:
        template <typename... Args>
        void construct(Args&&... args)
        {
            new(&this->object) T(stdext::forward<Args>(args)...);
        }

        void destroy()
        {
            this->object.~T();
        }
    };

    template <typename T>
    constexpr optional<std::decay_t<T>> make_optional(T&& v)
    {
        return optional<std::decay_t<T>>(stdext::forward<T>(v));
    }
}

namespace std
{
    // 5.11, Hash support
    template <typename T>
    struct hash<::stdext::optional<T>>
    {
        constexpr size_t operator () (const ::stdext::optional<T>& k) const
        {
            return k.has_value() ? hash<T>()(k.value()) : 0;
        }
    };
}

#if STDEXT_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif
