#include <stdext/span.h>

#include <catch2/catch.hpp>

#include <array>


namespace test
{
    static_assert(std::is_trivially_copy_constructible_v<stdext::span<int>>);
    static_assert(std::is_trivially_move_constructible_v<stdext::span<int>>);
    static_assert(std::is_trivially_copy_assignable_v<stdext::span<int>>);
    static_assert(std::is_trivially_move_assignable_v<stdext::span<int>>);
    static_assert(std::is_trivially_destructible_v<stdext::span<int>>);

    static int x[5];

    TEST_CASE("span default constructor", "[span]")
    {
        static constexpr stdext::span<int> s;
        STATIC_REQUIRE(s.data() == nullptr);
        STATIC_REQUIRE(s.size() == 0);
    }

    TEST_CASE("span constructor from pointer and length", "[span]")
    {
        SECTION("from same type")
        {
            static constexpr stdext::span<int> s(std::data(x), std::size(x));
            STATIC_REQUIRE(s.data() == std::data(x));
            STATIC_REQUIRE(s.size() == std::size(x));
        }

        SECTION("with qualification conversion")
        {
            static constexpr stdext::span<const int> s(std::data(x), std::size(x));
            STATIC_REQUIRE(s.data() == std::data(x));
            STATIC_REQUIRE(s.size() == std::size(x));
        }
    }

    TEST_CASE("span constructor from two pointers", "[span]")
    {
        SECTION("from same type")
        {
            static constexpr stdext::span<int> s(std::begin(x), std::end(x));
            STATIC_REQUIRE(s.data() == std::data(x));
            STATIC_REQUIRE(s.size() == std::size(x));
        }

        SECTION("with qualification conversion")
        {
            static constexpr stdext::span<const int> s(std::begin(x), std::end(x));
            STATIC_REQUIRE(s.data() == std::data(x));
            STATIC_REQUIRE(s.size() == std::size(x));
        }
    }

    TEST_CASE("span constructor from array", "[span]")
    {
        SECTION("from same type")
        {
            static constexpr stdext::span<int> s = x;
            STATIC_REQUIRE(s.data() == std::data(x));
            STATIC_REQUIRE(s.size() == std::size(x));
        }

        SECTION("with qualification conversion")
        {
            static constexpr stdext::span<const int> s = x;
            STATIC_REQUIRE(s.data() == std::data(x));
            STATIC_REQUIRE(s.size() == std::size(x));
        }
    }

    TEST_CASE("span constructor from contiguous range", "[span]")
    {
        static std::array<int, 5> x;

        SECTION("from same type")
        {
            static constexpr stdext::span<int> s = x;
            STATIC_REQUIRE(s.data() == x.data());
            STATIC_REQUIRE(s.size() == x.size());
        }

        SECTION("with qualification conversion")
        {
            static constexpr stdext::span<const int> s = x;
            STATIC_REQUIRE(s.data() == x.data());
            STATIC_REQUIRE(s.size() == x.size());
        }
    }

    TEST_CASE("span copy constructor", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<int> b = a;
        STATIC_REQUIRE(b.data() == std::data(x));
        STATIC_REQUIRE(b.size() == std::size(x));
    }

    TEST_CASE("span constructor from span of qualified type", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<const int> b = a;
        STATIC_REQUIRE(b.data() == std::data(x));
        STATIC_REQUIRE(b.size() == std::size(x));
    }

    TEST_CASE("span copy assignment", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<int> b = [](stdext::span<int> a)
        {
            stdext::span<int> s;
            s = a;
            return s;
        }(a);
        STATIC_REQUIRE(b.data() == std::data(x));
        STATIC_REQUIRE(b.size() == std::size(x));
    }

    TEST_CASE("leading subspan of n elements", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<int> b = a.first(3);
        STATIC_REQUIRE(b.data() == std::data(x));
        STATIC_REQUIRE(b.size() == 3);
    }

    TEST_CASE("trailing subspan of n elements", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<int> b = a.last(3);
        STATIC_REQUIRE(b.data() == std::data(x) + std::size(x) - 3);
        STATIC_REQUIRE(b.size() == std::size(x) - 3);
    }

    TEST_CASE("trailing subspan from offset", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<int> b = a.subspan(2);
        STATIC_REQUIRE(b.data() == std::data(x) + 2);
        STATIC_REQUIRE(b.size() == std::size(x) - 2);
    }

    TEST_CASE("internal subspan of n elements", "[span]")
    {
        static constexpr stdext::span<int> a = x;
        static constexpr stdext::span<int> b = a.subspan(1, 3);
        STATIC_REQUIRE(b.data() == std::data(x) + 1);
        STATIC_REQUIRE(b.size() == 3);
    }

    TEST_CASE("query for empty span", "[span]")
    {
        static constexpr stdext::span<int> a;
        STATIC_REQUIRE(a.empty());
        static constexpr stdext::span<int> b = x;
        STATIC_REQUIRE(!b.empty());
    }

    TEMPLATE_TEST_CASE("span index operator", "[span]", int, const int)
    {
        static constexpr stdext::span<TestType> s = x;
        static_assert(std::is_same_v<decltype(s[std::declval<size_t>()]), TestType&>);
        STATIC_REQUIRE(&s[0] == &x[0]);
        STATIC_REQUIRE(&s[1] == &x[1]);
        STATIC_REQUIRE(&s[2] == &x[2]);
        STATIC_REQUIRE(&s[3] == &x[3]);
        STATIC_REQUIRE(&s[4] == &x[4]);
    }

    TEST_CASE("access to first element via front()")
    {
        static constexpr stdext::span<int> s = x;
        static_assert(std::is_same_v<decltype(s.front()), int&>);
        STATIC_REQUIRE(&s.front() == &x[0]);
    }

    TEST_CASE("access to first element via back()")
    {
        static constexpr stdext::span<int> s = x;
        static_assert(std::is_same_v<decltype(s.back()), int&>);
        STATIC_REQUIRE(&s.back() == &x[4]);
    }

    TEST_CASE("iterator access to beginning of span")
    {
        static constexpr stdext::span<int> s = x;
        STATIC_REQUIRE(s.begin() == std::begin(x));
    }

    TEST_CASE("iterator access to end of span")
    {
        static constexpr stdext::span<int> s = x;
        STATIC_REQUIRE(s.end() == std::end(x));
    }

    TEST_CASE("iterator access to beginning of reversed span")
    {
        static constexpr stdext::span<int> s = x;
        STATIC_REQUIRE(s.rbegin().base() == std::end(x));
    }

    TEST_CASE("iterator access to end of reversed span")
    {
        static constexpr stdext::span<int> s = x;
        STATIC_REQUIRE(s.rend().base() == std::begin(x));
    }

    TEST_CASE("view as range of bytes")
    {
        const int x[] = { 0, 1, 2, 3, 4 };
        stdext::span<const int> s = x;
        stdext::span<const std::byte> bytes = stdext::as_bytes(s);
        CHECK(bytes.data() == reinterpret_cast<const std::byte*>(std::data(x)));
        CHECK(bytes.size() == sizeof(x));
    }

    TEST_CASE("view as range of modifiable bytes")
    {
        int x[] = { 0, 1, 2, 3, 4 };
        stdext::span<int> s = x;
        stdext::span<std::byte> bytes = stdext::as_writable_bytes(s);
        CHECK(bytes.data() == reinterpret_cast<std::byte*>(std::data(x)));
        CHECK(bytes.size() == sizeof(x));
    }
}
