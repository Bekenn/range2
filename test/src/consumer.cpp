#include <stdext/consumer.h>

#include "iterators.h"

#include <catch2/catch.hpp>

#include <functional>


namespace test
{
    // is_consumer
    static_assert(stdext::is_consumer<bool (int), int>::value);
    static_assert(stdext::is_consumer<bool (int), int&>::value);
    static_assert(!stdext::is_consumer<bool (int&), int>::value);
    static_assert(stdext::is_consumer<bool (int&), int&>::value);
    static_assert(stdext::is_consumer<bool (int&&), int>::value);
    static_assert(!stdext::is_consumer<bool (int&&), int&>::value);
    static_assert(!stdext::is_consumer<void (int), int>::value);
    static_assert(stdext::is_consumer<int (int), int>::value);
    static_assert(stdext::is_consumer<bool (int), short>::value);
    static_assert(!stdext::is_consumer<bool (int), int*>::value);
    static_assert(stdext::is_consumer<bool (*)(int), int>::value);
    static_assert(stdext::is_consumer<bool (&)(int), int>::value);
    static_assert(stdext::is_consumer<std::function<bool (int)>, int>::value);
    static_assert(stdext::is_consumer<stdext::iterator_consumer<int*>, int>::value);
    static_assert(stdext::is_consumer<stdext::delimited_iterator_consumer<int*>, int>::value);

    // is_consumer_v
    static_assert(stdext::is_consumer_v<bool (int), int>);
    static_assert(stdext::is_consumer_v<bool (int), int&>);
    static_assert(!stdext::is_consumer_v<bool (int&), int>);
    static_assert(stdext::is_consumer_v<bool (int&), int&>);
    static_assert(stdext::is_consumer_v<bool (int&&), int>);
    static_assert(!stdext::is_consumer_v<bool (int&&), int&>);
    static_assert(!stdext::is_consumer_v<void (int), int>);
    static_assert(stdext::is_consumer_v<int (int), int>);
    static_assert(stdext::is_consumer_v<bool (int), short>);
    static_assert(!stdext::is_consumer_v<bool (int), int*>);
    static_assert(stdext::is_consumer_v<bool (*)(int), int>);
    static_assert(stdext::is_consumer_v<bool (&)(int), int>);
    static_assert(stdext::is_consumer_v<std::function<bool (int)>, int>);
    static_assert(stdext::is_consumer_v<stdext::iterator_consumer<int*>, int>);
    static_assert(stdext::is_consumer_v<stdext::delimited_iterator_consumer<int*>, int>);

    // iterator_consumer
    TEST_CASE("iterator_consumer", "[consumer]")
    {
        stdext::iterator_consumer consume(fixed_output_iterator{});
        REQUIRE(consume(0));
        CHECK(fixed_output_iterator::value == 0);
        REQUIRE(consume(1));
        CHECK(fixed_output_iterator::value == 1);
        REQUIRE(consume(1729));
        CHECK(fixed_output_iterator::value == 1729);
    }

    // delimited_iterator_consumer
    TEST_CASE("delimited_iterator_consumer", "[consumer]")
    {
        int a[3];
        stdext::delimited_iterator_consumer consume(std::begin(a), std::end(a));
        REQUIRE(consume(0));
        REQUIRE(consume(1));
        REQUIRE(consume(1729));
        CHECK(a[0] == 0);
        CHECK(a[1] == 1);
        CHECK(a[2] == 1729);
        CHECK_FALSE(consume(420));
    }

    // make_consumer
    static_assert(std::is_same_v<decltype(stdext::make_consumer<int>(fixed_output_iterator())), stdext::iterator_consumer<fixed_output_iterator>>);
    static_assert(std::is_same_v<decltype(stdext::make_consumer<int>(std::declval<int*>(), std::declval<int*>())), stdext::delimited_iterator_consumer<int*, int*>>);
}
