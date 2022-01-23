#include <stdext/generator.h>

#include "iterators.h"

#include <catch2/catch.hpp>


namespace test
{
    namespace
    {
        class generator
        {
        public:
            using iterator_category = stdext::generator_tag;
            using value_type = int;
            using difference_type = std::ptrdiff_t;
            using pointer = const int*;
            using reference = int;

        public:
            STDEXT_PUSH_WARNINGS()
            STDEXT_DISABLE_WARNING_CLANG("-Wunneeded-internal-declaration")
            friend bool operator == (const generator& a, const generator& b) noexcept { return &a == &b; }
            friend bool operator != (const generator& a, const generator& b) noexcept { return &a != &b; }
            STDEXT_POP_WARNINGS()

        public:
            explicit operator bool () const noexcept { return true; }
            reference operator * () const noexcept { return value; }
            pointer operator -> () const noexcept { return &value; }
            generator& operator ++ () noexcept { ++value; return *this; }
            stdext::iterator_proxy<generator> operator ++ (int) noexcept { return value++; }

        private:
            static inline int value;
        };
    }

    // generator_tag
    static_assert(std::is_base_of_v<std::input_iterator_tag, stdext::generator_tag>);
    static_assert(!std::is_base_of_v<std::output_iterator_tag, stdext::generator_tag>);
    static_assert(!std::is_base_of_v<std::forward_iterator_tag, stdext::generator_tag>);

    // is_generator
    static_assert(!stdext::is_generator<void>::value);
    static_assert(!stdext::is_generator<int>::value);
    static_assert(!stdext::is_generator<int[]>::value);
    static_assert(!stdext::is_generator<int[5]>::value);
    static_assert(!stdext::is_generator<int ()>::value);
    static_assert(!stdext::is_generator<const void>::value);
    static_assert(!stdext::is_generator<const int>::value);
    static_assert(!stdext::is_generator<const int[]>::value);
    static_assert(!stdext::is_generator<const int[5]>::value);
    static_assert(!stdext::is_generator<void*>::value);
    static_assert(!stdext::is_generator<int*>::value);
    static_assert(!stdext::is_generator<int (*)[]>::value);
    static_assert(!stdext::is_generator<int (*)[5]>::value);
    static_assert(!stdext::is_generator<int (*)()>::value);
    static_assert(!stdext::is_generator<const void*>::value);
    static_assert(!stdext::is_generator<const int*>::value);
    static_assert(!stdext::is_generator<const int (*)[]>::value);
    static_assert(!stdext::is_generator<const int (*)[5]>::value);
    static_assert(!stdext::is_generator<fixed_input_iterator>::value);
    static_assert(!stdext::is_generator<fixed_output_iterator>::value);
    static_assert(!stdext::is_generator<input_iterator>::value);
    static_assert(!stdext::is_generator<output_iterator>::value);
    static_assert(!stdext::is_generator<forward_iterator>::value);
    static_assert(!stdext::is_generator<bidirectional_iterator>::value);
    static_assert(!stdext::is_generator<random_access_iterator>::value);
    static_assert(stdext::is_generator<generator>::value);

    // is_generator_v
    static_assert(!stdext::is_generator_v<void>);
    static_assert(!stdext::is_generator_v<int>);
    static_assert(!stdext::is_generator_v<int[]>);
    static_assert(!stdext::is_generator_v<int[5]>);
    static_assert(!stdext::is_generator_v<int ()>);
    static_assert(!stdext::is_generator_v<const void>);
    static_assert(!stdext::is_generator_v<const int>);
    static_assert(!stdext::is_generator_v<const int[]>);
    static_assert(!stdext::is_generator_v<const int[5]>);
    static_assert(!stdext::is_generator_v<void*>);
    static_assert(!stdext::is_generator_v<int*>);
    static_assert(!stdext::is_generator_v<int (*)[]>);
    static_assert(!stdext::is_generator_v<int (*)[5]>);
    static_assert(!stdext::is_generator_v<int (*)()>);
    static_assert(!stdext::is_generator_v<const void*>);
    static_assert(!stdext::is_generator_v<const int*>);
    static_assert(!stdext::is_generator_v<const int (*)[]>);
    static_assert(!stdext::is_generator_v<const int (*)[5]>);
    static_assert(!stdext::is_generator_v<fixed_input_iterator>);
    static_assert(!stdext::is_generator_v<fixed_output_iterator>);
    static_assert(!stdext::is_generator_v<input_iterator>);
    static_assert(!stdext::is_generator_v<output_iterator>);
    static_assert(!stdext::is_generator_v<forward_iterator>);
    static_assert(!stdext::is_generator_v<bidirectional_iterator>);
    static_assert(!stdext::is_generator_v<random_access_iterator>);
    static_assert(stdext::is_generator_v<generator>);

    // is_generator_adaptable
    static_assert(!stdext::is_generator_adaptable<void>::value);
    static_assert(!stdext::is_generator_adaptable<int>::value);
    static_assert(!stdext::is_generator_adaptable<int[]>::value);
    static_assert(!stdext::is_generator_adaptable<int[5]>::value);
    static_assert(stdext::is_generator_adaptable<int ()>::value);
    static_assert(!stdext::is_generator_adaptable<const void>::value);
    static_assert(!stdext::is_generator_adaptable<const int>::value);
    static_assert(!stdext::is_generator_adaptable<const int[]>::value);
    static_assert(stdext::is_generator_adaptable<const int[5]>::value);
    static_assert(!stdext::is_generator_adaptable<void*>::value);
    static_assert(!stdext::is_generator_adaptable<int*>::value);
    static_assert(!stdext::is_generator_adaptable<int (*)[]>::value);
    static_assert(!stdext::is_generator_adaptable<int (*)[5]>::value);
    static_assert(stdext::is_generator_adaptable<int (*)()>::value);
    static_assert(!stdext::is_generator_adaptable<const void*>::value);
    static_assert(!stdext::is_generator_adaptable<const int*>::value);
    static_assert(!stdext::is_generator_adaptable<const int (*)[]>::value);
    static_assert(!stdext::is_generator_adaptable<const int (*)[5]>::value);
    static_assert(!stdext::is_generator_adaptable<fixed_input_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<fixed_output_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<input_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<output_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<forward_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<bidirectional_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<random_access_iterator>::value);
    static_assert(!stdext::is_generator_adaptable<generator>::value);

    // is_generator_adaptable_v
    static_assert(!stdext::is_generator_adaptable_v<void>);
    static_assert(!stdext::is_generator_adaptable_v<int>);
    static_assert(!stdext::is_generator_adaptable_v<int[]>);
    static_assert(!stdext::is_generator_adaptable_v<int[5]>);
    static_assert(stdext::is_generator_adaptable_v<int ()>);
    static_assert(!stdext::is_generator_adaptable_v<const void>);
    static_assert(!stdext::is_generator_adaptable_v<const int>);
    static_assert(!stdext::is_generator_adaptable_v<const int[]>);
    static_assert(stdext::is_generator_adaptable_v<const int[5]>);
    static_assert(!stdext::is_generator_adaptable_v<void*>);
    static_assert(!stdext::is_generator_adaptable_v<int*>);
    static_assert(!stdext::is_generator_adaptable_v<int (*)[]>);
    static_assert(!stdext::is_generator_adaptable_v<int (*)[5]>);
    static_assert(stdext::is_generator_adaptable_v<int (*)()>);
    static_assert(!stdext::is_generator_adaptable_v<const void*>);
    static_assert(!stdext::is_generator_adaptable_v<const int*>);
    static_assert(!stdext::is_generator_adaptable_v<const int (*)[]>);
    static_assert(!stdext::is_generator_adaptable_v<const int (*)[5]>);
    static_assert(!stdext::is_generator_adaptable_v<fixed_input_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<fixed_output_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<input_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<output_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<forward_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<bidirectional_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<random_access_iterator>);
    static_assert(!stdext::is_generator_adaptable_v<generator>);

    // can_generate
    static_assert(!stdext::can_generate<void>::value);
    static_assert(!stdext::can_generate<int>::value);
    static_assert(!stdext::can_generate<int[]>::value);
    static_assert(!stdext::can_generate<int[5]>::value);
    static_assert(stdext::can_generate<int ()>::value);
    static_assert(!stdext::can_generate<const void>::value);
    static_assert(!stdext::can_generate<const int>::value);
    static_assert(!stdext::can_generate<const int[]>::value);
    static_assert(stdext::can_generate<const int[5]>::value);
    static_assert(!stdext::can_generate<void*>::value);
    static_assert(!stdext::can_generate<int*>::value);
    static_assert(!stdext::can_generate<int (*)[]>::value);
    static_assert(!stdext::can_generate<int (*)[5]>::value);
    static_assert(stdext::can_generate<int (*)()>::value);
    static_assert(!stdext::can_generate<const void*>::value);
    static_assert(!stdext::can_generate<const int*>::value);
    static_assert(!stdext::can_generate<const int (*)[]>::value);
    static_assert(!stdext::can_generate<const int (*)[5]>::value);
    static_assert(!stdext::can_generate<fixed_input_iterator>::value);
    static_assert(!stdext::can_generate<fixed_output_iterator>::value);
    static_assert(!stdext::can_generate<input_iterator>::value);
    static_assert(!stdext::can_generate<output_iterator>::value);
    static_assert(!stdext::can_generate<forward_iterator>::value);
    static_assert(!stdext::can_generate<bidirectional_iterator>::value);
    static_assert(!stdext::can_generate<random_access_iterator>::value);
    static_assert(stdext::can_generate<generator>::value);

    // can_generate_v
    static_assert(!stdext::can_generate_v<void>);
    static_assert(!stdext::can_generate_v<int>);
    static_assert(!stdext::can_generate_v<int[]>);
    static_assert(!stdext::can_generate_v<int[5]>);
    static_assert(stdext::can_generate_v<int ()>);
    static_assert(!stdext::can_generate_v<const void>);
    static_assert(!stdext::can_generate_v<const int>);
    static_assert(!stdext::can_generate_v<const int[]>);
    static_assert(stdext::can_generate_v<const int[5]>);
    static_assert(!stdext::can_generate_v<void*>);
    static_assert(!stdext::can_generate_v<int*>);
    static_assert(!stdext::can_generate_v<int (*)[]>);
    static_assert(!stdext::can_generate_v<int (*)[5]>);
    static_assert(stdext::can_generate_v<int (*)()>);
    static_assert(!stdext::can_generate_v<const void*>);
    static_assert(!stdext::can_generate_v<const int*>);
    static_assert(!stdext::can_generate_v<const int (*)[]>);
    static_assert(!stdext::can_generate_v<const int (*)[5]>);
    static_assert(!stdext::can_generate_v<fixed_input_iterator>);
    static_assert(!stdext::can_generate_v<fixed_output_iterator>);
    static_assert(!stdext::can_generate_v<input_iterator>);
    static_assert(!stdext::can_generate_v<output_iterator>);
    static_assert(!stdext::can_generate_v<forward_iterator>);
    static_assert(!stdext::can_generate_v<bidirectional_iterator>);
    static_assert(!stdext::can_generate_v<random_access_iterator>);
    static_assert(stdext::can_generate_v<generator>);

    // generator_value_type
    static_assert(std::is_same_v<stdext::generator_value_type<generator>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::delimited_iterator_generator<int*>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::delimited_iterator_generator<const int*>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::function_generator<int (*)()>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::function_generator<stdext::optional<int> (*)()>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::function_generator<stdext::optional<const int> (*)()>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::constant_generator<int>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::terminated_generator<int*, bool (*)(int)>>, int>);
    static_assert(std::is_same_v<stdext::generator_value_type<stdext::terminated_generator<const int*, bool (*)(int)>>, int>);

    // generator_difference_type
    static_assert(std::is_same_v<stdext::generator_difference_type<generator>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::delimited_iterator_generator<int*>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::delimited_iterator_generator<const int*>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::function_generator<int (*)()>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::function_generator<stdext::optional<int> (*)()>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::function_generator<stdext::optional<const int> (*)()>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::constant_generator<int>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::terminated_generator<int*, bool (*)(int)>>, std::ptrdiff_t>);
    static_assert(std::is_same_v<stdext::generator_difference_type<stdext::terminated_generator<const int*, bool (*)(int)>>, std::ptrdiff_t>);

    // generator_pointer_type
    static_assert(std::is_same_v<stdext::generator_pointer_type<generator>, const int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::delimited_iterator_generator<int*>>, int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::delimited_iterator_generator<const int*>>, const int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::function_generator<int (*)()>>, const int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::function_generator<stdext::optional<int> (*)()>>, const int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::function_generator<stdext::optional<const int> (*)()>>, const int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::constant_generator<int>>, const int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::terminated_generator<int*, bool (*)(int)>>, int*>);
    static_assert(std::is_same_v<stdext::generator_pointer_type<stdext::terminated_generator<const int*, bool (*)(int)>>, const int*>);

    // generator_reference_type
    static_assert(std::is_same_v<stdext::generator_reference_type<generator>, int>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::delimited_iterator_generator<int*>>, int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::delimited_iterator_generator<const int*>>, const int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::function_generator<int (*)()>>, const int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::function_generator<stdext::optional<int> (*)()>>, const int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::function_generator<stdext::optional<const int> (*)()>>, const int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::constant_generator<int>>, const int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::terminated_generator<int*, bool (*)(int)>>, int&>);
    static_assert(std::is_same_v<stdext::generator_reference_type<stdext::terminated_generator<const int*, bool (*)(int)>>, const int&>);

    TEST_CASE("delimited_iterator_generator", "[generator]")
    {
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int*>(), std::declval<int* const>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int*>(), std::declval<int* const&>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int*&>(), std::declval<int* const>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int*&>(), std::declval<int* const&>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int* const>(), std::declval<int*>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int* const>(), std::declval<int*&>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int* const&>(), std::declval<int*>())), stdext::delimited_iterator_generator<int*, int*>>);
        static_assert(std::is_same_v<decltype(stdext::delimited_iterator_generator(std::declval<int* const&>(), std::declval<int*&>())), stdext::delimited_iterator_generator<int*, int*>>);

        const int values[] = { 3, 1, 4, 1, 5, 9 };
        input_iterator i(values);
        input_iterator j(values + std::size(values));
        stdext::delimited_iterator_generator<input_iterator> g(i, j);

        SECTION("copies compare equal")
        {
            stdext::delimited_iterator_generator<input_iterator> h = g;
            CHECK(g == h);
            CHECK(h == g);
            CHECK_FALSE(g != h);
            CHECK_FALSE(h != g);
        }

        SECTION("distinct values compare unequal")
        {
            stdext::delimited_iterator_generator<input_iterator> h(j, j);
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("different end iterators compare unequal")
        {
            stdext::delimited_iterator_generator<input_iterator> h(i, i);
            REQUIRE(g.base() == h.base());
            REQUIRE(g.end() != h.end());
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("swap")
        {
            stdext::delimited_iterator_generator<input_iterator> h(j, j);

            stdext::delimited_iterator_generator<input_iterator> test_g = g;
            stdext::delimited_iterator_generator<input_iterator> test_h = h;
            REQUIRE(g == test_g);
            REQUIRE(g != test_h);
            REQUIRE(h != test_g);
            REQUIRE(h == test_h);

            stdext::swap(g, h);
            CHECK(g != test_g);
            CHECK(g == test_h);
            CHECK(h == test_g);
            CHECK(h != test_h);

            stdext::swap(g, h);
            CHECK(g == test_g);
            CHECK(g != test_h);
            CHECK(h != test_g);
            CHECK(h == test_h);
        }

        SECTION("base and end")
        {
            CHECK(g.base() == i);
            CHECK(g.base() != j);
            CHECK(g.end() == j);
            ++g;
            CHECK(g.base() != i);
            CHECK(g.base() != j);
            CHECK(g.end() == j);
            g = stdext::delimited_iterator_generator<input_iterator>(j, j);
            CHECK(g.base() != i);
            CHECK(g.base() == j);
            CHECK(g.end() == j);
        }

        SECTION("preincrement")
        {
            REQUIRE(g.base() == i);
            CHECK(&++g == &g);
            CHECK(g.base() != i);
            ++i;
            CHECK(g.base() == i);
        }

        SECTION("postincrement")
        {
            REQUIRE(g.base() == i);
            CHECK(*g++ == *i);
            CHECK(g.base() != i);
            ++i;
            CHECK(g.base() == i);
        }

        SECTION("dereference matches iterator dereference")
        {
            REQUIRE(g.base() == i);
            CHECK(*g == *i);
            CHECK(g.operator->() == i);

            ++g;
            ++i;
            REQUIRE(g.base() == i);
            CHECK(*g == *i);
            CHECK(g.operator->() == i);
        }

        SECTION("conversion to bool")
        {
            REQUIRE(g.base() == i);
            while (g.base() != j)
            {
                CHECK(g);
                ++g;
            }
            CHECK_FALSE(g);
        }

        SECTION("make_generator from iterator/sentinel pair")
        {
            CHECK(stdext::make_generator(i, j) == g);
        }

        SECTION("make_generator from array")
        {
            auto f = std::begin(values);
            auto l = std::end(values);
            auto h = stdext::make_generator(values);
            static_assert(std::is_same_v<decltype(h), stdext::delimited_iterator_generator<const int*, const int*>>);

            REQUIRE(h.end() == l);
            while (h.base() != l)
            {
                REQUIRE(h.base() == f);
                REQUIRE(h);
                CHECK(*h == *f);
                ++h;
                ++f;
            }

            CHECK_FALSE(h);
        }
    }

    TEST_CASE("counted_iterator_generator", "[generator]")
    {
        static_assert(std::is_same_v<decltype(stdext::counted_iterator_generator(std::declval<int*>(), 5)), stdext::counted_iterator_generator<int*>>);
        static_assert(std::is_same_v<decltype(stdext::counted_iterator_generator(std::declval<int*&>(), 5)), stdext::counted_iterator_generator<int*>>);
        static_assert(std::is_same_v<decltype(stdext::counted_iterator_generator(std::declval<int* const>(), 5)), stdext::counted_iterator_generator<int*>>);
        static_assert(std::is_same_v<decltype(stdext::counted_iterator_generator(std::declval<int* const&>(), 5)), stdext::counted_iterator_generator<int*>>);

        const int values[] = { 3, 1, 4, 1, 5, 9 };
        input_iterator i(values);
        input_iterator j(values + std::size(values));
        stdext::counted_iterator_generator<input_iterator> g(i, std::size(values));

        SECTION("copies compare equal")
        {
            stdext::counted_iterator_generator<input_iterator> h = g;
            CHECK(g == h);
            CHECK(h == g);
            CHECK_FALSE(g != h);
            CHECK_FALSE(h != g);
        }

        SECTION("distinct values compare unequal")
        {
            stdext::counted_iterator_generator<input_iterator> h(j, 0);
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("different counts compare unequal")
        {
            stdext::counted_iterator_generator<input_iterator> h(i, 0);
            REQUIRE(g.base() == h.base());
            REQUIRE(g.count() != h.count());
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("swap")
        {
            stdext::counted_iterator_generator<input_iterator> h(j, 0);

            stdext::counted_iterator_generator<input_iterator> test_g = g;
            stdext::counted_iterator_generator<input_iterator> test_h = h;
            REQUIRE(g == test_g);
            REQUIRE(g != test_h);
            REQUIRE(h != test_g);
            REQUIRE(h == test_h);

            stdext::swap(g, h);
            CHECK(g != test_g);
            CHECK(g == test_h);
            CHECK(h == test_g);
            CHECK(h != test_h);

            stdext::swap(g, h);
            CHECK(g == test_g);
            CHECK(g != test_h);
            CHECK(h != test_g);
            CHECK(h == test_h);
        }

        SECTION("base and count")
        {
            CHECK(g.base() == i);
            CHECK(g.base() != j);
            CHECK(g.count() == std::size(values));
            ++g;
            CHECK(g.base() != i);
            CHECK(g.base() != j);
            CHECK(g.count() == std::size(values) - 1);
            g = stdext::counted_iterator_generator<input_iterator>(j, 0);
            CHECK(g.base() != i);
            CHECK(g.base() == j);
            CHECK(g.count() == 0);
        }

        SECTION("preincrement")
        {
            REQUIRE(g.base() == i);
            CHECK(&++g == &g);
            CHECK(g.base() != i);
            ++i;
            CHECK(g.base() == i);
        }

        SECTION("postincrement")
        {
            REQUIRE(g.base() == i);
            CHECK(*g++ == *i);
            CHECK(g.base() != i);
            ++i;
            CHECK(g.base() == i);
        }

        SECTION("dereference matches iterator dereference")
        {
            REQUIRE(g.base() == i);
            CHECK(*g == *i);
            CHECK(g.operator->() == i);

            ++g;
            ++i;
            REQUIRE(g.base() == i);
            CHECK(*g == *i);
            CHECK(g.operator->() == i);
        }

        SECTION("conversion to bool")
        {
            REQUIRE(g.base() == i);
            while (g.base() != j)
            {
                CHECK(g);
                ++g;
            }
            CHECK_FALSE(g);
        }

        SECTION("make_counted_generator")
        {
            CHECK(stdext::make_generator(i, std::size(values)) == g);
        }
    }

    TEST_CASE("function_generator", "[generator]")
    {
        struct count_from
        {
            int value;
            int operator () () noexcept { return value++; }
            bool operator == (const count_from& other) const noexcept { return value == other.value; }
            bool operator != (const count_from& other) const noexcept { return value != other.value; }
        };

        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<count_from>())), stdext::function_generator<count_from>>);
        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<count_from&>())), stdext::function_generator<count_from>>);
        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<const count_from>())), stdext::function_generator<count_from>>);
        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<const count_from&>())), stdext::function_generator<count_from>>);

        stdext::function_generator<count_from> g(count_from{0});

        SECTION("copies compare equal")
        {
            stdext::function_generator<count_from> h = g;
            CHECK(g == h);
            CHECK(h == g);
            CHECK_FALSE(g != h);
            CHECK_FALSE(h != g);
        }

        SECTION("distinct values compare unequal")
        {
            stdext::function_generator<count_from> h(count_from{5});
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("swap")
        {
            stdext::function_generator<count_from> h(count_from{5});

            stdext::function_generator<count_from> test_g = g;
            stdext::function_generator<count_from> test_h = h;
            REQUIRE(g == test_g);
            REQUIRE(g != test_h);
            REQUIRE(h != test_g);
            REQUIRE(h == test_h);

            stdext::swap(g, h);
            CHECK(g != test_g);
            CHECK(g == test_h);
            CHECK(h == test_g);
            CHECK(h != test_h);

            stdext::swap(g, h);
            CHECK(g == test_g);
            CHECK(g != test_h);
            CHECK(h != test_g);
            CHECK(h == test_h);
        }

        SECTION("preincrement")
        {
            REQUIRE(*g == 0);
            CHECK(&++g == &g);
            CHECK(*g == 1);
        }

        SECTION("postincrement")
        {
            REQUIRE(*g == 0);
            CHECK(*g++ == 0);
            CHECK(*g == 1);
        }

        SECTION("conversion to bool")
        {
            CHECK(g);
        }

        SECTION("make_generator")
        {
            CHECK(stdext::make_generator(count_from{0}) == g);
        }
    }

    TEST_CASE("function_generator returning optional", "[generator]")
    {
        struct count
        {
            int value;
            unsigned remaining;
            stdext::optional<int> operator () () noexcept { if (remaining == 0) return stdext::nullopt; --remaining; return value++; }
            bool operator == (const count& other) const noexcept { return value == other.value && remaining == other.remaining; }
            bool operator != (const count& other) const noexcept { return value != other.value || remaining != other.remaining; }
        };

        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<count>())), stdext::function_generator<count>>);
        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<count&>())), stdext::function_generator<count>>);
        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<const count>())), stdext::function_generator<count>>);
        static_assert(std::is_same_v<decltype(stdext::function_generator(std::declval<const count&>())), stdext::function_generator<count>>);

        stdext::function_generator<count> g(count{0, 5});

        SECTION("copies compare equal")
        {
            stdext::function_generator<count> h = g;
            CHECK(g == h);
            CHECK(h == g);
            CHECK_FALSE(g != h);
            CHECK_FALSE(h != g);
        }

        SECTION("distinct values compare unequal")
        {
            stdext::function_generator<count> h(count{5, 5});
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("swap")
        {
            stdext::function_generator<count> h(count{5, 5});

            stdext::function_generator<count> test_g = g;
            stdext::function_generator<count> test_h = h;
            REQUIRE(g == test_g);
            REQUIRE(g != test_h);
            REQUIRE(h != test_g);
            REQUIRE(h == test_h);

            stdext::swap(g, h);
            CHECK(g != test_g);
            CHECK(g == test_h);
            CHECK(h == test_g);
            CHECK(h != test_h);

            stdext::swap(g, h);
            CHECK(g == test_g);
            CHECK(g != test_h);
            CHECK(h != test_g);
            CHECK(h == test_h);
        }

        SECTION("preincrement")
        {
            REQUIRE(*g == 0);
            CHECK(&++g == &g);
            CHECK(*g == 1);
        }

        SECTION("postincrement")
        {
            REQUIRE(*g == 0);
            CHECK(*g++ == 0);
            CHECK(*g == 1);
        }

        SECTION("conversion to bool")
        {
            CHECK(g);
            ++g;
            CHECK(g);
            ++g;
            CHECK(g);
            ++g;
            CHECK(g);
            ++g;
            CHECK(g);
            ++g;
            CHECK_FALSE(g);
        }

        SECTION("make_generator")
        {
            CHECK(stdext::make_generator(count{0, 5}) == g);
        }
    }

    TEST_CASE("constant_generator", "[generator]")
    {
        static_assert(std::is_same_v<decltype(stdext::constant_generator(std::declval<int>())), stdext::constant_generator<int>>);
        static_assert(std::is_same_v<decltype(stdext::constant_generator(std::declval<int&>())), stdext::constant_generator<int>>);
        static_assert(std::is_same_v<decltype(stdext::constant_generator(std::declval<const int>())), stdext::constant_generator<int>>);
        static_assert(std::is_same_v<decltype(stdext::constant_generator(std::declval<const int&>())), stdext::constant_generator<int>>);

        stdext::constant_generator<int> g(5);

        SECTION("copies compare equal")
        {
            stdext::constant_generator<int> h = g;
            CHECK(g == h);
            CHECK(h == g);
            CHECK_FALSE(g != h);
            CHECK_FALSE(h != g);
        }

        SECTION("distinct values compare unequal")
        {
            stdext::constant_generator<int> h(3);
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("swap")
        {
            stdext::constant_generator<int> h(3);

            stdext::constant_generator<int> test_g = g;
            stdext::constant_generator<int> test_h = h;
            REQUIRE(g == test_g);
            REQUIRE(g != test_h);
            REQUIRE(h != test_g);
            REQUIRE(h == test_h);

            stdext::swap(g, h);
            CHECK(g != test_g);
            CHECK(g == test_h);
            CHECK(h == test_g);
            CHECK(h != test_h);

            stdext::swap(g, h);
            CHECK(g == test_g);
            CHECK(g != test_h);
            CHECK(h != test_g);
            CHECK(h == test_h);
        }

        SECTION("preincrement")
        {
            REQUIRE(g);
            CHECK(*g == 5);
            CHECK(&++g == &g);
            CHECK(*g == 5);
        }

        SECTION("postincrement")
        {
            REQUIRE(g);
            CHECK(*g++ == 5);
            CHECK(*g == 5);
        }

        SECTION("conversion to bool")
        {
            REQUIRE(g);
            CHECK(*g == 5);
            ++g;
            CHECK(g);
            CHECK(*g == 5);
            ++g;
            CHECK(g);
            CHECK(*g == 5);
            ++g;
            CHECK(g);
            CHECK(*g == 5);
            ++g;
            CHECK(g);
            CHECK(*g == 5);
            ++g;
            CHECK(g);
            CHECK(*g == 5);
        }

        SECTION("make_constant_generator")
        {
            CHECK(stdext::make_constant_generator(5) == g);
        }
    }

    TEST_CASE("terminated_generator", "[generator]")
    {
        struct IsValue
        {
            char terminator;
            bool operator()(char x) const noexcept { return x == terminator; }
            bool operator==(const IsValue& other) const noexcept { return terminator == other.terminator; }
            bool operator!=(const IsValue& other) const noexcept { return terminator != other.terminator; }
        };

        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char*>(), std::declval<const IsValue>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char*>(), std::declval<const IsValue&>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char*&>(), std::declval<const IsValue>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char*&>(), std::declval<const IsValue&>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char* const>(), std::declval<IsValue>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char* const>(), std::declval<IsValue&>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char* const&>(), std::declval<IsValue>())), stdext::terminated_generator<char*, IsValue>>);
        static_assert(std::is_same_v<decltype(stdext::terminated_generator(std::declval<char* const&>(), std::declval<IsValue&>())), stdext::terminated_generator<char*, IsValue>>);

        static constexpr char message[] = "Hello, world!";
        stdext::terminated_generator<const char*, IsValue> g(message, IsValue{'\0'});

        SECTION("copies compare equal")
        {
            stdext::terminated_generator<const char*, IsValue> h = g;
            CHECK(g == h);
            CHECK(h == g);
            CHECK_FALSE(g != h);
            CHECK_FALSE(h != g);
        }

        SECTION("distinct values compare unequal")
        {
            stdext::terminated_generator<const char*, IsValue> h("Greetings, human!", IsValue{'\0'});
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("different predicates compare unequal")
        {
            stdext::terminated_generator<const char*, IsValue> h(message, IsValue{'!'});
            REQUIRE(g.base() == h.base());
            REQUIRE(g.predicate() != h.predicate());
            CHECK_FALSE(g == h);
            CHECK_FALSE(h == g);
            CHECK(g != h);
            CHECK(h != g);
        }

        SECTION("swap")
        {
            stdext::terminated_generator<const char*, IsValue> h("Greetings, human!", IsValue{'\0'});

            stdext::terminated_generator<const char*, IsValue> test_g = g;
            stdext::terminated_generator<const char*, IsValue> test_h = h;
            REQUIRE(g == test_g);
            REQUIRE(g != test_h);
            REQUIRE(h != test_g);
            REQUIRE(h == test_h);

            stdext::swap(g, h);
            CHECK(g != test_g);
            CHECK(g == test_h);
            CHECK(h == test_g);
            CHECK(h != test_h);

            stdext::swap(g, h);
            CHECK(g == test_g);
            CHECK(g != test_h);
            CHECK(h != test_g);
            CHECK(h == test_h);
        }

        SECTION("base and predicate")
        {
            CHECK(g.base() == message);
            CHECK(g.predicate() == IsValue{'\0'});
        }

        SECTION("preincrement")
        {
            REQUIRE(g.base() == message);
            CHECK(&++g == &g);
            CHECK(g.base() == message + 1);
        }

        SECTION("postincrement")
        {
            REQUIRE(g.base() == message);
            CHECK(*g++ == *message);
            CHECK(g.base() == message + 1);
        }

        SECTION("dereference matches iterator dereference")
        {
            REQUIRE(g.base() == message);
            CHECK(*g == *message);
            CHECK(g.operator->() == message);

            ++g;
            REQUIRE(g.base() == message + 1);
            CHECK(*g == *(message + 1));
            CHECK(g.operator->() == message + 1);
        }

        SECTION("conversion to bool")
        {
            REQUIRE(g.base() == message);
            while (!g.predicate()(*g.base()))
            {
                CHECK(g);
                ++g;
            }
            CHECK_FALSE(g);
        }

        SECTION("make_terminated_generator")
        {
            CHECK(stdext::make_terminated_generator(&message[0], IsValue{0}) == g);
        }
    }

    TEST_CASE("as_generator, [generator]")
    {
        const int values[] = { 3, 1, 4, 1, 5, 9 };
        input_iterator i(values);
        input_iterator j(values + std::size(values));

        SECTION("from generator")
        {
            auto gen = stdext::make_generator(i, j);
            decltype(auto) g = stdext::as_generator(gen);
            static_assert(std::is_same_v<decltype(g), decltype(gen)&>);
            CHECK(&g == &gen);
        }

        SECTION("from array")
        {
            auto f = std::begin(values);
            auto l = std::end(values);
            decltype(auto) g = stdext::as_generator(values);
            static_assert(std::is_same_v<decltype(g), stdext::delimited_iterator_generator<const int*, const int*>>);

            REQUIRE(g.end() == l);
            while (g.base() != l)
            {
                REQUIRE(g.base() == f);
                REQUIRE(g);
                CHECK(*g == *f);
                ++g;
                ++f;
            }

            CHECK_FALSE(g);
        }

        SECTION("from callable")
        {
            auto f = [&]() -> stdext::optional<int> { if (i == j) return stdext::nullopt; return *i++; };
            decltype(auto) g = stdext::as_generator(f);
            static_assert(std::is_same_v<decltype(g), stdext::function_generator<decltype(f)>>);

            auto p = std::begin(values);
            auto l = std::end(values);
            while (p != l)
            {
                REQUIRE(g);
                CHECK(*g == *p);
                ++g;
                ++p;
            }

            CHECK_FALSE(g);
        }
    }

    TEST_CASE("generators are consumable", "[generator]")
    {
        const int values[] = { 3, 1, 4, 1, 5, 9 };
        auto i = std::begin(values);
        auto j = std::end(values);

        int output[std::size(values)] = { };
        auto f = std::begin(output);
        auto l = std::end(output);

        auto g = stdext::make_generator(values);
        auto out = [&](int x) -> bool
        {
            if (f == l)
                return false;
            *f++ = x;
            return true;
        };

        REQUIRE(g >> out);
        CHECK(f == l);
        f = std::begin(output);

        while (i != j && f != l)
            CHECK(*i++ == *f++);

        CHECK(i == j);
        CHECK(f == l);
    }
}
