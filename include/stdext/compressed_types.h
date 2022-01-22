//
//  compressed_types.h
//  stdext
//
//  Created by James Touton on 1/28/21.
//  Copyright © 2021 James Touton. All rights reserved.
//

#ifndef STDEXT_COMPRESSED_PAIR_INCLUDED
#define STDEXT_COMPRESSED_PAIR_INCLUDED
#pragma once

#include <stdext/utility.h>

#include <type_traits>


namespace stdext
{
    template <typename T, bool Inheritable = std::is_class_v<T> && !std::is_final_v<T>>
    class compressed_base;

    template <typename T1, typename T2>
    class compressed_pair;

    template <typename T>
    class compressed_base<T, false>
    {
    public:
        using type = T;

    public:
        constexpr compressed_base() = default;
        explicit constexpr compressed_base(const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>) : value(v) { }
        explicit constexpr compressed_base(T&& v) noexcept(std::is_nothrow_move_constructible_v<T>) : value(stdext::move(v)) { }

        friend constexpr bool operator == (const compressed_base& a, const compressed_base& b) noexcept
        {
            return a.value == b.value;
        }
        friend constexpr bool operator != (const compressed_base& a, const compressed_base& b) noexcept
        {
            return !(a == b);
        }

    public:
        constexpr T& get() & noexcept { return value; }
        constexpr const T& get() const & noexcept { return value; }
        constexpr T&& get() && noexcept { return stdext::move(value); }
        constexpr const T&& get() const && noexcept { return stdext::move(value); }

    private:
        T value;
    };

    template <typename T>
    class compressed_base<T, true> : private T
    {
    public:
        using type = T;

    public:
        constexpr compressed_base() = default;
        explicit constexpr compressed_base(const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>) : T(v) { }
        explicit constexpr compressed_base(T&& v) noexcept(std::is_nothrow_move_constructible_v<T>) : T(v) { }

        constexpr friend bool operator == (const compressed_base& a, const compressed_base& b) noexcept
        {
            return static_cast<const T&>(a) == static_cast<const T&>(b);
        }
        constexpr friend bool operator != (const compressed_base& a, const compressed_base& b) noexcept
        {
            return !(a == b);
        }

    public:
        constexpr T& get() & noexcept { return static_cast<T&>(*this); }
        constexpr const T& get() const & noexcept { return static_cast<const T&>(*this); }
        constexpr T&& get() && noexcept { return static_cast<T&&>(*this); }
        constexpr const T&& get() const && noexcept { return static_cast<const T&&>(*this); }
    };

    template <typename T1, typename T2>
    class compressed_pair : compressed_base<T1>, compressed_base<T2>
    {
    public:
        using first_type = T1;
        using second_type = T2;
    private:
        using first_base = compressed_base<T1>;
        using second_base = compressed_base<T2>;

    public:
        constexpr compressed_pair() = default;
        constexpr compressed_pair(const T1& v1, const T2& v2) noexcept(std::is_nothrow_copy_constructible_v<T1> && std::is_nothrow_copy_constructible_v<T2>)
            : first_base(v1), second_base(v2) { }
        constexpr compressed_pair(const T1& v1, T2&& v2) noexcept(std::is_nothrow_copy_constructible_v<T1> && std::is_nothrow_move_constructible_v<T2>)
            : first_base(v1), second_base(stdext::move(v2)) { }
        constexpr compressed_pair(T1&& v1, const T2& v2) noexcept(std::is_nothrow_move_constructible_v<T1> && std::is_nothrow_copy_constructible_v<T2>)
            : first_base(stdext::move(v1)), second_base(v2) { }
        constexpr compressed_pair(T1&& v1, T2&& v2) noexcept(std::is_nothrow_move_constructible_v<T1> && std::is_nothrow_move_constructible_v<T2>)
            : first_base(stdext::move(v1)), second_base(stdext::move(v2)) { }

        constexpr T1& first() & noexcept { return static_cast<first_base&>(*this).get(); }
        constexpr const T1& first() const & noexcept { return static_cast<const first_base&>(*this).get(); }
        constexpr T1&& first() && noexcept { return static_cast<first_base&&>(*this).get(); }
        constexpr const T1& first() const && noexcept { return static_cast<const first_base&&>(*this).get(); }

        constexpr T2& second() & noexcept { return static_cast<second_base&>(*this).get(); }
        constexpr const T2& second() const & noexcept { return static_cast<const second_base&>(*this).get(); }
        constexpr T2& second() && noexcept { return static_cast<second_base&&>(*this).get(); }
        constexpr const T2& second() const && noexcept { return static_cast<const second_base&&>(*this).get(); }

        constexpr void swap(compressed_pair& p) noexcept
        {
            swap(first(), p.first());
            swap(second(), p.second());
        }
    };

    namespace _private
    {
        template <typename T, bool Empty = std::is_empty_v<T>>
        class compressed_pair_same_type;

        template <typename T>
        class compressed_pair_same_type<T, false>
        {
        public:
            using first_type = T;
            using second_type = T;

        public:
            constexpr compressed_pair_same_type() noexcept(std::is_nothrow_default_constructible_v<T> && std::is_nothrow_default_constructible_v<T>)
                : v1_(), v2_() { }
            constexpr compressed_pair_same_type(const T& v1, const T& v2) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>)
                : v1_(v1), v2_(v2) { }
            constexpr compressed_pair_same_type(const T& v1, T&& v2) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_move_constructible_v<T>)
                : v1_(v1), v2_(stdext::move(v2)) { }
            constexpr compressed_pair_same_type(T&& v1, const T& v2) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
                : v1_(stdext::move(v1)), v2_(v2) { }
            constexpr compressed_pair_same_type(T&& v1, T&& v2) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<T>)
                : v1_(stdext::move(v1)), v2_(stdext::move(v2)) { }

            constexpr T& first() & noexcept { return v1_; }
            constexpr const T& first() const & noexcept { return v1_; }
            constexpr T&& first() && noexcept { return stdext::move(v1_); }
            constexpr const T& first() const && noexcept { return stdext::move(v1_); }

            constexpr T& second() & noexcept { return v2_; }
            constexpr const T& second() const & noexcept { return v2_; }
            constexpr T& second() && noexcept { return stdext::move(v2_); }
            constexpr const T& second() const && noexcept { return stdext::move(v2_); }

            constexpr void swap(compressed_pair_same_type& p) noexcept
            {
                swap(v1_, p.v1_);
                swap(v2_, p.v2_);
            }

        private:
            T v1_;
            T v2_;
        };

        template <typename T>
        class compressed_pair_same_type<T, true> : compressed_base<T>
        {
        public:
            using first_type = T;
            using second_type = T;
        private:
            using base = compressed_base<T>;

        public:
            constexpr compressed_pair_same_type() = default;
            constexpr compressed_pair_same_type(const T&, const T&) noexcept(std::is_nothrow_default_constructible_v<T>) { }
            constexpr compressed_pair_same_type(const T&, T&&) noexcept(std::is_nothrow_default_constructible_v<T>) { }
            constexpr compressed_pair_same_type(T&&, const T&) noexcept(std::is_nothrow_default_constructible_v<T>) { }
            constexpr compressed_pair_same_type(T&&, T&&) noexcept(std::is_nothrow_default_constructible_v<T>) { }

            constexpr T& first() & noexcept { return static_cast<base&>(*this).get(); }
            constexpr const T& first() const & noexcept { return static_cast<const base&>(*this).get(); }
            constexpr T&& first() && noexcept { return static_cast<base&&>(*this).get(); }
            constexpr const T& first() const && noexcept { return static_cast<const base&&>(*this).get(); }

            constexpr T& second() & noexcept { return static_cast<base&>(*this).get(); }
            constexpr const T& second() const & noexcept { return static_cast<const base&>(*this).get(); }
            constexpr T& second() && noexcept { return static_cast<base&&>(*this).get(); }
            constexpr const T& second() const && noexcept { return static_cast<const base&&>(*this).get(); }

            constexpr void swap(compressed_pair_same_type&) noexcept { }
        };
    }

    template <typename T>
    class compressed_pair<T, T> : public _private::compressed_pair_same_type<T>
    {
        using _private::compressed_pair_same_type<T>::compressed_pair_same_type;
    };
}

#endif
