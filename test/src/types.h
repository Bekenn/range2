#pragma once

#include <stdext/utility.h>


namespace test
{
    struct explicit_int
    {
        int value;

        explicit_int() = default;
        explicit constexpr explicit_int(int x) : value(x) { }

        constexpr operator int () const noexcept { return value; }

        friend constexpr bool operator == (const explicit_int& a, const explicit_int& b) noexcept { return a.value == b.value; }
        friend constexpr bool operator != (const explicit_int& a, const explicit_int& b) noexcept { return a.value != b.value; }
        friend constexpr bool operator == (const explicit_int& a, int b) noexcept { return a.value == b; }
        friend constexpr bool operator != (const explicit_int& a, int b) noexcept { return a.value != b; }
        friend constexpr bool operator == (int a, const explicit_int& b) noexcept { return a == b.value; }
        friend constexpr bool operator != (int a, const explicit_int& b) noexcept { return a != b.value; }
    };
    static_assert(std::is_trivially_default_constructible_v<explicit_int>);
    static_assert(std::is_trivially_move_constructible_v<explicit_int>);
    static_assert(std::is_trivially_copy_constructible_v<explicit_int>);
    static_assert(std::is_trivially_move_assignable_v<explicit_int>);
    static_assert(std::is_trivially_copy_assignable_v<explicit_int>);
    static_assert(std::is_trivially_destructible_v<explicit_int>);
    static_assert(stdext::is_equality_comparable_v<explicit_int>);
    static_assert(stdext::is_equality_comparable_with_v<explicit_int, int>);

    struct immovable : explicit_int
    {
        using explicit_int::explicit_int;
        immovable() = default;
        immovable(const immovable&) = delete;
        immovable& operator = (const immovable&) = delete;
    };
    static_assert(std::is_trivially_default_constructible_v<immovable>);
    static_assert(!std::is_move_constructible_v<immovable>);
    static_assert(!std::is_copy_constructible_v<immovable>);
    static_assert(!std::is_move_assignable_v<immovable>);
    static_assert(!std::is_copy_assignable_v<immovable>);
    static_assert(std::is_trivially_destructible_v<immovable>);
    static_assert(stdext::is_equality_comparable_v<immovable>);
    static_assert(stdext::is_equality_comparable_with_v<immovable, int>);

    struct not_move_constructible : explicit_int
    {
        using explicit_int::explicit_int;
        not_move_constructible() = default;
        not_move_constructible(const not_move_constructible&) = delete;
        not_move_constructible(not_move_constructible&&) = delete;
        not_move_constructible& operator = (const not_move_constructible&) = default;
        not_move_constructible& operator = (not_move_constructible&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<not_move_constructible>);
    static_assert(!std::is_move_constructible_v<not_move_constructible>);
    static_assert(!std::is_copy_constructible_v<not_move_constructible>);
    static_assert(std::is_trivially_move_assignable_v<not_move_constructible>);
    static_assert(std::is_trivially_copy_assignable_v<not_move_constructible>);
    static_assert(std::is_trivially_destructible_v<not_move_constructible>);
    static_assert(stdext::is_equality_comparable_v<not_move_constructible>);
    static_assert(stdext::is_equality_comparable_with_v<not_move_constructible, int>);

    struct not_move_assignable : explicit_int
    {
        using explicit_int::explicit_int;
        not_move_assignable() = default;
        explicit not_move_assignable(const not_move_assignable&) = default;
        explicit not_move_assignable(not_move_assignable&&) = default;
        not_move_assignable& operator = (const not_move_assignable&) = delete;
        not_move_assignable& operator = (not_move_assignable&&) = delete;
    };
    static_assert(std::is_trivially_default_constructible_v<not_move_assignable>);
    static_assert(std::is_trivially_move_constructible_v<not_move_assignable>);
    static_assert(std::is_trivially_copy_constructible_v<not_move_assignable>);
    static_assert(!std::is_move_assignable_v<not_move_assignable>);
    static_assert(!std::is_copy_assignable_v<not_move_assignable>);
    static_assert(std::is_trivially_destructible_v<not_move_assignable>);
    static_assert(stdext::is_equality_comparable_v<not_move_assignable>);
    static_assert(stdext::is_equality_comparable_with_v<not_move_assignable, int>);

    struct copy_not_movable : explicit_int
    {
        using explicit_int::explicit_int;
        copy_not_movable() = default;
        copy_not_movable(const copy_not_movable&) = default;
        copy_not_movable(copy_not_movable&&) = delete;
        copy_not_movable& operator = (const copy_not_movable&) = default;
        copy_not_movable& operator = (copy_not_movable&&) = delete;
    };
    static_assert(std::is_trivially_default_constructible_v<copy_not_movable>);
    static_assert(!std::is_move_constructible_v<copy_not_movable>);
    static_assert(std::is_trivially_copy_constructible_v<copy_not_movable>);
    static_assert(!std::is_move_assignable_v<copy_not_movable>);
    static_assert(std::is_trivially_copy_assignable_v<copy_not_movable>);
    static_assert(std::is_trivially_destructible_v<copy_not_movable>);
    static_assert(stdext::is_equality_comparable_v<copy_not_movable>);
    static_assert(stdext::is_equality_comparable_with_v<copy_not_movable, int>);

    struct noncopyable : explicit_int
    {
        using explicit_int::explicit_int;
        noncopyable() = default;
        noncopyable(const noncopyable&) = delete;
        noncopyable(noncopyable&&) = default;
        noncopyable& operator = (const noncopyable&) = delete;
        noncopyable& operator = (noncopyable&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<noncopyable>);
    static_assert(std::is_trivially_move_constructible_v<noncopyable>);
    static_assert(!std::is_copy_constructible_v<noncopyable>);
    static_assert(std::is_trivially_move_assignable_v<noncopyable>);
    static_assert(!std::is_copy_assignable_v<noncopyable>);
    static_assert(std::is_trivially_destructible_v<noncopyable>);
    static_assert(stdext::is_equality_comparable_v<noncopyable>);
    static_assert(stdext::is_equality_comparable_with_v<noncopyable, int>);

    struct copy_not_move_constructible : explicit_int
    {
        using explicit_int::explicit_int;
        copy_not_move_constructible() = default;
        copy_not_move_constructible(const copy_not_move_constructible&) = default;
        copy_not_move_constructible(copy_not_move_constructible&&) = delete;
        copy_not_move_constructible& operator = (const copy_not_move_constructible&) = default;
        copy_not_move_constructible& operator = (copy_not_move_constructible&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<copy_not_move_constructible>);
    static_assert(!std::is_move_constructible_v<copy_not_move_constructible>);
    static_assert(std::is_trivially_copy_constructible_v<copy_not_move_constructible>);
    static_assert(std::is_trivially_move_assignable_v<copy_not_move_constructible>);
    static_assert(std::is_trivially_copy_assignable_v<copy_not_move_constructible>);
    static_assert(std::is_trivially_destructible_v<copy_not_move_constructible>);
    static_assert(stdext::is_equality_comparable_v<copy_not_move_constructible>);
    static_assert(stdext::is_equality_comparable_with_v<copy_not_move_constructible, int>);

    struct not_copy_constructible : explicit_int
    {
        using explicit_int::explicit_int;
        not_copy_constructible() = default;
        not_copy_constructible(const not_copy_constructible&) = delete;
        not_copy_constructible(not_copy_constructible&&) = default;
        not_copy_constructible& operator = (const not_copy_constructible&) = default;
        not_copy_constructible& operator = (not_copy_constructible&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<not_copy_constructible>);
    static_assert(std::is_trivially_move_constructible_v<not_copy_constructible>);
    static_assert(!std::is_copy_constructible_v<not_copy_constructible>);
    static_assert(std::is_trivially_move_assignable_v<not_copy_constructible>);
    static_assert(std::is_trivially_copy_assignable_v<not_copy_constructible>);
    static_assert(std::is_trivially_destructible_v<not_copy_constructible>);
    static_assert(stdext::is_equality_comparable_v<not_copy_constructible>);
    static_assert(stdext::is_equality_comparable_with_v<not_copy_constructible, int>);

    struct copy_not_move_assignable : explicit_int
    {
        using explicit_int::explicit_int;
        copy_not_move_assignable() = default;
        copy_not_move_assignable(const copy_not_move_assignable&) = default;
        explicit copy_not_move_assignable(copy_not_move_assignable&&) = default;
        copy_not_move_assignable& operator = (const copy_not_move_assignable&) = default;
        copy_not_move_assignable& operator = (copy_not_move_assignable&&) = delete;
    };
    static_assert(std::is_trivially_default_constructible_v<copy_not_move_assignable>);
    static_assert(std::is_trivially_move_constructible_v<copy_not_move_assignable>);
    static_assert(std::is_trivially_copy_constructible_v<copy_not_move_assignable>);
    static_assert(!std::is_move_assignable_v<copy_not_move_assignable>);
    static_assert(std::is_trivially_copy_assignable_v<copy_not_move_assignable>);
    static_assert(std::is_trivially_destructible_v<copy_not_move_assignable>);
    static_assert(stdext::is_equality_comparable_v<copy_not_move_assignable>);
    static_assert(stdext::is_equality_comparable_with_v<copy_not_move_assignable, int>);

    struct not_copy_assignable : explicit_int
    {
        using explicit_int::explicit_int;
        not_copy_assignable() = default;
        explicit not_copy_assignable(const not_copy_assignable&) = default;
        not_copy_assignable(not_copy_assignable&&) = default;
        not_copy_assignable& operator = (const not_copy_assignable&) = delete;
        not_copy_assignable& operator = (not_copy_assignable&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<not_copy_assignable>);
    static_assert(std::is_trivially_move_constructible_v<not_copy_assignable>);
    static_assert(std::is_trivially_copy_constructible_v<not_copy_assignable>);
    static_assert(std::is_trivially_move_assignable_v<not_copy_assignable>);
    static_assert(!std::is_copy_assignable_v<not_copy_assignable>);
    static_assert(std::is_trivially_destructible_v<not_copy_assignable>);
    static_assert(stdext::is_equality_comparable_v<not_copy_assignable>);
    static_assert(stdext::is_equality_comparable_with_v<not_copy_assignable, int>);

    struct nontrivial_default_construct : explicit_int
    {
        using explicit_int::explicit_int;
        constexpr nontrivial_default_construct() noexcept : explicit_int(0) { }
        nontrivial_default_construct(const nontrivial_default_construct&) = default;
        nontrivial_default_construct(nontrivial_default_construct&&) = default;
        nontrivial_default_construct& operator = (const nontrivial_default_construct&) = default;
        nontrivial_default_construct& operator = (nontrivial_default_construct&&) = default;
    };
    static_assert(!std::is_trivially_default_constructible_v<nontrivial_default_construct>);
    static_assert(std::is_default_constructible_v<nontrivial_default_construct>);
    static_assert(std::is_trivially_move_constructible_v<nontrivial_default_construct>);
    static_assert(std::is_trivially_copy_constructible_v<nontrivial_default_construct>);
    static_assert(std::is_trivially_move_assignable_v<nontrivial_default_construct>);
    static_assert(std::is_trivially_copy_assignable_v<nontrivial_default_construct>);
    static_assert(std::is_trivially_destructible_v<nontrivial_default_construct>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_default_construct>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_default_construct, int>);

    struct nontrivial_move_construct : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial_move_construct() = default;
        nontrivial_move_construct(const nontrivial_move_construct&) = default;
        constexpr nontrivial_move_construct(nontrivial_move_construct&& rhs) noexcept : explicit_int(stdext::move(rhs)) { }
        nontrivial_move_construct& operator = (const nontrivial_move_construct&) = default;
        nontrivial_move_construct& operator = (nontrivial_move_construct&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial_move_construct>);
    static_assert(!std::is_trivially_move_constructible_v<nontrivial_move_construct>);
    static_assert(std::is_move_constructible_v<nontrivial_move_construct>);
    static_assert(std::is_trivially_copy_constructible_v<nontrivial_move_construct>);
    static_assert(std::is_trivially_move_assignable_v<nontrivial_move_construct>);
    static_assert(std::is_trivially_copy_assignable_v<nontrivial_move_construct>);
    static_assert(std::is_trivially_destructible_v<nontrivial_move_construct>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_move_construct>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_move_construct, int>);

    struct nontrivial_copy_construct : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial_copy_construct() = default;
        constexpr nontrivial_copy_construct(const nontrivial_copy_construct& rhs) noexcept : explicit_int(rhs) { }
        nontrivial_copy_construct(nontrivial_copy_construct&&) = default;
        nontrivial_copy_construct& operator = (const nontrivial_copy_construct&) = default;
        nontrivial_copy_construct& operator = (nontrivial_copy_construct&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial_copy_construct>);
    static_assert(std::is_trivially_move_constructible_v<nontrivial_copy_construct>);
    static_assert(!std::is_trivially_copy_constructible_v<nontrivial_copy_construct>);
    static_assert(std::is_copy_constructible_v<nontrivial_copy_construct>);
    static_assert(std::is_trivially_move_assignable_v<nontrivial_copy_construct>);
    static_assert(std::is_trivially_copy_assignable_v<nontrivial_copy_construct>);
    static_assert(std::is_trivially_destructible_v<nontrivial_copy_construct>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_copy_construct>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_copy_construct, int>);

    struct nontrivial_move_assign : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial_move_assign() = default;
        nontrivial_move_assign(const nontrivial_move_assign&) = default;
        nontrivial_move_assign(nontrivial_move_assign&&) = default;
        nontrivial_move_assign& operator = (const nontrivial_move_assign&) = default;
        constexpr nontrivial_move_assign& operator = (nontrivial_move_assign&& rhs) noexcept { explicit_int::operator=(stdext::move(rhs)); return *this; }
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial_move_assign>);
    static_assert(std::is_trivially_move_constructible_v<nontrivial_move_assign>);
    static_assert(std::is_trivially_copy_constructible_v<nontrivial_move_assign>);
    static_assert(!std::is_trivially_move_assignable_v<nontrivial_move_assign>);
    static_assert(std::is_move_assignable_v<nontrivial_move_assign>);
    static_assert(std::is_trivially_copy_assignable_v<nontrivial_move_assign>);
    static_assert(std::is_trivially_destructible_v<nontrivial_move_assign>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_move_assign>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_move_assign, int>);

    struct nontrivial_copy_assign : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial_copy_assign() = default;
        nontrivial_copy_assign(const nontrivial_copy_assign&) = default;
        nontrivial_copy_assign(nontrivial_copy_assign&&) = default;
        constexpr nontrivial_copy_assign& operator = (const nontrivial_copy_assign& rhs) noexcept { explicit_int::operator=(rhs); return *this; }
        nontrivial_copy_assign& operator = (nontrivial_copy_assign&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial_copy_assign>);
    static_assert(std::is_trivially_move_constructible_v<nontrivial_copy_assign>);
    static_assert(std::is_trivially_copy_constructible_v<nontrivial_copy_assign>);
    static_assert(std::is_trivially_move_assignable_v<nontrivial_copy_assign>);
    static_assert(!std::is_trivially_copy_assignable_v<nontrivial_copy_assign>);
    static_assert(std::is_copy_assignable_v<nontrivial_copy_assign>);
    static_assert(std::is_trivially_destructible_v<nontrivial_copy_assign>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_copy_assign>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_copy_assign, int>);

    struct nontrivial_move : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial_move() = default;
        nontrivial_move(const nontrivial_move&) = default;
        constexpr nontrivial_move(nontrivial_move&& rhs) noexcept : explicit_int(stdext::move(rhs)) { }
        nontrivial_move& operator = (const nontrivial_move&) = default;
        constexpr nontrivial_move& operator = (nontrivial_move&& rhs) noexcept { explicit_int::operator=(stdext::move(rhs)); return *this; }
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial_move>);
    static_assert(!std::is_trivially_move_constructible_v<nontrivial_move>);
    static_assert(std::is_move_constructible_v<nontrivial_move>);
    static_assert(std::is_trivially_copy_constructible_v<nontrivial_move>);
    static_assert(!std::is_trivially_move_assignable_v<nontrivial_move>);
    static_assert(std::is_move_assignable_v<nontrivial_move>);
    static_assert(std::is_trivially_copy_assignable_v<nontrivial_move>);
    static_assert(std::is_trivially_destructible_v<nontrivial_move>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_move>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_move, int>);

    struct nontrivial_copy : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial_copy() = default;
        constexpr nontrivial_copy(const nontrivial_copy& rhs) noexcept : explicit_int(rhs) { }
        nontrivial_copy(nontrivial_copy&&) = default;
        constexpr nontrivial_copy& operator = (const nontrivial_copy& rhs) noexcept { explicit_int::operator=(rhs); return *this; }
        nontrivial_copy& operator = (nontrivial_copy&&) = default;
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial_copy>);
    static_assert(std::is_trivially_move_constructible_v<nontrivial_copy>);
    static_assert(!std::is_trivially_copy_constructible_v<nontrivial_copy>);
    static_assert(std::is_copy_constructible_v<nontrivial_copy>);
    static_assert(std::is_trivially_move_assignable_v<nontrivial_copy>);
    static_assert(!std::is_trivially_copy_assignable_v<nontrivial_copy>);
    static_assert(std::is_copy_assignable_v<nontrivial_copy>);
    static_assert(std::is_trivially_destructible_v<nontrivial_copy>);
    static_assert(stdext::is_equality_comparable_v<nontrivial_copy>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial_copy, int>);

    struct nontrivial : explicit_int
    {
        using explicit_int::explicit_int;
        nontrivial() = default;
        constexpr nontrivial(const nontrivial& rhs) noexcept : explicit_int(rhs) { }
        constexpr nontrivial(nontrivial&& rhs) noexcept : explicit_int(stdext::move(rhs)) { }
        constexpr nontrivial& operator = (const nontrivial& rhs) noexcept { explicit_int::operator=(rhs); return *this; }
        constexpr nontrivial& operator = (nontrivial&& rhs) noexcept { explicit_int::operator=(stdext::move(rhs)); return *this; }
    };
    static_assert(std::is_trivially_default_constructible_v<nontrivial>);
    static_assert(!std::is_trivially_move_constructible_v<nontrivial>);
    static_assert(std::is_move_constructible_v<nontrivial>);
    static_assert(!std::is_trivially_copy_constructible_v<nontrivial>);
    static_assert(std::is_copy_constructible_v<nontrivial>);
    static_assert(!std::is_trivially_move_assignable_v<nontrivial>);
    static_assert(std::is_move_assignable_v<nontrivial>);
    static_assert(!std::is_trivially_copy_assignable_v<nontrivial>);
    static_assert(std::is_copy_assignable_v<nontrivial>);
    static_assert(std::is_trivially_destructible_v<nontrivial>);
    static_assert(stdext::is_equality_comparable_v<nontrivial>);
    static_assert(stdext::is_equality_comparable_with_v<nontrivial, int>);

    struct nontrivial_destructor
    {
        static inline unsigned count = 0;
        ~nontrivial_destructor() { ++count; }
    };
    static_assert(std::is_default_constructible_v<nontrivial_destructor>);
    static_assert(std::is_move_constructible_v<nontrivial_destructor>);
    static_assert(std::is_copy_constructible_v<nontrivial_destructor>);
    static_assert(std::is_move_assignable_v<nontrivial_destructor>);
    static_assert(std::is_copy_assignable_v<nontrivial_destructor>);
    static_assert(!std::is_trivially_destructible_v<nontrivial_destructor>);

    struct reset_on_move : explicit_int
    {
        using explicit_int::explicit_int;
        reset_on_move() = default;
        reset_on_move(const reset_on_move&) = default;
        constexpr reset_on_move(reset_on_move&& rhs) noexcept : explicit_int(stdext::exchange(rhs.value, 0)) { }
        reset_on_move& operator = (const reset_on_move&) = default;
        constexpr reset_on_move& operator = (reset_on_move&& rhs) noexcept { value = stdext::exchange(rhs.value, 0); return *this; }
    };
    static_assert(std::is_trivially_default_constructible_v<reset_on_move>);
    static_assert(!std::is_trivially_move_constructible_v<reset_on_move>);
    static_assert(std::is_move_constructible_v<reset_on_move>);
    static_assert(std::is_trivially_copy_constructible_v<reset_on_move>);
    static_assert(!std::is_trivially_move_assignable_v<reset_on_move>);
    static_assert(std::is_move_assignable_v<reset_on_move>);
    static_assert(std::is_trivially_copy_assignable_v<reset_on_move>);
    static_assert(std::is_trivially_destructible_v<reset_on_move>);

    struct throwing_copy : explicit_int
    {
        using explicit_int::explicit_int;
        throwing_copy() = default;
        throwing_copy(const throwing_copy&) { throw 0; }
        throwing_copy& operator = (const throwing_copy&) { throw 0; }
    };
}
