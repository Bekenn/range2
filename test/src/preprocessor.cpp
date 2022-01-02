#include <stdext/preprocessor.h>

#include <catch2/catch.hpp>

#include <cstring>


namespace test
{
    // STDEXT_PP_PASTE
    namespace
    {
        constexpr int foobar = 5;
        static_assert(STDEXT_PP_PASTE(foo, bar) == 5);
    }

    TEST_CASE("STDEXT_PP_STRINGIFY", "[preprocessor]")
    {
        CHECK(std::strcmp(STDEXT_PP_STRINGIFY(hello world), "hello world") == 0);
    }

    TEST_CASE("STDEXT_PP_DEPAREN", "[preprocessor]")
    {
#define STRINGIFY(x) STDEXT_PP_STRINGIFY(x)
        CHECK(std::strcmp(STRINGIFY(x), "x") == 0);
        CHECK(std::strcmp(STRINGIFY((x)), "(x)") == 0);
        CHECK(std::strcmp(STRINGIFY(STDEXT_PP_DEPAREN(x)), "x") == 0);
        CHECK(std::strcmp(STRINGIFY(STDEXT_PP_DEPAREN((x))), "x") == 0);
#undef STRINGIFY
    }
}
