#include <stdext/utility.h>

#include <catch2/catch.hpp>


namespace
{
    enum class value_category
    {
        lvalue,
        rvalue
    };

    value_category test_value_category(int& x);
    value_category test_value_category(int&& x);
}

TEST_CASE("discard", "[utility]")
{
    // not testable
}

TEST_CASE("unreachable", "[utility]")
{
    // not testable
}

TEST_CASE("forward", "[utility]")
{
    int x = 5;
    CHECK(test_value_category(stdext::forward<int&>(x)) == value_category::lvalue);
    CHECK(x == 0);
    CHECK(test_value_category(stdext::forward<int>(x)) == value_category::rvalue);
    CHECK(x == 1);
}

TEST_CASE("copy", "[utility]")
{
    int x = 5;
    CHECK(test_value_category(stdext::copy(x)) == value_category::rvalue);
    CHECK(x == 5);
}

TEST_CASE("move", "[utility]")
{
    int x = 5;
    CHECK(test_value_category(stdext::move(x)) == value_category::rvalue);
    CHECK(x == 1);
}

TEST_CASE("move_if_noexcept", "[utility]")
{
    struct nonthrowing
    {
        nonthrowing() = default;
        nonthrowing(const nonthrowing&) = default;
        nonthrowing(nonthrowing&&) noexcept = default;
    } nt;

    struct throwing
    {
        throwing() = default;
        throwing(const throwing&) = default;
        throwing(throwing&&) noexcept(false) { }
    } t;

    static_assert(std::is_rvalue_reference_v<decltype(stdext::move_if_noexcept(nt))>);
    static_assert(std::is_lvalue_reference_v<decltype(stdext::move_if_noexcept(t))>);
    auto&& nt2 = stdext::move_if_noexcept(nt);
    CHECK(&nt2 == &nt);
    auto&& t2 = stdext::move_if_noexcept(t);
    CHECK(&t2 == &t);
}

TEST_CASE("exchage", "[utility]")
{
    int x = 5;
    int y = stdext::exchange(x, 7);
    CHECK(x == 7);
    CHECK(y == 5);
}

TEST_CASE("swap", "[utility]")
{
    int x = 5;
    int y = 7;
    stdext::swap(x, y);
    CHECK(x == 7);
    CHECK(y == 5);
}

TEST_CASE("array swap", "[utility]")
{
    int x[] = { 0, 1, 2 };
    int y[] = { 3, 4, 5 };
    stdext::swap(x, y);
    CHECK(x[0] == 3);
    CHECK(x[1] == 4);
    CHECK(x[2] == 5);
    CHECK(y[0] == 0);
    CHECK(y[1] == 1);
    CHECK(y[2] == 2);
}

TEST_CASE("addressof", "[utility]")
{
    int x;
    CHECK(stdext::addressof(x) == &x);

    struct B { };
    struct D : B { int operator & () const noexcept { return 5; } } d;
    CHECK(&d == 5);
    CHECK(stdext::addressof(d) == static_cast<D*>(&static_cast<B&>(d)));
}

TEST_CASE("min", "[utility]")
{
    CHECK(stdext::min(5) == 5);
    CHECK(stdext::min(5, 7) == 5);
    CHECK(stdext::min(7, 5) == 5);
    CHECK(stdext::min(3, 4, 5) == 3);
    CHECK(stdext::min(4, 3, 5) == 3);
    CHECK(stdext::min(4, 5, 3) == 3);
}

TEST_CASE("max", "[utility]")
{
    CHECK(stdext::max(5) == 5);
    CHECK(stdext::max(5, 7) == 7);
    CHECK(stdext::max(7, 5) == 7);
    CHECK(stdext::max(3, 4, 5) == 5);
    CHECK(stdext::max(4, 3, 5) == 5);
    CHECK(stdext::max(4, 5, 3) == 5);
}

namespace
{
    value_category test_value_category(int& x)
    {
        x = 0;
        return value_category::lvalue;
    }

    value_category test_value_category(int&& x)
    {
        x = 1;
        return value_category::rvalue;
    }
}
