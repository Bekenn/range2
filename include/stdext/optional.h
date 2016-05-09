#ifndef STDEXT_OPTIONAL_INCLUDED
#define STDEXT_OPTIONAL_INCLUDED
#pragma once

#include "concept.h"

#include <initializer_list>
#include <new>
#include <stdexcept>

#include <cstddef>


namespace stdext
{
    // 5.3, optional for object types
    template <class T> class optional;

    // 5.4, In-place construction
    struct in_place_t { };
    constexpr in_place_t in_place{};

    // 5.5, No-value state indicator
    struct nullopt_t { };
    constexpr nullopt_t nullopt{};

    // 5.6, Class bad_optional_access
    class bad_optional_access : public ::std::logic_error
    {
        bad_optional_access() : logic_error("attempted to access empty optional") { }
    };

    template <class T, bool TriviallyDestructible>
    struct optional_base
    {
        ::std::aligned_storage_t<sizeof(T), alignof(T)> storage;
        bool initialized;
    };
    template <class T>
    struct optional_base<T, true>
    {
        ~optional_base()
        {
            if (initialized)
                reinterpret_cast<T&>(storage).~T();
        }

        ::std::aligned_storage_t<sizeof(T), alignof(T)> storage;
        bool initialized;
    };

    template <class T>
    class optional : private optional_base<T, ::std::is_trivially_destructible<T>::value>
    {
        using optional_base<T, ::std::is_trivially_destructible<T>::value>::initialized;
    public:
        typedef T value_type;

        // 5.3.1, Constructors
        constexpr optional() noexcept { initialized = false; }
        constexpr optional(nullopt_t) noexcept { initialized = false; }
        optional(const optional& rhs)
        {
            if ((initialized = rhs.initialized))
                construct(rhs.value());
        }
        optional(optional&& rhs) noexcept(::std::is_nothrow_move_constructible<T>::value)
        {
            if ((initialized = ::std::move(rhs.initialized)))
                construct(::std::move(rhs.value()));
        }
        constexpr optional(const T& v)
        {
            initialized = true;
            construct(v);
        }
        constexpr optional(T&& v)
        {
            initialized = true;
            construct(::std::move(v));
        }
        template <class... Args>
        constexpr explicit optional(in_place_t, Args&&... args)
        {
            initialized = true;
            construct(::std::forward<Args>(args)...);
        }
        template <class U, class... Args, REQUIRES(::std::is_constructible<T, ::std::initializer_list<U>, Args&&...>::value)>
        constexpr explicit optional(in_place_t, ::std::initializer_list<U> il, Args&&... args)
        {
            initialized = true;
            construct(il, ::std::forward<Args>(args)...);
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
        optional& operator=(optional&&) noexcept(::std::is_nothrow_move_assignable<T>::value)
        {
            if (initialized)
            {
                if (rhs.initialized)
                    value() = ::std::move(rhs.value());
                else
                {
                    destroy();
                    initialized = false;
                }
            }
            else if (rhs.initialized)
            {
                construct(::std::move(rhs.value()));
                initialized = true;
            }
            return *this;
        }

        template <class U> optional& operator=(U&& rhs)
        {
            if (initialized)
                value() = ::std::move(rhs);
            else
            {
                construct(::std::move(rhs));
                initialized = true;
            }
        }

        template <class... Args> void emplace(Args&&... args)
        {
            if (initialized)
                destroy();
            construct(::std::forward<Args>(args)...);
            initialized = true;
        }

        template <class U, class... Args>
        void emplace(::std::initializer_list<U> il, Args&&... args)
        {
            emplace(il, ::std::forward<Args>(args)...);
        }

        // 5.3.4, Swap
        void swap(optional& rhs)
            noexcept(::std::is_nothrow_move_constructible<T>::value
                && noexcept(swap(::std::declval<T&>(), ::std::declval<T&>())))
        {
            using ::std::swap;

            if (initialized)
            {
                if (rhs.initialized)
                    swap(value(), rhs.value());
                else
                {
                    rhs.emplace(::std::move(value()));
                    destroy();
                    initialized = false;
                }
            }
            else if (rhs.initialized)
            {
                emplace(::std::move(rhs.value()));
                rhs.destroy();
                rhs.initialized = false;
            }
        }

        // 5.3.5, Observers
        constexpr bool has_value() const { return initialized; }
        constexpr T const& value() const &
        {
            return initialized ? *reinterpret_cast<T&>(storage) : throw bad_optional_access();
        }
        constexpr T& value() &
        {
            return initialized ? *reinterpret_cast<T&>(storage) : throw bad_optional_access();
        }
        constexpr T&& value() &&
        {
            return initialized ? *reinterpret_cast<T&>(storage) : throw bad_optional_access();
        }
        constexpr const T&& value() const &&
        {
            return initialized ? *reinterpret_cast<T&>(storage) : throw bad_optional_access();
        }
        template <class U, REQUIRES(::std::is_copy_constructible<T>::value && ::std::is_convertible<U&&, T>::value)>
        constexpr T value_or(U&& v) const &
        {
            return initialized ? *reinterpret_cast<T&>(storage) : static_cast<T>(::std::forward<U>(v));
        }
        template <class U, REQUIRES(::std::is_move_constructible<T>::value && ::std::is_convertible<U&&, T>::value)>
        constexpr T value_or(U&& v) &&
        {
            return initialized ? ::std::move(*reinterpret_cast<T&>(storage)) : static_cast<T>(::std::forward<U>(v));
        }

    private:
        template <class... Args>
        void construct()
        {
            new(&storage) T(::std::forward<Args>(args)...);
        }

        void destroy()
        {
            reinterpret_cast<T&>(storage).~T();
        }
    };

    // 5.7, Relational operators
    template <class T>
    constexpr bool operator==(const optional<T>& x, const optional<T>& y)
    {
        return x.has_value() != y.has_value() ? false
            : !x.has_value() ? true
            : x.value() == y.value();
    }
    template <class T>
    constexpr bool operator!=(const optional<T>& x, const optional<T>& y)
    {
        return !(x == y);
    }
    template <class T>
    constexpr bool operator<(const optional<T>& x, const optional<T>& y)
    {
        return !y.has_value() ? false
            : !x.has_value() ? true
            : x.value() < y.value();
    }
    template <class T>
    constexpr bool operator>(const optional<T>& x, const optional<T>& y)
    {
        return y < x;
    }
    template <class T>
    constexpr bool operator<=(const optional<T>& x, const optional<T>& y)
    {
        return !x.has_value() ? true
            : !y.has_value() ? false
            : x.value() <= y.value();
    }
    template <class T>
    constexpr bool operator>=(const optional<T>& x, const optional<T>& y)
    {
        return y <= x;
    }

    // 5.8, Comparison with nullopt
    template <class T> constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept
    {
        return !x.has_value();
    }
    template <class T> constexpr bool operator==(nullopt_t, const optional<T>& x) noexcept
    {
        return !x.has_value();
    }
    template <class T> constexpr bool operator!=(const optional<T>& x, nullopt_t) noexcept
    {
        return x.has_value();
    }
    template <class T> constexpr bool operator!=(nullopt_t, const optional<T>& x) noexcept
    {
        return x.has_value();
    }
    template <class T> constexpr bool operator<(const optional<T>& x, nullopt_t) noexcept
    {
        return false;
    }
    template <class T> constexpr bool operator<(nullopt_t, const optional<T>& x) noexcept
    {
        return x.has_value();
    }
    template <class T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept
    {
        return !x.has_value();
    }
    template <class T> constexpr bool operator<=(nullopt_t, const optional<T>& x) noexcept
    {
        return true;
    }
    template <class T> constexpr bool operator>(const optional<T>& x, nullopt_t) noexcept
    {
        return x.has_value();
    }
    template <class T> constexpr bool operator>(nullopt_t, const optional<T>& x) noexcept
    {
        return false;
    }
    template <class T> constexpr bool operator>=(const optional<T>& x, nullopt_t) noexcept
    {
        return true;
    }
    template <class T> constexpr bool operator>=(nullopt_t, const optional<T>& x) noexcept
    {
        return x.has_value();
    }

    // 5.9, Comparison with T
    template <class T> constexpr bool operator==(const optional<T>& x, const T& v)
    {
        return !x.has_value() || x.value() == v;
    }
    template <class T> constexpr bool operator==(const T& v, const optional<T>& x)
    {
        return x == v;
    }
    template <class T> constexpr bool operator!=(const optional<T>& x, const T& v)
    {
        return !(x == v);
    }
    template <class T> constexpr bool operator!=(const T& v, const optional<T>& x)
    {
        return !(x == v)
    }
    template <class T> constexpr bool operator<(const optional<T>& x, const T& v)
    {
        return !x.has_value() || x.value() < v;
    }
    template <class T> constexpr bool operator<(const T& v, const optional<T>& x)
    {
        return x < v;
    }
    template <class T> constexpr bool operator<=(const optional<T>& x, const T& v)
    {
        return !x.has_value() || x.value() <= v;
    }
    template <class T> constexpr bool operator<=(const T& v, const optional<T>& x)
    {
        return x <= v;
    }
    template <class T> constexpr bool operator>(const optional<T>& x, const T& v)
    {
        return v < x;
    }
    template <class T> constexpr bool operator>(const T& v, const optional<T>& x)
    {
        return x < v;
    }
    template <class T> constexpr bool operator>=(const optional<T>& x, const T& v)
    {
        return v <= x;
    }
    template <class T> constexpr bool operator>=(const T& v, const optional<T>& x)
    {
        return x <= v;
    }

    // 5.10, Specialized algorithms
    template <class T>
    void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
    {
        x.swap(y);
    }
    template <class T>
    constexpr optional<::std::decay_t<T>> make_optional(T&& v)
    {
        return optional<::std::decay_t<T>>(::std::forward<T>(v));
    }
}

namespace std
{
    // 5.11, Hash support
    template <class T>
    struct hash<::stdext::optional<T>>
    {
        constexpr ::std::size_t operator () (const ::stdext::optional<T>& k) const
        {
            return k.has_value() ? hash<T>()(k.value()) : 0;
        }
    };
}

#endif