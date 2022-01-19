#include <stdext/optional.h>

#include "types.h"

#include <catch2/catch.hpp>

#include <limits>
#include <tuple>
#include <vector>
#include <string>


namespace test
{
    // constexpr optional() noexcept
    static_assert(std::is_nothrow_default_constructible_v<stdext::optional<int>>);
    static_assert(!stdext::optional<int>().has_value());

    // constexpr optional(nullopt_t) noexcept
    static_assert(std::is_nothrow_constructible_v<stdext::optional<int>, stdext::nullopt_t>);
    static_assert(std::is_nothrow_constructible_v<stdext::optional<int>, const stdext::nullopt_t&>);
    static_assert(std::is_convertible_v<stdext::nullopt_t, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const stdext::nullopt_t&, stdext::optional<int>>);
    static_assert(!stdext::optional<int>(stdext::nullopt).has_value());

    // constexpr optional(const optional& rhs)
    namespace
    {
        template <typename T>
        constexpr bool test_copy_constructible(const stdext::optional<T>& x)
        {
            stdext::optional<T> y(x);
            return x.has_value() ? y.has_value() && y.value() == x.value() : !y.has_value();
        }
    }
    static_assert(std::is_copy_constructible_v<stdext::optional<int>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial_move_construct>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial_copy_construct>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial_move_assign>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial_copy_assign>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial_move>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial_copy>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<nontrivial>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<throwing_copy>>);
    static_assert(!std::is_copy_constructible_v<stdext::optional<not_copy_constructible>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<copy_not_move_constructible>>);
    static_assert(!std::is_copy_constructible_v<stdext::optional<noncopyable>>);
    static_assert(std::is_copy_constructible_v<stdext::optional<copy_not_movable>>);
    static_assert(std::is_convertible_v<const stdext::optional<int>&, stdext::optional<int>>);
    static_assert(test_copy_constructible(stdext::optional<int>()));
    static_assert(test_copy_constructible(stdext::optional<int>(5)));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_move_construct>()));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_move_construct>(5)));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_move_assign>()));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_move_assign>(5)));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_copy_assign>()));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_copy_assign>(5)));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_move>()));
    static_assert(test_copy_constructible(stdext::optional<nontrivial_move>(5)));

    TEMPLATE_TEST_CASE("nontrivial copy constructor", "[optional]", nontrivial_copy_construct, nontrivial_copy, nontrivial)
    {
        SECTION("disengaged")
        {
            stdext::optional<TestType> x;
            REQUIRE_FALSE(x.has_value());
            stdext::optional<TestType> y(x);
            CHECK_FALSE(y.has_value());
        }

        SECTION("engaged")
        {
            stdext::optional<TestType> x(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 5);

            stdext::optional<TestType> y(x);
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }
    }

    TEST_CASE("throwing copy constructor", "[optional]")
    {
        stdext::optional<throwing_copy> x(stdext::in_place);
        REQUIRE(x.has_value());
        CHECK_THROWS(stdext::optional<throwing_copy>(x));
    }

    // constexpr optional(optional&& rhs) noexcept(...)
    namespace
    {
        template <typename T>
        constexpr bool test_move_constructible(stdext::optional<T>&& x)
        {
            if (x.has_value())
            {
                T value = x.value();
                stdext::optional<T> y(stdext::move(x));
                return y.has_value() && y.value() == value;
            }

            stdext::optional<T> y(stdext::move(x));
            return !y.has_value();
        }
    }
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<int>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial_move_construct>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial_copy_construct>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial_move_assign>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial_copy_assign>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial_move>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial_copy>>);
    static_assert(std::is_nothrow_move_constructible_v<stdext::optional<nontrivial>>);
    static_assert(!std::is_nothrow_move_constructible_v<stdext::optional<throwing_copy>>);
    static_assert(std::is_move_constructible_v<stdext::optional<throwing_copy>>);
    static_assert(!std::is_move_constructible_v<stdext::optional<not_move_constructible>>);
    static_assert(!std::is_move_constructible_v<stdext::optional<immovable>>);
    static_assert(std::is_convertible_v<stdext::optional<int>, stdext::optional<int>>);
    static_assert(test_move_constructible(stdext::optional<int>()));
    static_assert(test_move_constructible(stdext::optional<int>(5)));
    static_assert(test_move_constructible(stdext::optional<nontrivial_copy_construct>()));
    static_assert(test_move_constructible(stdext::optional<nontrivial_copy_construct>(5)));
    static_assert(test_move_constructible(stdext::optional<nontrivial_move_assign>()));
    static_assert(test_move_constructible(stdext::optional<nontrivial_move_assign>(5)));
    static_assert(test_move_constructible(stdext::optional<nontrivial_copy_assign>()));
    static_assert(test_move_constructible(stdext::optional<nontrivial_copy_assign>(5)));
    static_assert(test_move_constructible(stdext::optional<nontrivial_copy>()));
    static_assert(test_move_constructible(stdext::optional<nontrivial_copy>(5)));

    TEMPLATE_TEST_CASE("nontrivial move constructor", "[optional]", nontrivial_move_construct, nontrivial_move, nontrivial)
    {
        SECTION("disengaged")
        {
            stdext::optional<TestType> x;
            REQUIRE_FALSE(x.has_value());

            stdext::optional<TestType> y(stdext::move(x));
            CHECK_FALSE(y.has_value());
        }

        SECTION("engaged")
        {
            stdext::optional<TestType> x(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 5);

            stdext::optional<TestType> y(stdext::move(x));
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }
    }

    TEST_CASE("throwing move constructor", "[optional]")
    {
        stdext::optional<throwing_copy> x(stdext::in_place);
        REQUIRE(x.has_value());

        CHECK_THROWS(stdext::optional<throwing_copy>(stdext::move(x)));
    }

    // constexpr explicit optional(in_place_t, Args&&... args)
    static_assert(std::is_constructible_v<stdext::optional<int>, stdext::in_place_t>);
    static_assert(!std::is_convertible_v<stdext::in_place_t, stdext::optional<int>>);
    static_assert(stdext::optional<int>(stdext::in_place).has_value());
    static_assert(stdext::optional<int>(stdext::in_place).value() == 0);
    static_assert(stdext::optional<int>(stdext::in_place, 5).has_value());
    static_assert(stdext::optional<int>(stdext::in_place, 5).value() == 5);
    static_assert(stdext::optional<std::tuple<int, long>>(stdext::in_place).has_value());
    static_assert(stdext::optional<std::tuple<int, long>>(stdext::in_place).value() == std::tuple<int, long>());
    static_assert(stdext::optional<std::tuple<int, long>>(stdext::in_place, 5, 10).has_value());
    static_assert(stdext::optional<std::tuple<int, long>>(stdext::in_place, 5, 10).value() == std::tuple<int, long>(5, 10));

    // constexpr explicit optional(in_place_t, initializer_list<U> il, Args&&... args)
    namespace
    {
        struct init_list_adder
        {
            int value;

            constexpr init_list_adder(std::initializer_list<int> values, int more = 0) noexcept : value(more)
            {
                for (auto x : values)
                    value += x;
            }

            constexpr operator int () const noexcept { return value; }
        };
    }

    static_assert(std::is_constructible_v<stdext::optional<init_list_adder>, stdext::in_place_t, std::initializer_list<int>>);
    static_assert(std::is_constructible_v<stdext::optional<init_list_adder>, stdext::in_place_t, std::initializer_list<int>, int>);
    static_assert(stdext::optional<init_list_adder>(stdext::in_place, { 1, 1, 2, 3, 5 }).has_value());
    static_assert(stdext::optional<init_list_adder>(stdext::in_place, { 1, 1, 2, 3, 5 }).value() == 12);
    static_assert(stdext::optional<init_list_adder>(stdext::in_place, { 1, 1, 2, 3, 5 }, 8).has_value());
    static_assert(stdext::optional<init_list_adder>(stdext::in_place, { 1, 1, 2, 3, 5 }, 8).value() == 20);

    TEST_CASE("in_place constructor with initializer_list", "[optional]")
    {
        stdext::optional<std::vector<int>> x(stdext::in_place, { 1, 1, 2, 3, 5 });
        CHECK(x == std::vector<int>{ 1, 1, 2, 3, 5 });
    }

    // EXPLICIT constexpr optional(U&& v)
    static_assert(std::is_constructible_v<stdext::optional<int>, int>);
    static_assert(std::is_constructible_v<stdext::optional<int>, const int>);
    static_assert(std::is_constructible_v<stdext::optional<int>, int&>);
    static_assert(std::is_constructible_v<stdext::optional<int>, const int&>);
    static_assert(std::is_convertible_v<int, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const int, stdext::optional<int>>);
    static_assert(std::is_convertible_v<int&, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const int&, stdext::optional<int>>);
    static_assert(std::is_constructible_v<stdext::optional<int>, short>);
    static_assert(std::is_constructible_v<stdext::optional<int>, const short>);
    static_assert(std::is_constructible_v<stdext::optional<int>, short&>);
    static_assert(std::is_constructible_v<stdext::optional<int>, const short&>);
    static_assert(std::is_convertible_v<short, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const short, stdext::optional<int>>);
    static_assert(std::is_convertible_v<short&, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const short&, stdext::optional<int>>);
    static_assert(std::is_constructible_v<stdext::optional<explicit_int>, int>);
    static_assert(!std::is_convertible_v<int, stdext::optional<explicit_int>>);
    static_assert(std::is_constructible_v<stdext::optional<init_list_adder>, std::initializer_list<int>>);
    static_assert(std::is_convertible_v<std::initializer_list<int>, stdext::optional<init_list_adder>>);
    static_assert(std::is_constructible_v<stdext::optional<stdext::optional<int>>, int>);
    static_assert(std::is_constructible_v<stdext::optional<stdext::optional<int>>, long>);
    static_assert(std::is_constructible_v<stdext::optional<stdext::optional<int>>, stdext::optional<int>>);
    static_assert(std::is_constructible_v<stdext::optional<stdext::optional<int>>, stdext::optional<long>>);
    static_assert(stdext::optional<int>(5).has_value());
    static_assert(stdext::optional<int>(5).value() == 5);
    static_assert(stdext::optional<explicit_int>(5).has_value());
    static_assert(stdext::optional<explicit_int>(5).value() == 5);
    static_assert(stdext::optional<init_list_adder>({ 1, 1, 2, 3, 5 }).has_value());
    static_assert(stdext::optional<init_list_adder>({ 1, 1, 2, 3, 5 }).value() == 12);
    static_assert(stdext::optional<stdext::optional<int>>(5).has_value());
    static_assert(stdext::optional<stdext::optional<int>>(5).value().has_value());
    static_assert(stdext::optional<stdext::optional<int>>(5).value().value() == 5);
    static_assert(stdext::optional<stdext::optional<int>>(5L).has_value());
    static_assert(stdext::optional<stdext::optional<int>>(5L).value().has_value());
    static_assert(stdext::optional<stdext::optional<int>>(5L).value().value() == 5);
    static_assert(stdext::optional<stdext::optional<int>>(stdext::optional<int>()).has_value());
    static_assert(!stdext::optional<stdext::optional<int>>(stdext::optional<int>()).value().has_value());
    static_assert(stdext::optional<stdext::optional<int>>(stdext::optional<int>(5)).has_value());
    static_assert(stdext::optional<stdext::optional<int>>(stdext::optional<int>(5)).value().has_value());
    static_assert(stdext::optional<stdext::optional<int>>(stdext::optional<int>(5)).value().value() == 5);

    TEST_CASE("nested optional converting constructor", "[optional]")
    {
        SECTION("disengaged")
        {
            stdext::optional<stdext::optional<int>> x = stdext::optional<long>();
            REQUIRE(x.has_value());
            REQUIRE_FALSE(x.value().has_value());
        }

        SECTION("engaged")
        {
            stdext::optional<stdext::optional<int>> x = stdext::optional<long>(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value().has_value());
            REQUIRE(x.value().value() == 5);
        }
    }

    // EXPLICIT optional(const optional<U>& rhs)
    static_assert(std::is_constructible_v<stdext::optional<int>, stdext::optional<short>&>);
    static_assert(std::is_constructible_v<stdext::optional<int>, const stdext::optional<short>&>);
    static_assert(std::is_convertible_v<stdext::optional<short>&, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const stdext::optional<short>&, stdext::optional<int>>);
    static_assert(std::is_constructible_v<stdext::optional<explicit_int>, stdext::optional<int>&>);
    static_assert(!std::is_convertible_v<stdext::optional<int>&, stdext::optional<explicit_int>>);
    TEST_CASE("constructor from optional lvalue", "[optional]")
    {
        SECTION("implicit")
        {
            stdext::optional<long> ol(5);
            REQUIRE(ol.has_value());
            REQUIRE(ol.value() == 5);

            stdext::optional<int> oi = ol;
            REQUIRE(oi.has_value());
            CHECK(oi.value() == 5);
        }

        SECTION("explicit")
        {
            stdext::optional<int> oi(5);
            REQUIRE(oi.has_value());
            REQUIRE(oi.value() == 5);

            stdext::optional<explicit_int> oei(oi);
            REQUIRE(oei.has_value());
            CHECK(oei.value() == 5);
        }
    }

    // EXPLICIT optional(optional<U>&& rhs)
    static_assert(std::is_constructible_v<stdext::optional<int>, stdext::optional<short>>);
    static_assert(std::is_constructible_v<stdext::optional<int>, const stdext::optional<short>>);
    static_assert(std::is_convertible_v<stdext::optional<short>, stdext::optional<int>>);
    static_assert(std::is_convertible_v<const stdext::optional<short>, stdext::optional<int>>);
    static_assert(std::is_constructible_v<stdext::optional<explicit_int>, stdext::optional<int>>);
    static_assert(!std::is_convertible_v<stdext::optional<int>, stdext::optional<explicit_int>>);
    TEST_CASE("constructor from optional rvalue", "[optional]")
    {
        SECTION("implicit")
        {
            stdext::optional<long> ol(5);
            REQUIRE(ol.has_value());
            REQUIRE(ol.value() == 5);

            stdext::optional<int> oi = stdext::move(ol);
            REQUIRE(oi.has_value());
            CHECK(oi.value() == 5);
        }

        SECTION("explicit")
        {
            stdext::optional<int> oi(5);
            REQUIRE(oi.has_value());
            REQUIRE(oi.value() == 5);

            stdext::optional<explicit_int> oei(stdext::move(oi));
            REQUIRE(oei.has_value());
            CHECK(oei.value() == 5);
        }
    }

    // ~optional()
    static_assert(std::is_trivially_destructible_v<stdext::optional<int>>);
    static_assert(!std::is_trivially_destructible_v<stdext::optional<nontrivial_destructor>>);
    TEST_CASE("destructor", "[optional]")
    {
        unsigned prev_count = nontrivial_destructor::count;
        {
            stdext::optional<nontrivial_destructor> x(stdext::in_place);
            REQUIRE(x.has_value());
        }

        CHECK(nontrivial_destructor::count == prev_count + 1);
    };

    // optional<T>& operator=(nullopt_t) noexcept
    static_assert(std::is_nothrow_assignable_v<stdext::optional<int>, stdext::nullopt_t>);
    static_assert(std::is_nothrow_assignable_v<stdext::optional<int>, const stdext::nullopt_t&>);
    TEST_CASE("assignment from nullopt", "[optional]")
    {
        SECTION("disengaged")
        {
            unsigned prev_count = nontrivial_destructor::count;
            stdext::optional<nontrivial_destructor> x;
            REQUIRE_FALSE(x.has_value());

            x = stdext::nullopt;
            CHECK_FALSE(x.has_value());
            CHECK(nontrivial_destructor::count == prev_count);
        }

        SECTION("engaged")
        {
            unsigned prev_count = nontrivial_destructor::count;
            stdext::optional<nontrivial_destructor> x(stdext::in_place);
            REQUIRE(x.has_value());

            x = stdext::nullopt;
            CHECK_FALSE(x.has_value());
            CHECK(nontrivial_destructor::count == prev_count + 1);
        }
    }

    // optional<T>& operator=(const optional& rhs)
    static_assert(std::is_copy_assignable_v<stdext::optional<int>>);
    static_assert(std::is_copy_assignable_v<stdext::optional<throwing_copy>>);
    static_assert(!std::is_copy_assignable_v<stdext::optional<not_copy_constructible>>);
    static_assert(!std::is_copy_assignable_v<stdext::optional<not_copy_assignable>>);
    static_assert(!std::is_copy_assignable_v<stdext::optional<noncopyable>>);
    static_assert(std::is_copy_assignable_v<stdext::optional<copy_not_move_constructible>>);
    static_assert(std::is_copy_assignable_v<stdext::optional<copy_not_move_assignable>>);
    static_assert(std::is_copy_assignable_v<stdext::optional<copy_not_movable>>);

    TEST_CASE("copy assignment", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            stdext::optional<int> x;
            stdext::optional<int> y;
            REQUIRE_FALSE(x.has_value());
            REQUIRE_FALSE(y.has_value());

            x = y;
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("disengaged from engaged")
        {
            stdext::optional<int> x;
            stdext::optional<int> y(5);
            REQUIRE_FALSE(x.has_value());
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            x = y;
            REQUIRE(x.has_value());
            CHECK(x.value() == 5);
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }

        SECTION("engaged from disengaged")
        {
            stdext::optional<int> x(3);
            stdext::optional<int> y;
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE_FALSE(y.has_value());

            x = y;
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("engaged from engaged")
        {
            stdext::optional<int> x(3);
            stdext::optional<int> y(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            x = y;
            REQUIRE(x.has_value());
            CHECK(x.value() == 5);
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }
    }

    TEST_CASE("throwing copy assignment", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            stdext::optional<throwing_copy> x;
            stdext::optional<throwing_copy> y;
            REQUIRE_FALSE(x.has_value());
            REQUIRE_FALSE(y.has_value());

            CHECK_NOTHROW(x = y);
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("disengaged from engaged")
        {
            stdext::optional<throwing_copy> x;
            stdext::optional<throwing_copy> y(5);
            REQUIRE_FALSE(x.has_value());
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            CHECK_THROWS(x = y);
            CHECK_FALSE(x.has_value());
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }

        SECTION("engaged from disengaged")
        {
            stdext::optional<throwing_copy> x(3);
            stdext::optional<throwing_copy> y;
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE_FALSE(y.has_value());

            CHECK_NOTHROW(x = y);
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("engaged from engaged")
        {
            stdext::optional<throwing_copy> x(3);
            stdext::optional<throwing_copy> y(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            CHECK_THROWS(x = y);
            REQUIRE(x.has_value());
            CHECK(x.value() == 3);
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }
    }

    // optional<T>& operator=(optional&& rhs) noexcept(...)
    static_assert(std::is_nothrow_move_assignable_v<stdext::optional<int>>);
    static_assert(std::is_move_assignable_v<stdext::optional<throwing_copy>>);
    static_assert(!std::is_move_assignable_v<stdext::optional<not_move_constructible>>);
    static_assert(!std::is_move_assignable_v<stdext::optional<not_move_assignable>>);

    TEST_CASE("move assignment", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            stdext::optional<reset_on_move> x;
            stdext::optional<reset_on_move> y;
            REQUIRE_FALSE(x.has_value());
            REQUIRE_FALSE(y.has_value());

            x = stdext::move(y);
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("disengaged from engaged")
        {
            stdext::optional<reset_on_move> x;
            stdext::optional<reset_on_move> y(5);
            REQUIRE_FALSE(x.has_value());
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            x = stdext::move(y);
            REQUIRE(x.has_value());
            CHECK(x.value() == 5);
            REQUIRE(y.has_value());
            CHECK(y.value() == 0);
        }

        SECTION("engaged from disengaged")
        {
            stdext::optional<reset_on_move> x(3);
            stdext::optional<reset_on_move> y;
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE_FALSE(y.has_value());

            x = stdext::move(y);
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("engaged from engaged")
        {
            stdext::optional<reset_on_move> x(3);
            stdext::optional<reset_on_move> y(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            x = stdext::move(y);
            REQUIRE(x.has_value());
            CHECK(x.value() == 5);
            REQUIRE(y.has_value());
            CHECK(y.value() == 0);
        }
    }

    TEST_CASE("throwing move assignment", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            stdext::optional<throwing_copy> x;
            stdext::optional<throwing_copy> y;
            REQUIRE_FALSE(x.has_value());
            REQUIRE_FALSE(y.has_value());

            CHECK_NOTHROW(x = stdext::move(y));
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("disengaged from engaged")
        {
            stdext::optional<throwing_copy> x;
            stdext::optional<throwing_copy> y(5);
            REQUIRE_FALSE(x.has_value());
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            CHECK_THROWS(x = stdext::move(y));
            CHECK_FALSE(x.has_value());
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }

        SECTION("engaged from disengaged")
        {
            stdext::optional<throwing_copy> x(3);
            stdext::optional<throwing_copy> y;
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE_FALSE(y.has_value());

            CHECK_NOTHROW(x = stdext::move(y));
            CHECK_FALSE(x.has_value());
            CHECK_FALSE(y.has_value());
        }

        SECTION("engaged from engaged")
        {
            stdext::optional<throwing_copy> x(3);
            stdext::optional<throwing_copy> y(5);
            REQUIRE(x.has_value());
            REQUIRE(x.value() == 3);
            REQUIRE(y.has_value());
            REQUIRE(y.value() == 5);

            CHECK_THROWS(x = stdext::move(y));
            REQUIRE(x.has_value());
            CHECK(x.value() == 3);
            REQUIRE(y.has_value());
            CHECK(y.value() == 5);
        }
    }

    // optional<T>& operator=(U&& v)
    static_assert(std::is_assignable_v<stdext::optional<int>&, int>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const int>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, int&>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const int&>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, short>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const short>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, short&>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const short&>);
    static_assert(std::is_assignable_v<stdext::optional<std::vector<int>>&, std::initializer_list<int>>);
    static_assert(std::is_assignable_v<stdext::optional<std::string>&, const char*>);
    static_assert(std::is_assignable_v<stdext::optional<stdext::optional<int>>, int>);
    static_assert(std::is_assignable_v<stdext::optional<stdext::optional<int>>, long>);
    static_assert(std::is_assignable_v<stdext::optional<stdext::optional<int>>, stdext::optional<int>>);
    static_assert(std::is_assignable_v<stdext::optional<stdext::optional<int>>, stdext::optional<long>>);
    TEST_CASE("assignment from assignable-to-T", "[optional]")
    {
        SECTION("disengaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x;
                REQUIRE_FALSE(x.has_value());

                x = 5;
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("string")
            {
                stdext::optional<std::string> x;
                REQUIRE_FALSE(x.has_value());

                x = "Hello, world!";
                REQUIRE(x.has_value());
                CHECK(x.value().compare("Hello, world!") == 0);
            }

            SECTION("vector")
            {
                stdext::optional<std::vector<int>> x;
                REQUIRE_FALSE(x.has_value());

                x = { 1, 1, 2, 3, 5 };
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x;
                REQUIRE_FALSE(x.has_value());

                CHECK_THROWS(x = throwing_copy());
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("engaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x(0);
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 0);

                x = 5;
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("string")
            {
                stdext::optional<std::string> x(stdext::in_place);
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());

                x = "Hello, world!";
                REQUIRE(x.has_value());
                CHECK(x.value().compare("Hello, world!") == 0);
            }

            SECTION("vector")
            {
                stdext::optional<std::vector<int>> x(stdext::in_place);
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());

                x = { 1, 1, 2, 3, 5 };
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x(stdext::in_place);
                REQUIRE(x.has_value());

                CHECK_THROWS(x = throwing_copy());
                CHECK(x.has_value());
            }
        }
    }

    TEST_CASE("nested optional converting assignment", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            SECTION("int")
            {
                stdext::optional<stdext::optional<int>> x;
                REQUIRE_FALSE(x.has_value());

                x = stdext::optional<int>();
                REQUIRE(x.has_value());
                CHECK_FALSE(x.value().has_value());
            }

            SECTION("long")
            {
                stdext::optional<stdext::optional<int>> x;
                REQUIRE_FALSE(x.has_value());

                x = stdext::optional<long>();
                REQUIRE(x.has_value());
                CHECK_FALSE(x.value().has_value());
            }
        }

        SECTION("disengaged from engaged")
        {
            SECTION("int")
            {
                stdext::optional<stdext::optional<int>> x;
                REQUIRE_FALSE(x.has_value());

                x = stdext::optional<int>(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                CHECK(x.value().value() == 5);
            }

            SECTION("long")
            {
                stdext::optional<stdext::optional<int>> x;
                REQUIRE_FALSE(x.has_value());

                x = stdext::optional<long>(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                CHECK(x.value().value() == 5);
            }
        }

        SECTION("engaged from disengaged")
        {
            SECTION("int")
            {
                stdext::optional<stdext::optional<int>> x(3);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                REQUIRE(x.value().value() == 3);

                x = stdext::optional<int>();
                REQUIRE(x.has_value());
                CHECK_FALSE(x.value().has_value());
            }

            SECTION("long")
            {
                stdext::optional<stdext::optional<int>> x(3);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                REQUIRE(x.value().value() == 3);

                x = stdext::optional<long>();
                REQUIRE(x.has_value());
                CHECK_FALSE(x.value().has_value());
            }
        }

        SECTION("engaged from engaged")
        {
            SECTION("int")
            {
                stdext::optional<stdext::optional<int>> x(3);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                REQUIRE(x.value().value() == 3);

                x = stdext::optional<int>(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                CHECK(x.value().value() == 5);
            }

            SECTION("long")
            {
                stdext::optional<stdext::optional<int>> x(3);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                REQUIRE(x.value().value() == 3);

                x = stdext::optional<long>(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value().has_value());
                CHECK(x.value().value() == 5);
            }
        }
    }

    // optional<T>& operator=(const optional<U>& rhs)
    static_assert(std::is_assignable_v<stdext::optional<int>&, stdext::optional<const int>&>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const stdext::optional<const int>&>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, stdext::optional<short>&>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const stdext::optional<short>&>);
    static_assert(std::is_assignable_v<stdext::optional<std::string>&, stdext::optional<const char*>&>);
    static_assert(std::is_assignable_v<stdext::optional<std::string>&, const stdext::optional<const char*>&>);
    TEST_CASE("assignment from optional<assignable-to-T> lvalue", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x;
                stdext::optional<const int> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                x = y;
                CHECK_FALSE(x.has_value());
            }

            SECTION("string")
            {
                stdext::optional<std::string> x;
                stdext::optional<const char*> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                x = y;
                CHECK_FALSE(x.has_value());
            }

            SECTION("vector")
            {
                stdext::optional<std::vector<int>> x;
                stdext::optional<std::initializer_list<int>> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                x = y;
                CHECK_FALSE(x.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x;
                stdext::optional<const throwing_copy> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                CHECK_NOTHROW(x = y);
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("disengaged from engaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x;
                stdext::optional<const int> y(5);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());
                REQUIRE(y.value() == 5);

                x = y;
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("string")
            {
                static constexpr char message[] = "Hello, world!";
                stdext::optional<std::string> x;
                stdext::optional<const char*> y(message);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());
                REQUIRE(y.value() == message);

                x = y;
                REQUIRE(x.has_value());
                CHECK(x.value().compare(message) == 0);
            }

            SECTION("vector")
            {
                std::initializer_list<int> inits = { 1, 1, 2, 3, 5 };
                stdext::optional<std::vector<int>> x;
                stdext::optional<std::initializer_list<int>> y(inits);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());
                REQUIRE(y.value().begin() == inits.begin());
                REQUIRE(y.value().end() == inits.end());

                x = y;
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x;
                stdext::optional<const throwing_copy> y(stdext::in_place);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());

                CHECK_THROWS(x = y);
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("engaged from disengaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x(0);
                stdext::optional<const int> y;
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 0);
                REQUIRE_FALSE(y.has_value());

                x = y;
                CHECK_FALSE(x.has_value());
            }

            SECTION("string")
            {
                stdext::optional<std::string> x(stdext::in_place);
                stdext::optional<const char*> y;
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE_FALSE(y.has_value());

                x = y;
                CHECK_FALSE(x.has_value());
            }

            SECTION("vector")
            {
                stdext::optional<std::vector<int>> x(stdext::in_place);
                stdext::optional<std::initializer_list<int>> y;
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE_FALSE(y.has_value());

                x = y;
                CHECK_FALSE(x.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x(stdext::in_place);
                stdext::optional<const throwing_copy> y;
                REQUIRE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                CHECK_NOTHROW(x = y);
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("engaged from engaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x(0);
                stdext::optional<const int> y(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 0);
                REQUIRE(y.has_value());
                REQUIRE(y.value() == 5);

                x = y;
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("string")
            {
                static constexpr char message[] = "Hello, world!";
                stdext::optional<std::string> x(stdext::in_place);
                stdext::optional<const char*> y(message);
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE(y.has_value());
                REQUIRE(y.value() == message);

                x = y;
                REQUIRE(x.has_value());
                CHECK(x.value().compare(message) == 0);
            }

            SECTION("vector")
            {
                std::initializer_list<int> inits = { 1, 1, 2, 3, 5 };
                stdext::optional<std::vector<int>> x(stdext::in_place);
                stdext::optional<std::initializer_list<int>> y(inits);
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE(y.has_value());
                REQUIRE(y.value().begin() == inits.begin());
                REQUIRE(y.value().end() == inits.end());

                x = y;
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x(stdext::in_place);
                stdext::optional<const throwing_copy> y(stdext::in_place);
                REQUIRE(x.has_value());
                REQUIRE(y.has_value());

                CHECK_THROWS(x = y);
                CHECK(x.has_value());
            }
        }
    }

    // optional<T>& operator=(optional<U>&& rhs)
    static_assert(std::is_assignable_v<stdext::optional<int>&, stdext::optional<const int>>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const stdext::optional<const int>>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, stdext::optional<short>>);
    static_assert(std::is_assignable_v<stdext::optional<int>&, const stdext::optional<short>>);
    static_assert(std::is_assignable_v<stdext::optional<std::string>&, stdext::optional<const char*>>);
    static_assert(std::is_assignable_v<stdext::optional<std::string>&, const stdext::optional<const char*>>);
    TEST_CASE("assignment from optional<assignable-to-T> rvalue", "[optional]")
    {
        SECTION("disengaged from disengaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x;
                stdext::optional<const int> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                x = stdext::move(y);
                CHECK_FALSE(x.has_value());
            }

            SECTION("string")
            {
                stdext::optional<std::string> x;
                stdext::optional<const char*> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                x = stdext::move(y);
                CHECK_FALSE(x.has_value());
            }

            SECTION("vector")
            {
                stdext::optional<std::vector<int>> x;
                stdext::optional<std::initializer_list<int>> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                x = stdext::move(y);
                CHECK_FALSE(x.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x;
                stdext::optional<const throwing_copy> y;
                REQUIRE_FALSE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                CHECK_NOTHROW(x = stdext::move(y));
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("disengaged from engaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x;
                stdext::optional<const int> y(5);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());
                REQUIRE(y.value() == 5);

                x = stdext::move(y);
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("string")
            {
                static constexpr char message[] = "Hello, world!";
                stdext::optional<std::string> x;
                stdext::optional<const char*> y(message);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());
                REQUIRE(y.value() == message);

                x = stdext::move(y);
                REQUIRE(x.has_value());
                CHECK(x.value().compare(message) == 0);
            }

            SECTION("vector")
            {
                std::initializer_list<int> inits = { 1, 1, 2, 3, 5 };
                stdext::optional<std::vector<int>> x;
                stdext::optional<std::initializer_list<int>> y(inits);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());
                REQUIRE(y.value().begin() == inits.begin());
                REQUIRE(y.value().end() == inits.end());

                x = stdext::move(y);
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x;
                stdext::optional<const throwing_copy> y(stdext::in_place);
                REQUIRE_FALSE(x.has_value());
                REQUIRE(y.has_value());

                CHECK_THROWS(x = stdext::move(y));
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("engaged from disengaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x(0);
                stdext::optional<const int> y;
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 0);
                REQUIRE_FALSE(y.has_value());

                x = stdext::move(y);
                CHECK_FALSE(x.has_value());
            }

            SECTION("string")
            {
                stdext::optional<std::string> x(stdext::in_place);
                stdext::optional<const char*> y;
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE_FALSE(y.has_value());

                x = stdext::move(y);
                CHECK_FALSE(x.has_value());
            }

            SECTION("vector")
            {
                stdext::optional<std::vector<int>> x(stdext::in_place);
                stdext::optional<std::initializer_list<int>> y;
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE_FALSE(y.has_value());

                x = stdext::move(y);
                CHECK_FALSE(x.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x(stdext::in_place);
                stdext::optional<const throwing_copy> y;
                REQUIRE(x.has_value());
                REQUIRE_FALSE(y.has_value());

                CHECK_NOTHROW(x = stdext::move(y));
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("engaged from engaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x(0);
                stdext::optional<const int> y(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 0);
                REQUIRE(y.has_value());
                REQUIRE(y.value() == 5);

                x = stdext::move(y);
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("string")
            {
                static constexpr char message[] = "Hello, world!";
                stdext::optional<std::string> x(stdext::in_place);
                stdext::optional<const char*> y(message);
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE(y.has_value());
                REQUIRE(y.value() == message);

                x = stdext::move(y);
                REQUIRE(x.has_value());
                CHECK(x.value().compare(message) == 0);
            }

            SECTION("vector")
            {
                std::initializer_list<int> inits = { 1, 1, 2, 3, 5 };
                stdext::optional<std::vector<int>> x(stdext::in_place);
                stdext::optional<std::initializer_list<int>> y(inits);
                REQUIRE(x.has_value());
                REQUIRE(x.value().empty());
                REQUIRE(y.has_value());
                REQUIRE(y.value().begin() == inits.begin());
                REQUIRE(y.value().end() == inits.end());

                x = stdext::move(y);
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x(stdext::in_place);
                stdext::optional<const throwing_copy> y(stdext::in_place);
                REQUIRE(x.has_value());
                REQUIRE(y.has_value());

                CHECK_THROWS(x = stdext::move(y));
                CHECK(x.has_value());
            }
        }
    }

    TEST_CASE("emplace", "[optional]")
    {
        SECTION("disengaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x;
                REQUIRE_FALSE(x.has_value());

                x.emplace(5);
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("const int")
            {
                stdext::optional<const int> x;
                REQUIRE_FALSE(x.has_value());

                x.emplace(5);
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("tuple")
            {
                static constexpr char message[] = "Hello, world!";

                stdext::optional<std::tuple<int, const char*>> x;
                REQUIRE_FALSE(x.has_value());

                x.emplace(5, message);
                REQUIRE(x.has_value());
                CHECK(std::get<0>(x.value()) == 5);
                CHECK(std::get<1>(x.value()) == message);
            }

            SECTION("default")
            {
                stdext::optional<throwing_copy> x;
                REQUIRE_FALSE(x.has_value());

                CHECK_NOTHROW(x.emplace());
                CHECK(x.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x;
                REQUIRE_FALSE(x.has_value());

                CHECK_THROWS(x.emplace(throwing_copy()));
                CHECK_FALSE(x.has_value());
            }
        }

        SECTION("engaged")
        {
            SECTION("int")
            {
                stdext::optional<int> x(3);
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 3);

                x.emplace(5);
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("const int")
            {
                stdext::optional<const int> x(3);
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 3);

                x.emplace(5);
                REQUIRE(x.has_value());
                CHECK(x.value() == 5);
            }

            SECTION("tuple")
            {
                static constexpr char message[] = "Hello, world!";

                stdext::optional<std::tuple<int, const char*>> x(stdext::in_place, 3, nullptr);
                REQUIRE(x.has_value());
                REQUIRE(std::get<0>(x.value()) == 3);
                REQUIRE(std::get<1>(x.value()) == nullptr);

                x.emplace(5, message);
                REQUIRE(x.has_value());
                CHECK(std::get<0>(x.value()) == 5);
                CHECK(std::get<1>(x.value()) == message);
            }

            SECTION("default")
            {
                stdext::optional<int> x(5);
                REQUIRE(x.has_value());
                REQUIRE(x.value() == 5);

                x.emplace();
                CHECK(x.has_value());
                CHECK(x.value() == 0);
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> x(stdext::in_place);
                REQUIRE(x.has_value());

                CHECK_THROWS(x.emplace(throwing_copy()));
                CHECK_FALSE(x.has_value());
            }
        }
    }

    TEST_CASE("emplace with initializer_list", "[optional]")
    {
        SECTION("disengaged")
        {
            SECTION("vector<int>")
            {
                stdext::optional<std::vector<int>> x;
                REQUIRE_FALSE(x.has_value());

                x.emplace({ 1, 1, 2, 3, 5 });
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("const vector<int>")
            {
                stdext::optional<const std::vector<int>> x;
                REQUIRE_FALSE(x.has_value());

                x.emplace({ 1, 1, 2, 3, 5 });
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("init_list_adder")
            {
                stdext::optional<init_list_adder> x;
                REQUIRE_FALSE(x.has_value());

                x.emplace({ 1, 1, 2, 3, 5 }, 8);
                REQUIRE(x.has_value());
                CHECK(x.value() == 20);
            }
        }

        SECTION("engaged")
        {
            SECTION("vector<int>")
            {
                stdext::optional<std::vector<int>> x({ 3, 2, 1 });
                REQUIRE(x.has_value());
                REQUIRE(x.value() == std::vector<int>{ 3, 2, 1 });

                x.emplace({ 1, 1, 2, 3, 5 });
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("const vector<int>")
            {
                stdext::optional<const std::vector<int>> x({ 3, 2, 1 });
                REQUIRE(x.has_value());
                REQUIRE(x.value() == std::vector<int>{ 3, 2, 1 });

                x.emplace({ 1, 1, 2, 3, 5 });
                REQUIRE(x.has_value());
                CHECK(x.value() == std::vector<int>{ 1, 1, 2, 3, 5 });
            }

            SECTION("init_list_adder")
            {
                stdext::optional<init_list_adder> x({ 3, 2, 1 });
                REQUIRE(x.has_value());
                CHECK(x.value() == 6);

                x.emplace({ 1, 1, 2, 3, 5 }, 8);
                REQUIRE(x.has_value());
                CHECK(x.value() == 20);
            }
        }
    }

    // void swap(optional& rhs) noexcept(...)
    TEST_CASE("member swap", "[optional]")
    {
        SECTION("disengaged with disengaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a;
                stdext::optional<int> b;

                REQUIRE_FALSE(a.has_value());
                REQUIRE_FALSE(b.has_value());

                a.swap(b);
                CHECK_FALSE(a.has_value());
                CHECK_FALSE(b.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a;
                stdext::optional<throwing_copy> b;

                REQUIRE_FALSE(a.has_value());
                REQUIRE_FALSE(b.has_value());

                CHECK_NOTHROW(a.swap(b));
                CHECK_FALSE(a.has_value());
                CHECK_FALSE(b.has_value());
            }
        }

        SECTION("disengaged with engaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a;
                stdext::optional<int> b(3);

                REQUIRE_FALSE(a.has_value());
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 3);

                a.swap(b);
                REQUIRE(a.has_value());
                CHECK(a.value() == 3);
                CHECK_FALSE(b.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a;
                stdext::optional<throwing_copy> b(3);

                REQUIRE_FALSE(a.has_value());
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 3);

                REQUIRE_THROWS(a.swap(b));
                CHECK_FALSE(a.has_value());
                REQUIRE(b.has_value());
                CHECK(b.value() == 3);
            }
        }

        SECTION("engaged with disengaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a(3);
                stdext::optional<int> b;

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE_FALSE(b.has_value());

                CHECK_NOTHROW(a.swap(b));
                CHECK_FALSE(a.has_value());
                REQUIRE(b.has_value());
                CHECK(b.value() == 3);
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a(3);
                stdext::optional<throwing_copy> b;

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE_FALSE(b.has_value());

                REQUIRE_THROWS(a.swap(b));
                REQUIRE(a.has_value());
                CHECK(a.value() == 3);
                CHECK_FALSE(b.has_value());
            }
        }

        SECTION("engaged with engaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a(3);
                stdext::optional<int> b(5);

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 5);

                a.swap(b);
                REQUIRE(a.has_value());
                CHECK(a.value() == 5);
                REQUIRE(b.has_value());
                CHECK(b.value() == 3);
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a(3);
                stdext::optional<throwing_copy> b(5);

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 5);

                REQUIRE_THROWS(a.swap(b));
                REQUIRE(a.has_value());
                CHECK(a.value() == 3);
                REQUIRE(b.has_value());
                CHECK(b.value() == 5);
            }
        }
    }

    // constexpr explicit operator const T& () const &
    // constexpr explicit operator T& () &
    // constexpr explicit operator T&& () &&
    // constexpr explicit operator const T&& () const &&
    namespace
    {
        constexpr bool test_explicit_conversion() noexcept
        {
            stdext::optional<int> x(5);
            return int(x) == 5 && int(stdext::as_const(x)) == 5 && int(stdext::move(x)) == 5 && int(stdext::move(stdext::as_const(x))) == 5;
        }
    }
    static_assert(std::is_constructible_v<int, stdext::optional<int>>);
    static_assert(std::is_constructible_v<int, const stdext::optional<int>>);
    static_assert(std::is_constructible_v<int, stdext::optional<int>&>);
    static_assert(std::is_constructible_v<int, const stdext::optional<int>&>);
    static_assert(!std::is_convertible_v<stdext::optional<int>, int>);
    static_assert(!std::is_convertible_v<const stdext::optional<int>, int>);
    static_assert(!std::is_convertible_v<stdext::optional<int>&, int>);
    static_assert(!std::is_convertible_v<const stdext::optional<int>&, int>);
    static_assert(!std::is_assignable_v<int, stdext::optional<int>>);
    static_assert(!std::is_assignable_v<int, const stdext::optional<int>>);
    static_assert(!std::is_assignable_v<int, stdext::optional<int>&>);
    static_assert(!std::is_assignable_v<int, const stdext::optional<int>&>);
    static_assert(test_explicit_conversion());

    // constexpr bool has_value() const noexcept
    static_assert(!stdext::optional<int>().has_value());
    static_assert(stdext::optional<int>(0).has_value());

    // constexpr const T& value() const &
    // constexpr T& value() &
    // constexpr T&& value() &&
    // constexpr const T&& value() const &&
    namespace
    {
        constexpr bool test_value() noexcept
        {
            stdext::optional<int> x(5);
            return x.value() == 5 && stdext::as_const(x).value() == 5 && stdext::move(x).value() == 5 && stdext::move(stdext::as_const(x)).value() == 5;
        }
    }
    static_assert(std::is_same_v<decltype(std::declval<stdext::optional<int>>().value()), int&&>);
    static_assert(std::is_same_v<decltype(std::declval<const stdext::optional<int>>().value()), const int&&>);
    static_assert(std::is_same_v<decltype(std::declval<stdext::optional<int>&>().value()), int&>);
    static_assert(std::is_same_v<decltype(std::declval<const stdext::optional<int>&>().value()), const int&>);
    static_assert(test_value());

    // constexpr T value_or(U&& v) const &
    // constexpr T value_or(U&& v) &&
    namespace
    {
        constexpr bool test_value_or() noexcept
        {
            struct implicit_int_reset_on_move : reset_on_move
            {
                constexpr implicit_int_reset_on_move(int x) noexcept : reset_on_move(x) { }
            };

            stdext::optional<implicit_int_reset_on_move> x(5);
            stdext::optional<implicit_int_reset_on_move> y;

            if (x.value_or(10) != 5 || !x.has_value() || x.value() != 5)
                return false;
            if (stdext::move(x).value_or(10) != 5 || !x.has_value() || x.value() != 0)
                return false;
            return y.value_or(10) == 10 && stdext::move(y).value_or(10) == 10;
        }
    }
    static_assert(test_value_or());

    //void reset() noexcept
    TEST_CASE("reset", "[optional]")
    {
        SECTION("disengaged")
        {
            stdext::optional<nontrivial_destructor> x;
            REQUIRE_FALSE(x.has_value());

            unsigned prev_count = nontrivial_destructor::count;
            x.reset();
            CHECK_FALSE(x.has_value());
            CHECK(nontrivial_destructor::count == prev_count);
        }

        SECTION("engaged")
        {
            stdext::optional<nontrivial_destructor> x(stdext::in_place);
            REQUIRE(x.has_value());

            unsigned prev_count = nontrivial_destructor::count;
            x.reset();
            CHECK_FALSE(x.has_value());
            CHECK(nontrivial_destructor::count == prev_count + 1);
        }
    }

    // comparisons
    namespace
    {
        template <typename T>
        constexpr T min = std::numeric_limits<T>::min();
    }

    // ==
    static_assert(stdext::optional<int>() == stdext::optional<long>());
    static_assert(!(stdext::optional<int>() == stdext::optional<long>(0)));
    static_assert(!(stdext::optional<int>(0) == stdext::optional<long>()));
    static_assert(!(stdext::optional<int>(0) == stdext::optional<long>(1)));
    static_assert(stdext::optional<int>(0) == stdext::optional<long>(0));
    static_assert(!(stdext::optional<int>(0) == stdext::optional<long>(-1)));
    static_assert(stdext::optional<int>() == stdext::nullopt);
    static_assert(!(stdext::optional<int>(0) == stdext::nullopt));
    static_assert(stdext::nullopt == stdext::optional<int>());
    static_assert(!(stdext::nullopt == stdext::optional<int>(0)));
    static_assert(!(stdext::optional<int>() == 0L));
    static_assert(!(0 == stdext::optional<long>()));
    static_assert(!(stdext::optional<int>(0) == 1L));
    static_assert(!(0 == stdext::optional<long>(1)));
    static_assert(stdext::optional<int>(0) == 0L);
    static_assert(0 == stdext::optional<long>(0));
    static_assert(!(stdext::optional<int>(0) == -1L));
    static_assert(!(0 == stdext::optional<long>(-1)));

    // !=
    static_assert(!(stdext::optional<int>() != stdext::optional<long>()));
    static_assert(stdext::optional<int>() != stdext::optional<long>(0));
    static_assert(stdext::optional<int>(0) != stdext::optional<long>());
    static_assert(stdext::optional<int>(0) != stdext::optional<long>(1));
    static_assert(!(stdext::optional<int>(0) != stdext::optional<long>(0)));
    static_assert(stdext::optional<int>(0) != stdext::optional<long>(-1));
    static_assert(!(stdext::optional<int>() != stdext::nullopt));
    static_assert(stdext::optional<int>(0) != stdext::nullopt);
    static_assert(!(stdext::nullopt != stdext::optional<int>()));
    static_assert(stdext::nullopt != stdext::optional<int>(0));
    static_assert(stdext::optional<int>() != 0L);
    static_assert(0 != stdext::optional<long>());
    static_assert(stdext::optional<int>(0) != 1L);
    static_assert(0 != stdext::optional<long>(1));
    static_assert(!(stdext::optional<int>(0) != 0L));
    static_assert(!(0 != stdext::optional<long>(0)));
    static_assert(stdext::optional<int>(0) != -1L);
    static_assert(0 != stdext::optional<long>(-1));

    // <
    static_assert(!(stdext::optional<int>() < stdext::optional<long>()));
    static_assert(stdext::optional<int>() < stdext::optional<long>(min<long>));
    static_assert(!(stdext::optional<int>(min<int>) < stdext::optional<long>()));
    static_assert(stdext::optional<int>(0) < stdext::optional<long>(1));
    static_assert(!(stdext::optional<int>(0) < stdext::optional<long>(0)));
    static_assert(!(stdext::optional<int>(0) < stdext::optional<long>(-1)));
    static_assert(!(stdext::optional<int>() < stdext::nullopt));
    static_assert(!(stdext::optional<int>(min<int>) < stdext::nullopt));
    static_assert(!(stdext::nullopt < stdext::optional<int>()));
    static_assert(stdext::nullopt < stdext::optional<int>(min<int>));
    static_assert(stdext::optional<int>() < min<long>);
    static_assert(!(min<int> < stdext::optional<long>()));
    static_assert(stdext::optional<int>(0) < 1L);
    static_assert(0 < stdext::optional<long>(1));
    static_assert(!(stdext::optional<int>(0) < 0L));
    static_assert(!(0 < stdext::optional<long>(0)));
    static_assert(!(stdext::optional<int>(0) < -1L));
    static_assert(!(0 < stdext::optional<long>(-1)));

    // >
    static_assert(!(stdext::optional<int>() > stdext::optional<long>()));
    static_assert(!(stdext::optional<int>() > stdext::optional<long>(min<long>)));
    static_assert(stdext::optional<int>(min<int>) > stdext::optional<long>());
    static_assert(!(stdext::optional<int>(0) > stdext::optional<long>(1)));
    static_assert(!(stdext::optional<int>(0) > stdext::optional<long>(0)));
    static_assert(stdext::optional<int>(0) > stdext::optional<long>(-1));
    static_assert(!(stdext::optional<int>() > stdext::nullopt));
    static_assert(stdext::optional<int>(min<int>) > stdext::nullopt);
    static_assert(!(stdext::nullopt > stdext::optional<int>()));
    static_assert(!(stdext::nullopt > stdext::optional<int>(min<int>)));
    static_assert(!(stdext::optional<int>() > min<long>));
    static_assert(min<int> > stdext::optional<long>());
    static_assert(!(stdext::optional<int>(0) > 1L));
    static_assert(!(0 > stdext::optional<long>(1)));
    static_assert(!(stdext::optional<int>(0) > 0L));
    static_assert(!(0 > stdext::optional<long>(0)));
    static_assert(stdext::optional<int>(0) > -1L);
    static_assert(0 > stdext::optional<long>(-1));

    // <=
    static_assert(stdext::optional<int>() <= stdext::optional<long>());
    static_assert(stdext::optional<int>() <= stdext::optional<long>(min<long>));
    static_assert(!(stdext::optional<int>(min<int>) <= stdext::optional<long>()));
    static_assert(stdext::optional<int>(0) <= stdext::optional<long>(1));
    static_assert(stdext::optional<int>(0) <= stdext::optional<long>(0));
    static_assert(!(stdext::optional<int>(0) <= stdext::optional<long>(-1)));
    static_assert(stdext::optional<int>() <= stdext::nullopt);
    static_assert(!(stdext::optional<int>(min<int>) <= stdext::nullopt));
    static_assert(stdext::nullopt <= stdext::optional<int>());
    static_assert(stdext::nullopt <= stdext::optional<int>(min<int>));
    static_assert(stdext::optional<int>() <= min<long>);
    static_assert(!(min<int> <= stdext::optional<long>()));
    static_assert(stdext::optional<int>(0) <= 1L);
    static_assert(0 <= stdext::optional<long>(1));
    static_assert(stdext::optional<int>(0) <= 0L);
    static_assert(0 <= stdext::optional<long>(0));
    static_assert(!(stdext::optional<int>(0) <= -1L));
    static_assert(!(0 <= stdext::optional<long>(-1)));

    // >=
    static_assert(stdext::optional<int>() >= stdext::optional<long>());
    static_assert(!(stdext::optional<int>() >= stdext::optional<long>(min<long>)));
    static_assert(stdext::optional<int>(min<int>) >= stdext::optional<long>());
    static_assert(!(stdext::optional<int>(0) >= stdext::optional<long>(1)));
    static_assert(stdext::optional<int>(0) >= stdext::optional<long>(0));
    static_assert(stdext::optional<int>(0) >= stdext::optional<long>(-1));
    static_assert(stdext::optional<int>() >= stdext::nullopt);
    static_assert(stdext::optional<int>(min<int>) >= stdext::nullopt);
    static_assert(stdext::nullopt >= stdext::optional<int>());
    static_assert(!(stdext::nullopt >= stdext::optional<int>(min<int>)));
    static_assert(!(stdext::optional<int>() >= min<long>));
    static_assert(min<int> >= stdext::optional<long>());
    static_assert(!(stdext::optional<int>(0) >= 1L));
    static_assert(!(0 >= stdext::optional<long>(1)));
    static_assert(stdext::optional<int>(0) >= 0L);
    static_assert(0 >= stdext::optional<long>(0));
    static_assert(stdext::optional<int>(0) >= -1L);
    static_assert(0 >= stdext::optional<long>(-1));

    // swap
    TEST_CASE("adl swap", "[optional]")
    {
        STATIC_REQUIRE(std::is_nothrow_swappable_v<stdext::optional<int>>);

        SECTION("disengaged with disengaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a;
                stdext::optional<int> b;

                REQUIRE_FALSE(a.has_value());
                REQUIRE_FALSE(b.has_value());

                swap(a, b);
                CHECK_FALSE(a.has_value());
                CHECK_FALSE(b.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a;
                stdext::optional<throwing_copy> b;

                REQUIRE_FALSE(a.has_value());
                REQUIRE_FALSE(b.has_value());

                CHECK_NOTHROW(swap(a, b));
                CHECK_FALSE(a.has_value());
                CHECK_FALSE(b.has_value());
            }
        }

        SECTION("disengaged with engaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a;
                stdext::optional<int> b(3);

                REQUIRE_FALSE(a.has_value());
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 3);

                swap(a, b);
                REQUIRE(a.has_value());
                CHECK(a.value() == 3);
                CHECK_FALSE(b.has_value());
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a;
                stdext::optional<throwing_copy> b(3);

                REQUIRE_FALSE(a.has_value());
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 3);

                REQUIRE_THROWS(swap(a, b));
                CHECK_FALSE(a.has_value());
                REQUIRE(b.has_value());
                CHECK(b.value() == 3);
            }
        }

        SECTION("engaged with disengaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a(3);
                stdext::optional<int> b;

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE_FALSE(b.has_value());

                CHECK_NOTHROW(swap(a, b));
                CHECK_FALSE(a.has_value());
                REQUIRE(b.has_value());
                CHECK(b.value() == 3);
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a(3);
                stdext::optional<throwing_copy> b;

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE_FALSE(b.has_value());

                REQUIRE_THROWS(swap(a, b));
                REQUIRE(a.has_value());
                CHECK(a.value() == 3);
                CHECK_FALSE(b.has_value());
            }
        }

        SECTION("engaged with engaged")
        {
            SECTION("nonthrowing")
            {
                stdext::optional<int> a(3);
                stdext::optional<int> b(5);

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 5);

                swap(a, b);
                REQUIRE(a.has_value());
                CHECK(a.value() == 5);
                REQUIRE(b.has_value());
                CHECK(b.value() == 3);
            }

            SECTION("throwing")
            {
                stdext::optional<throwing_copy> a(3);
                stdext::optional<throwing_copy> b(5);

                REQUIRE(a.has_value());
                REQUIRE(a.value() == 3);
                REQUIRE(b.has_value());
                REQUIRE(b.value() == 5);

                REQUIRE_THROWS(swap(a, b));
                REQUIRE(a.has_value());
                CHECK(a.value() == 3);
                REQUIRE(b.has_value());
                CHECK(b.value() == 5);
            }
        }
    }

    // optional(T) -> optional<T>
    static_assert(std::is_same_v<decltype(stdext::optional(std::declval<int>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::optional(std::declval<const int>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::optional(std::declval<int&>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::optional(std::declval<const int&>())), stdext::optional<int>>);

    // in-place construction
    static_assert(!std::is_default_constructible_v<stdext::in_place_t>);
    static_assert(std::is_same_v<decltype(stdext::in_place), const stdext::in_place_t>);

    // no-value state indicator
    static_assert(!std::is_default_constructible_v<stdext::nullopt_t>);
    static_assert(std::is_same_v<decltype(stdext::nullopt), const stdext::nullopt_t>);

    // class bad_optional_access
    static_assert(std::is_base_of_v<std::exception, stdext::bad_optional_access>);
    static_assert(!std::is_abstract_v<stdext::bad_optional_access>);
    static_assert(std::is_default_constructible_v<stdext::bad_optional_access>);
    static_assert(std::is_nothrow_copy_constructible_v<stdext::bad_optional_access>);
    static_assert(std::is_nothrow_copy_assignable_v<stdext::bad_optional_access>);

    // make_optional
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<int>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<int&>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<const int>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<const int&>())), stdext::optional<int>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<int[]>())), stdext::optional<int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<int (&)[]>())), stdext::optional<int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<const int[]>())), stdext::optional<const int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<const int (&)[]>())), stdext::optional<const int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<int[5]>())), stdext::optional<int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<int (&)[5]>())), stdext::optional<int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<const int[5]>())), stdext::optional<const int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<const int (&)[5]>())), stdext::optional<const int*>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<void ()>())), stdext::optional<void (*)()>>);
    static_assert(std::is_same_v<decltype(stdext::make_optional(std::declval<void (&)()>())), stdext::optional<void (*)()>>);
    static_assert(stdext::make_optional(5).has_value());
    static_assert(stdext::make_optional(5).value() == 5);

    static_assert(std::is_same_v<decltype(stdext::make_optional<std::tuple<int, long>>(5, 10)), stdext::optional<std::tuple<int, long>>>);
    static_assert(stdext::make_optional<std::tuple<int, long>>(5, 10).has_value());
    static_assert(stdext::make_optional<std::tuple<int, long>>(5, 10).value() == std::make_tuple(5, 10));

    TEST_CASE("make_optional with initializer_list", "[optional]")
    {
        STATIC_REQUIRE(std::is_same_v<decltype(stdext::make_optional<std::vector<int>>(std::declval<std::initializer_list<int>>())), stdext::optional<std::vector<int>>>);
        auto x = stdext::make_optional<std::vector<int>>({ 1, 1, 2, 3, 5 });
        CHECK(x == std::vector<int>{ 1, 1, 2, 3, 5 });
    }
}
