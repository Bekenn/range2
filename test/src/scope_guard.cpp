#include <stdext/scope_guard.h>

#include <catch2/catch.hpp>


namespace test
{
    TEST_CASE("scope_guard deduction guide", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                stdext::scope_guard g([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 1);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                stdext::scope_guard g([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 1);
            }
        }
    }

    TEST_CASE("make_scope_guard", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                auto g = stdext::make_scope_guard([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 1);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                auto g = stdext::make_scope_guard([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 1);
            }
        }
    }

    TEST_CASE("at_scope_exit", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                at_scope_exit([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 1);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                at_scope_exit([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 1);
            }
        }
    }

    TEST_CASE("scope_success_guard deduction guide", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                stdext::scope_success_guard g([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 1);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                stdext::scope_success_guard g([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 0);
            }
        }
    }

    TEST_CASE("make_scope_success_guard", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                auto g = stdext::make_scope_success_guard([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 1);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                auto g = stdext::make_scope_success_guard([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 0);
            }
        }
    }

    TEST_CASE("at_scope_success", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                at_scope_success([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 1);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                at_scope_success([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 0);
            }
        }
    }

    TEST_CASE("scope_error_guard deduction guide", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                stdext::scope_error_guard g([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 0);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                stdext::scope_error_guard g([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 1);
            }
        }
    }

    TEST_CASE("make_scope_error_guard", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                auto g = stdext::make_scope_error_guard([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 0);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                auto g = stdext::make_scope_error_guard([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 1);
            }
        }
    }

    TEST_CASE("at_scope_error", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                at_scope_error([&]{ x = 1; });
                CHECK(x == 0);
            }
            CHECK(x == 0);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                at_scope_error([&]{ x = 1; });
                CHECK(x == 0);
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 1);
            }
        }
    }

    TEST_CASE("scope_guard invalidate", "[scope_guard]")
    {
        int x = 0;

        SECTION("ordinary scope exit")
        {
            {
                stdext::scope_guard g([&]{ x = 1; });
                CHECK(x == 0);
                g.invalidate();
            }
            CHECK(x == 0);
        }

        SECTION("exceptional scope exit")
        {
            try
            {
                stdext::scope_guard g([&]{ x = 1; });
                CHECK(x == 0);
                g.invalidate();
                throw 0;
            }
            catch (int)
            {
                CHECK(x == 0);
            }
        }
    }

    TEST_CASE("scope_success_guard invalidate", "[scope_guard]")
    {
        int x = 0;

        {
            stdext::scope_success_guard g([&]{ x = 1; });
            CHECK(x == 0);
            g.invalidate();
        }
        CHECK(x == 0);
    }

    TEST_CASE("scope_error_guard invalidate", "[scope_guard]")
    {
        int x = 0;

        try
        {
            stdext::scope_error_guard g([&]{ x = 1; });
            CHECK(x == 0);
            g.invalidate();
            throw 0;
        }
        catch (int)
        {
            CHECK(x == 0);
        }
    }
}
