#include <stdext/flags.h>

#include <catch2/catch.hpp>


namespace test
{
    namespace
    {
        enum class test_flags : std::uint32_t
        {
            none = 0,
            flag0 = 1,
            flag1 = 2,
            flag2 = 4,
            flag3 = 8
        };
    }

    static_assert(std::is_same_v<stdext::flags<test_flags>::enum_type, test_flags>);
    static_assert(std::is_same_v<stdext::flags<test_flags>::underlying_type, std::uint32_t>);
    static_assert(std::is_default_constructible_v<stdext::flags<test_flags>>);
    static_assert(std::is_copy_constructible_v<stdext::flags<test_flags>>);
    static_assert(std::is_copy_assignable_v<stdext::flags<test_flags>>);
    static_assert(std::is_convertible_v<test_flags, stdext::flags<test_flags>>);
    static_assert(std::is_assignable_v<stdext::flags<test_flags>&, test_flags>);
    static_assert(std::is_constructible_v<stdext::flags<test_flags>, test_flags, test_flags>);
    static_assert(std::is_constructible_v<stdext::flags<test_flags>, test_flags, test_flags, test_flags>);
    static_assert(std::is_constructible_v<stdext::flags<test_flags>, std::uint32_t>);
    static_assert(!std::is_convertible_v<std::uint32_t, stdext::flags<test_flags>>);
    static_assert(stdext::is_equality_comparable_v<stdext::flags<test_flags>>);
    static_assert(stdext::is_equality_comparable_with_v<stdext::flags<test_flags>, test_flags>);
    static_assert(std::is_convertible_v<stdext::flags<test_flags>, test_flags>);
    static_assert(std::is_constructible_v<std::uint32_t, stdext::flags<test_flags>>);
    static_assert(!std::is_convertible_v<stdext::flags<test_flags>, std::uint32_t>);

    TEST_CASE("flags default constructs empty", "[flags]")
    {
        const stdext::flags<test_flags> flags;
        CHECK(flags.value() == test_flags::none);
    }

    TEST_CASE("flags is convertible from enum type", "[flags]")
    {
        const stdext::flags<test_flags> flags = test_flags::flag0;
        CHECK(flags.value() == test_flags::flag0);
    }

    TEST_CASE("flags copy yields same value", "[flags]")
    {
        const stdext::flags<test_flags> flags = test_flags::flag0;
        const stdext::flags<test_flags> copy = flags;
        CHECK(copy.value() == test_flags::flag0);
    }

    TEST_CASE("flags is constructible from multiple flag values (2)", "[flags]")
    {
        const stdext::flags<test_flags> flags = { test_flags::flag0, test_flags::flag1 };
        CHECK(flags.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag1)));
    }

    TEST_CASE("flags is constructible from multiple flag values (3)", "[flags]")
    {
        const stdext::flags<test_flags> flags = { test_flags::flag0, test_flags::flag1, test_flags::flag2 };
        CHECK(flags.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag1) | std::uint32_t(test_flags::flag2)));
    }

    TEST_CASE("flags is constructible from underlying type", "[flags]")
    {
        const stdext::flags<test_flags> flags{std::uint32_t(test_flags::flag3)};
        CHECK(flags.value() == test_flags::flag3);
    }

    TEST_CASE("flags is assignable from enum type", "[flags]")
    {
        stdext::flags<test_flags> flags;
        flags = test_flags::flag0;
        CHECK(flags.value() == test_flags::flag0);
    }

    TEST_CASE("flags is assignable from multiple flag values (2)", "[flags]")
    {
        stdext::flags<test_flags> flags;
        flags = { test_flags::flag0, test_flags::flag1 };
        CHECK(flags.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag1)));
    }

    TEST_CASE("flags is assignable from multiple flag values (3)", "[flags]")
    {
        stdext::flags<test_flags> flags;
        flags = { test_flags::flag0, test_flags::flag1, test_flags::flag2 };
        CHECK(flags.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag1) | std::uint32_t(test_flags::flag2)));
    }

    TEST_CASE("flags instances with equal values compare as equal", "[flags]")
    {
        const stdext::flags<test_flags> a = test_flags::flag0;
        const stdext::flags<test_flags> b = test_flags::flag0;
        CHECK(a == b);
        CHECK(b == a);
        CHECK_FALSE(a != b);
        CHECK_FALSE(b != a);
    }

    TEST_CASE("flags instances with unequal values compare as unequal", "[flags]")
    {
        const stdext::flags<test_flags> a = test_flags::flag0;
        const stdext::flags<test_flags> b = test_flags::flag1;
        CHECK_FALSE(a == b);
        CHECK_FALSE(b == a);
        CHECK(a != b);
        CHECK(b != a);
    }

    TEST_CASE("flags compares equal with equal enum value", "[flags]")
    {
        const stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a == test_flags::flag0);
        CHECK(test_flags::flag0 == a);
        CHECK_FALSE(a != test_flags::flag0);
        CHECK_FALSE(test_flags::flag0 != a);
    }

    TEST_CASE("flags compares unequal with unequal enum value", "[flags]")
    {
        const stdext::flags<test_flags> a = test_flags::flag1;
        CHECK_FALSE(a == test_flags::flag0);
        CHECK_FALSE(test_flags::flag0 == a);
        CHECK(a != test_flags::flag0);
        CHECK(test_flags::flag0 != a);
    }

    TEST_CASE("flags is convertible to enum type", "[flags]")
    {
        const stdext::flags<test_flags> a = test_flags::flag0;
        const test_flags b = a;
        CHECK(b == test_flags::flag0);
    }

    TEST_CASE("flags is explicitly convertible to underlying type", "[flags]")
    {
        const stdext::flags<test_flags> a = test_flags::flag0;
        const std::uint32_t b(a);
        CHECK(b == std::uint32_t(test_flags::flag0));
    }

    TEST_CASE("flags::value() returns most recently set value", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a = test_flags::flag1;
        CHECK(a.value() == test_flags::flag1);
    }

    TEST_CASE("flags::clear() resets value to default", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        a.clear();
        CHECK(a.value() == test_flags::none);
    }


    TEST_CASE("flags::clear() supports chaining", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        a.clear().clear();
        CHECK(a.value() == test_flags::none);
    }

    TEST_CASE("flags::set() sets individual flag values without reseting others", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a.set(test_flags::flag2);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2)));
        a.set(test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.set(test_flags::flag2);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::set() supports multiple flag values", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a.set(test_flags::flag2, test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::set() supports chaining", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a.set(test_flags::flag2).set(test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::reset() resets individual flag values without reseting others", "[flags]")
    {
        stdext::flags<test_flags> a = { test_flags::flag0, test_flags::flag2, test_flags::flag3 };
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.reset(test_flags::flag2);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag3)));
        a.reset(test_flags::flag3);
        CHECK(a.value() == test_flags::flag0);
        a.reset(test_flags::flag2);
        CHECK(a.value() == test_flags::flag0);
    }

    TEST_CASE("flags::reset() supports multiple flag values", "[flags]")
    {
        stdext::flags<test_flags> a = { test_flags::flag0, test_flags::flag2, test_flags::flag3 };
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.reset(test_flags::flag2, test_flags::flag3);
        CHECK(a.value() == test_flags::flag0);
    }

    TEST_CASE("flags::reset() supports chaining", "[flags]")
    {
        stdext::flags<test_flags> a = { test_flags::flag0, test_flags::flag2, test_flags::flag3 };
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.reset(test_flags::flag2).reset(test_flags::flag3);
        CHECK(a.value() == test_flags::flag0);
    }

    TEST_CASE("flags::toggle() toggles individual flag values without reseting others", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a.toggle(test_flags::flag2);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2)));
        a.toggle(test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.toggle(test_flags::flag2);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::toggle() supports multiple flag values", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a.toggle(test_flags::flag2, test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::toggle() supports chaining", "[flags]")
    {
        stdext::flags<test_flags> a = test_flags::flag0;
        CHECK(a.value() == test_flags::flag0);
        a.toggle(test_flags::flag2).toggle(test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::keep() keeps individual flag values, reseting all others", "[flags]")
    {
        stdext::flags<test_flags> a = { test_flags::flag0, test_flags::flag2, test_flags::flag3 };
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.keep(test_flags::flag2);
        CHECK(a.value() == test_flags::flag2);
        a.keep(test_flags::flag3);
        CHECK(a.value() == test_flags::none);
    }

    TEST_CASE("flags::keep() supports multiple flag values", "[flags]")
    {
        stdext::flags<test_flags> a = { test_flags::flag0, test_flags::flag2, test_flags::flag3 };
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.keep(test_flags::flag2, test_flags::flag3);
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
    }

    TEST_CASE("flags::keep() supports chaining", "[flags]")
    {
        stdext::flags<test_flags> a = { test_flags::flag0, test_flags::flag2, test_flags::flag3 };
        CHECK(a.value() == test_flags(std::uint32_t(test_flags::flag0) | std::uint32_t(test_flags::flag2) | std::uint32_t(test_flags::flag3)));
        a.keep(test_flags::flag2).keep(test_flags::flag3);
        CHECK(a.value() == test_flags::none);
    }

    TEST_CASE("flags::test_any() returns true if any provided value is set", "[flags]")
    {
        const stdext::flags<test_flags> a = { test_flags::flag1, test_flags::flag3 };
        CHECK_FALSE(a.test_any(test_flags::flag0));
        CHECK(a.test_any(test_flags::flag1));
        CHECK_FALSE(a.test_any(test_flags::flag2));
        CHECK(a.test_any(test_flags::flag3));
        CHECK(a.test_any(test_flags::flag0, test_flags::flag1));
        CHECK(a.test_any(test_flags::flag2, test_flags::flag3));
        CHECK_FALSE(a.test_any(test_flags::flag0, test_flags::flag2));
        CHECK(a.test_any(test_flags::flag1, test_flags::flag3));
    }

    TEST_CASE("flags::test_any() returns true if all provided values are set", "[flags]")
    {
        const stdext::flags<test_flags> a = { test_flags::flag1, test_flags::flag3 };
        CHECK_FALSE(a.test_all(test_flags::flag0));
        CHECK(a.test_all(test_flags::flag1));
        CHECK_FALSE(a.test_all(test_flags::flag2));
        CHECK(a.test_all(test_flags::flag3));
        CHECK_FALSE(a.test_all(test_flags::flag0, test_flags::flag1));
        CHECK_FALSE(a.test_all(test_flags::flag2, test_flags::flag3));
        CHECK_FALSE(a.test_all(test_flags::flag0, test_flags::flag2));
        CHECK(a.test_all(test_flags::flag1, test_flags::flag3));
    }

    TEST_CASE("flags::test() returns true if all flag values nominated by the mask match corresponding provided values")
    {
        const stdext::flags<test_flags> a = { test_flags::flag1, test_flags::flag3 };
        CHECK(a.test({ }, { }));
        CHECK_FALSE(a.test({ test_flags::flag1 }, { }));
        CHECK(a.test({ test_flags::flag1 }, { test_flags::flag1 }));
        CHECK_FALSE(a.test({ test_flags::flag3 }, { }));
        CHECK(a.test({ test_flags::flag3 }, { test_flags::flag3 }));
        CHECK_FALSE(a.test({ test_flags::flag1, test_flags::flag3 }, { }));
        CHECK_FALSE(a.test({ test_flags::flag1, test_flags::flag3 }, { test_flags::flag1 }));
        CHECK_FALSE(a.test({ test_flags::flag1, test_flags::flag3 }, { test_flags::flag3 }));
        CHECK(a.test({ test_flags::flag1, test_flags::flag3 }, { test_flags::flag1, test_flags::flag3 }));
        CHECK(a.test({ test_flags::flag0 }, { }));
        CHECK_FALSE(a.test({ test_flags::flag0 }, { test_flags::flag0 }));
        CHECK(a.test({ test_flags::flag2 }, { }));
        CHECK_FALSE(a.test({ test_flags::flag2 }, { test_flags::flag2 }));
        CHECK(a.test({ test_flags::flag0, test_flags::flag2 }, { }));
        CHECK_FALSE(a.test({ test_flags::flag0, test_flags::flag2 }, { test_flags::flag0 }));
        CHECK_FALSE(a.test({ test_flags::flag0, test_flags::flag2 }, { test_flags::flag2 }));
        CHECK_FALSE(a.test({ test_flags::flag0, test_flags::flag2 }, { test_flags::flag0, test_flags::flag2 }));
        CHECK(a.test({ test_flags::flag1, test_flags::flag2, test_flags::flag3 }, { test_flags::flag1, test_flags::flag3 }));
        CHECK_FALSE(a.test({ test_flags::flag1, test_flags::flag2, test_flags::flag3 }, { test_flags::flag1, test_flags::flag2, test_flags::flag3 }));
    }
}
