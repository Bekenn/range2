#ifndef STDEXT_OPTIONAL_INCLUDED
#define STDEXT_OPTIONAL_INCLUDED
#pragma once

#include <stdext/_impl/enable_copy_move.h>

#include <stdext/traits.h>
#include <stdext/utility.h>

#include <exception>
#include <initializer_list>
#include <new>

#include <cassert>


namespace stdext
{
    // class template optional
    template <typename T>
    class optional;

    namespace _private
    {
        struct tag_init_t { explicit tag_init_t() = default; };
        inline constexpr tag_init_t tag_init;
    };

    // in-place construction
    struct in_place_t
    {
        explicit constexpr in_place_t(_private::tag_init_t) noexcept { }
    };
    inline constexpr in_place_t in_place(_private::tag_init);

    // no-value state indicator
    struct nullopt_t
    {
        explicit constexpr nullopt_t(_private::tag_init_t) noexcept { }
    };
    inline constexpr nullopt_t nullopt(_private::tag_init);

    // class bad_optional_access
    class bad_optional_access : public std::exception
    {
    public:
        const char* what() const noexcept override { return "attempted to access empty optional"; }
    };

    namespace _private
    {
        template <typename T> struct is_optional : false_type { };
        template <typename T> struct is_optional<optional<T>> : true_type { };
        template <typename T> inline constexpr bool is_optional_v = is_optional<T>::value;

        template <typename T, typename U> using is_constructible_from_optional =
            std::disjunction<
                std::is_constructible<T, optional<U>&>,
                std::is_constructible<T, optional<U>&&>,
                std::is_constructible<T, const optional<U>&>,
                std::is_constructible<T, const optional<U>&&>>;
        template <typename T, typename U> inline constexpr bool is_constructible_from_optional_v = is_constructible_from_optional<T, U>::value;

        template <typename T, typename U> using is_convertible_from_optional =
            std::disjunction<
                std::is_convertible<optional<U>&, T>,
                std::is_convertible<optional<U>&&, T>,
                std::is_convertible<const optional<U>&, T>,
                std::is_convertible<const optional<U>&&, T>>;
        template <typename T, typename U> inline constexpr bool is_convertible_from_optional_v = is_convertible_from_optional<T, U>::value;

        template <typename T, typename U> using is_assignable_from_optional =
            std::disjunction<
                std::is_assignable<T&, optional<U>&>,
                std::is_assignable<T&, optional<U>&&>,
                std::is_assignable<T&, const optional<U>&>,
                std::is_assignable<T&, const optional<U>&&>>;
        template <typename T, typename U> inline constexpr bool is_assignable_from_optional_v = is_assignable_from_optional<T, U>::value;

        template <typename T, bool = std::is_trivially_destructible_v<T>>
        struct optional_storage_base;

        template <typename T>
        struct optional_storage_base<T, false>
        {
            union
            {
                nullopt_t empty = nullopt;
                T value;
            };

            bool has_value = false;

            constexpr optional_storage_base() noexcept { }
            template <typename... Args, STDEXT_REQUIRES(std::is_constructible_v<T, Args...>)>
            constexpr explicit optional_storage_base(in_place_t, Args&&... args) : value(stdext::forward<Args>(args)...), has_value(true) { }
            ~optional_storage_base() { if (has_value) { value.~T(); } }

            template <typename... Args> T& emplace(Args&&... args);
            void reset() noexcept;
        };

        template <typename T>
        struct optional_storage_base<T, true>
        {
            union
            {
                nullopt_t empty = nullopt;
                T value;
            };

            bool has_value = false;

            constexpr optional_storage_base() noexcept { }
            template <typename... Args, STDEXT_REQUIRES(std::is_constructible_v<T, Args...>)>
            constexpr explicit optional_storage_base(in_place_t, Args&&... args) : value(stdext::forward<Args>(args)...), has_value(true) { }

            template <typename... Args> T& emplace(Args&&... args);
            void reset() noexcept;
        };

        template <typename T, bool = std::is_trivially_copy_constructible_v<T>, bool = std::is_trivially_move_constructible_v<T>>
        struct optional_storage_construct : optional_storage_base<T>
        {
            optional_storage_construct() = default;
            optional_storage_construct(const optional_storage_construct& rhs) { if (rhs.has_value) this->emplace(rhs.value); }
            optional_storage_construct(optional_storage_construct&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>) { if (rhs.has_value) this->emplace(stdext::move(rhs.value)); }
            using optional_storage_base<T>::optional_storage_base;
        };

        template <typename T>
        struct optional_storage_construct<T, false, true> : optional_storage_base<T>
        {
            optional_storage_construct() = default;
            optional_storage_construct(const optional_storage_construct& rhs) { if (rhs.has_value) this->emplace(rhs.value); }
            optional_storage_construct(optional_storage_construct&&) = default;
            using optional_storage_base<T>::optional_storage_base;
        };

        template <typename T>
        struct optional_storage_construct<T, true, false> : optional_storage_base<T>
        {
            optional_storage_construct() = default;
            optional_storage_construct(const optional_storage_construct& rhs) = default;
            optional_storage_construct(optional_storage_construct&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>) { if (rhs.has_value) this->emplace(stdext::move(rhs.value)); }
            using optional_storage_base<T>::optional_storage_base;
        };

        template <typename T>
        struct optional_storage_construct<T, true, true> : optional_storage_base<T>
        {
            optional_storage_construct() = default;
            optional_storage_construct(const optional_storage_construct& rhs) = default;
            optional_storage_construct(optional_storage_construct&&) = default;
            using optional_storage_base<T>::optional_storage_base;
        };

        template <typename T>
        struct optional_storage : optional_storage_construct<T>
        {
            optional_storage() = default;
            optional_storage(const optional_storage&) = default;
            optional_storage(optional_storage&&) = default;
            using optional_storage_construct<T>::optional_storage_construct;
            optional_storage& operator = (const optional_storage& rhs);
            optional_storage& operator = (optional_storage&& rhs) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>);
        };
    }

    template <typename T>
    class optional
        : _private::enable_copy_construct<std::is_copy_constructible_v<T>>
        , _private::enable_move_construct<std::is_move_constructible_v<T>>
        , _private::enable_copy_assign<std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>>
        , _private::enable_move_assign<std::is_move_constructible_v<T> && std::is_move_assignable_v<T>>
    {
        static_assert(!std::is_reference_v<T>, "optional does not support reference types");
        static_assert(!std::is_same_v<std::remove_cv_t<T>, in_place_t>, "optional does not support in_place_t");
        static_assert(!std::is_same_v<std::remove_cv_t<T>, nullopt_t>, "optional does not support nullopt_t");

    public:
        using value_type = T;

        // constructors
        constexpr optional() noexcept = default;
        constexpr optional(nullopt_t) noexcept { };
        constexpr optional(const optional&) = default;
        constexpr optional(optional&&) = default;
        template <typename... Args,
            STDEXT_REQUIRES(std::is_constructible_v<T, Args...>)>
        constexpr explicit optional(in_place_t, Args&&... args) : _storage(in_place, stdext::forward<Args>(args)...) { }
        template <typename U, typename... Args,
            STDEXT_REQUIRES(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>)>
        constexpr explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args) : _storage(in_place, il, stdext::forward<Args>(args)...) { }
        template <typename U = T,
            STDEXT_REQUIRES(std::is_convertible_v<U&&, T>
                            && std::is_constructible_v<T, U&&>
                            && !std::is_same_v<remove_cvref_t<U>, in_place_t>
                            && !std::is_same_v<optional, remove_cvref_t<U>>)>
        constexpr optional(U&& v) : _storage(in_place, stdext::forward<U>(v)) { }
        template <typename U = T,
            STDEXT_REQUIRES(!std::is_convertible_v<U&&, T>
                            && std::is_constructible_v<T, U&&>
                            && !std::is_same_v<remove_cvref_t<U>, in_place_t>
                            && !std::is_same_v<optional, remove_cvref_t<U>>)>
        explicit constexpr optional(U&& v) : _storage(in_place, stdext::forward<U>(v)) { }
        template <typename U,
            STDEXT_REQUIRES(std::is_convertible_v<const U&, T>
                            && std::is_constructible_v<T, const U&>
                            && !_private::is_constructible_from_optional_v<T, U>
                            && !_private::is_convertible_from_optional_v<T, U>)>
        optional(const optional<U>& rhs) { if (rhs.has_value()) _storage.emplace(rhs.value()); }
        template <typename U,
            STDEXT_REQUIRES(!std::is_convertible_v<const U&, T>
                            && std::is_constructible_v<T, const U&>
                            && !_private::is_constructible_from_optional_v<T, U>
                            && !_private::is_convertible_from_optional_v<T, U>)>
        explicit optional(const optional<U>& rhs) { if (rhs.has_value()) _storage.emplace(rhs.value()); }
        template <typename U,
            STDEXT_REQUIRES(std::is_convertible_v<U&&, T>
                            && std::is_constructible_v<T, U&&>
                            && !_private::is_constructible_from_optional_v<T, U>
                            && !_private::is_convertible_from_optional_v<T, U>)>
        optional(optional<U>&& rhs) { if (rhs.has_value()) _storage.emplace(stdext::move(rhs.value())); }
        template <typename U,
            STDEXT_REQUIRES(!std::is_convertible_v<U&&, T>
                            && std::is_constructible_v<T, U&&>
                            && !_private::is_constructible_from_optional_v<T, U>
                            && !_private::is_convertible_from_optional_v<T, U>)>
        explicit optional(optional<U>&& rhs) { if (rhs.has_value()) _storage.emplace(stdext::move(rhs.value())); }

        // destructor
        ~optional() = default;

        // assignment
        optional& operator=(nullopt_t) noexcept { _storage.reset(); return *this; }
        optional& operator=(const optional&) = default;
        optional& operator=(optional&&) = default;
        template <typename U = T,
            STDEXT_REQUIRES(!std::is_same_v<optional, remove_cvref_t<U>>
                            && (!std::is_scalar_v<T> || !std::is_same_v<T, std::decay_t<U>>)
                            && std::is_constructible_v<T, U>
                            && std::is_assignable_v<T&, U>)>
        optional& operator=(U&& v);
        template <typename U,
            STDEXT_REQUIRES(std::is_constructible_v<T, const U&>
                            && std::is_assignable_v<T&, const U&>
                            && !_private::is_constructible_from_optional_v<T, U>
                            && !_private::is_convertible_from_optional_v<T, U>
                            && !_private::is_assignable_from_optional_v<T, U>)>
        optional& operator=(const optional<U>& rhs);
        template <typename U,
            STDEXT_REQUIRES(std::is_constructible_v<T, U>
                            && std::is_assignable_v<T&, U>
                            && !_private::is_constructible_from_optional_v<T, U>
                            && !_private::is_convertible_from_optional_v<T, U>
                            && !_private::is_assignable_from_optional_v<T, U>)>
        optional& operator=(optional<U>&& rhs);
        template <typename... Args> T& emplace(Args&&... args);
        template <typename U, typename... Args,
            STDEXT_REQUIRES(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>)>
        T& emplace(std::initializer_list<U> il, Args&&... args);

        // swap
        void swap(optional&) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>);

        // observers
        constexpr bool has_value() const noexcept { return _storage.has_value; }
        constexpr const T& value() const & { return _storage.has_value ? _storage.value : throw bad_optional_access(); }
        constexpr T& value() & { return _storage.has_value ? _storage.value : throw bad_optional_access(); }
        constexpr T&& value() && { return _storage.has_value ? stdext::move(_storage.value) : throw bad_optional_access(); }
        constexpr const T&& value() const && { return _storage.has_value ? stdext::move(_storage.value) : throw bad_optional_access(); }
        template <typename U> constexpr T value_or(U&& v) const &;
        template <typename U> constexpr T value_or(U&& v) &&;

        // modifiers
        void reset() noexcept { _storage.reset(); }

        // relational operators
        template <typename U>
        friend constexpr bool operator==(const optional& x, const optional<U>& y) { return x.has_value() ? y.has_value() && x.value() == y.value() : !y.has_value(); }
        template <typename U>
        friend constexpr bool operator!=(const optional& x, const optional<U>& y) { return x.has_value() ? !y.has_value() || x.value() != y.value() : y.has_value(); }
        template <typename U>
        friend constexpr bool operator<(const optional& x, const optional<U>& y) { return x.has_value() ? y.has_value() && x.value() < y.value() : y.has_value(); }
        template <typename U>
        friend constexpr bool operator>(const optional& x, const optional<U>& y) { return x.has_value() && (!y.has_value() || x.value() > y.value()); }
        template <typename U>
        friend constexpr bool operator<=(const optional& x, const optional<U>& y) { return !x.has_value() || (y.has_value() && x.value() <= y.value()); }
        template <typename U>
        friend constexpr bool operator>=(const optional& x, const optional<U>& y) { return x.has_value() ? !y.has_value() || x.value() >= y.value() : !y.has_value(); }

        // comparison with nullopt
        friend constexpr bool operator==(const optional& x, nullopt_t) noexcept { return !x.has_value(); }
        friend constexpr bool operator==(nullopt_t, const optional& x) noexcept { return !x.has_value(); }
        friend constexpr bool operator!=(const optional& x, nullopt_t) noexcept { return x.has_value(); }
        friend constexpr bool operator!=(nullopt_t, const optional& x) noexcept { return x.has_value(); }
        friend constexpr bool operator<(const optional& x, nullopt_t) noexcept { return false; }
        friend constexpr bool operator<(nullopt_t, const optional& x) noexcept { return x.has_value(); }
        friend constexpr bool operator<=(const optional& x, nullopt_t) noexcept{ return !x.has_value(); }
        friend constexpr bool operator<=(nullopt_t, const optional& x) noexcept { return true; }
        friend constexpr bool operator>(const optional& x, nullopt_t) noexcept { return x.has_value(); }
        friend constexpr bool operator>(nullopt_t, const optional& x) noexcept { return false; }
        friend constexpr bool operator>=(const optional& x, nullopt_t) noexcept { return true; }
        friend constexpr bool operator>=(nullopt_t, const optional& x) noexcept { return !x.has_value(); }

        // comparison with T
        template <typename U>
        friend constexpr bool operator==(const optional& x, const U& v) { return x.has_value() && x.value() == v; }
        template <typename U, STDEXT_REQUIRES(!_private::is_optional_v<U>)>
        friend constexpr bool operator==(const U& v, const optional& x) { return x.has_value() && v == x.value(); }
        template <typename U>
        friend constexpr bool operator!=(const optional& x, const U& v) { return !x.has_value() || x.value() != v; }
        template <typename U, STDEXT_REQUIRES(!_private::is_optional_v<U>)>
        friend constexpr bool operator!=(const U& v, const optional& x) { return !x.has_value() || v != x.value(); }
        template <typename U>
        friend constexpr bool operator<(const optional& x, const U& v) { return !x.has_value() || x.value() < v; }
        template <typename U, STDEXT_REQUIRES(!_private::is_optional_v<U>)>
        friend constexpr bool operator<(const U& v, const optional& x) { return x.has_value() && v < x.value(); }
        template <typename U>
        friend constexpr bool operator<=(const optional& x, const U& v) { return !x.has_value() || x.value() <= v; }
        template <typename U, STDEXT_REQUIRES(!_private::is_optional_v<U>)>
        friend constexpr bool operator<=(const U& v, const optional& x) { return x.has_value() && v <= x.value(); }
        template <typename U>
        friend constexpr bool operator>(const optional& x, const U& v) { return x.has_value() && x.value() > v; }
        template <typename U, STDEXT_REQUIRES(!_private::is_optional_v<U>)>
        friend constexpr bool operator>(const U& v, const optional& x) { return !x.has_value() || v > x.value(); }
        template <typename U>
        friend constexpr bool operator>=(const optional& x, const U& v) { return x.has_value() && x.value() >= v; }
        template <typename U, STDEXT_REQUIRES(!_private::is_optional_v<U>)>
        friend constexpr bool operator>=(const U& v, const optional& x) { return !x.has_value() || v >= x.value(); }

        // specialized algorithms
        template <typename U = T, STDEXT_REQUIRES(std::is_move_constructible_v<U> && std::is_swappable_v<T>)>
        friend void swap(optional& x, optional& y) noexcept(noexcept(x.swap(y))) { return x.swap(y); }

    private:
        _private::optional_storage<T> _storage;
    };

    template <typename T> optional(T) -> optional<T>;

    // specialized algorithms
    template <typename T>
    constexpr optional<std::decay_t<T>> make_optional(T&& v) { return optional<std::decay_t<T>>(stdext::forward<T>(v)); }
    template <typename T, typename... Args>
    constexpr optional<T> make_optional(Args&&... args) { return optional<T>(in_place, stdext::forward<Args>(args)...); }
    template <typename T, typename U, typename... Args>
    constexpr optional<T> make_optional(std::initializer_list<U> il, Args&&... args) { return optional<T>(in_place, il, stdext::forward<Args>(args)...); }
}

#include <stdext/_impl/optional.inl>

#endif
