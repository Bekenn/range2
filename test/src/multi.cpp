#include <stdext/multi.h>

#include <catch2/catch.hpp>


namespace test
{
    namespace
    {
        struct interface0
        {
            virtual void f() = 0;
        };

        struct interface1
        {
            virtual void g() = 0;
        };

        struct interface2
        {
            virtual void h() = 0;
        };

        struct implementation : interface0, interface1, interface2
        {
            void f() override { }
            void g() override { }
            void h() override { }
        };
    }

    static_assert(!std::is_default_constructible_v<stdext::multi_ref<>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<>, int>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, int&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, implementation&>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, stdext::multi_ref<interface0>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, stdext::multi_ref<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, stdext::multi_ref<interface0, interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, interface0&>);
    static_assert(std::is_constructible_v<stdext::multi_ref<>, interface1&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<>, interface0&, interface1&>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ref<>>);
    static_assert(!std::is_copy_assignable_v<stdext::multi_ref<>>);
    static_assert(!stdext::is_equality_comparable_v<stdext::multi_ref<>>);
    static_assert(std::is_same_v<decltype(&stdext::declval<stdext::multi_ref<>>()), stdext::multi_ptr<>>);

    static_assert(!std::is_default_constructible_v<stdext::multi_ref<interface0>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0>, int>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0>, int&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0>, implementation&>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0>, stdext::multi_ref<interface0>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0>, stdext::multi_ref<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0>, stdext::multi_ref<interface0, interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0>, interface0&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0>, interface1&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0>, interface0&, interface1&>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ref<interface0>>);
    static_assert(!std::is_copy_assignable_v<stdext::multi_ref<interface0>>);
    static_assert(!stdext::is_equality_comparable_v<stdext::multi_ref<interface0>>);
    static_assert(std::is_same_v<decltype(&stdext::declval<stdext::multi_ref<interface0>>()), stdext::multi_ptr<interface0>>);

    static_assert(!std::is_default_constructible_v<stdext::multi_ref<interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface1>, int>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface1>, int&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface1>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface1>, implementation&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface1>, stdext::multi_ref<interface0>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface1>, stdext::multi_ref<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface1>, stdext::multi_ref<interface0, interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface1>, interface0&>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface1>, interface1&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface1>, interface0&, interface1&>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ref<interface1>>);
    static_assert(!std::is_copy_assignable_v<stdext::multi_ref<interface1>>);
    static_assert(!stdext::is_equality_comparable_v<stdext::multi_ref<interface1>>);
    static_assert(std::is_same_v<decltype(&stdext::declval<stdext::multi_ref<interface1>>()), stdext::multi_ptr<interface1>>);

    static_assert(!std::is_default_constructible_v<stdext::multi_ref<interface0, interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, int>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, int&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0, interface1>, implementation&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, stdext::multi_ref<interface0>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, stdext::multi_ref<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0, interface1>, stdext::multi_ref<interface0, interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, interface0&>);
    static_assert(!std::is_constructible_v<stdext::multi_ref<interface0, interface1>, interface1&>);
    static_assert(std::is_constructible_v<stdext::multi_ref<interface0, interface1>, interface0&, interface1&>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ref<interface0, interface1>>);
    static_assert(!std::is_copy_assignable_v<stdext::multi_ref<interface0, interface1>>);
    static_assert(!stdext::is_equality_comparable_v<stdext::multi_ref<interface0, interface1>>);
    static_assert(std::is_same_v<decltype(&stdext::declval<stdext::multi_ref<interface0, interface1>>()), stdext::multi_ptr<interface0, interface1>>);

    TEST_CASE("multi_ref<interface0>", "[multi]")
    {
        implementation impl;
        stdext::multi_ref<interface0> r = impl;

        SECTION("is copyable")
        {
            stdext::multi_ref<interface0> r1 = r;
            CHECK(&r1.as<interface0>() == &r.as<interface0>());
        }

        SECTION("can retrieve reference to interface0")
        {
            static_assert(std::is_same_v<decltype(r.as<interface0>()), interface0&>);
            interface0& ri0 = r.as<interface0>();
            CHECK(&ri0 == &impl);
        }

        SECTION("can retrieve reference to const interface0")
        {
            static_assert(std::is_same_v<decltype(r.as_const<interface0>()), const interface0&>);
            const interface0& ri0 = r.as_const<interface0>();
            CHECK(&ri0 == &impl);
        }

        SECTION("can rebind to other object")
        {
            implementation impl1;
            r.rebind(impl1);

            SECTION("can retrieve reference to interface0")
            {
                interface0& ri0 = r.as<interface0>();
                CHECK(&ri0 == &impl1);
            }

            SECTION("can retrieve reference to const interface0")
            {
                const interface0& ri0 = r.as_const<interface0>();
                CHECK(&ri0 == &impl1);
            }
        }

        SECTION("can rebind to other multi_ref")
        {
            implementation impl1;
            stdext::multi_ref<interface0> r1 = impl1;
            CHECK(&r.as<interface0>() != &r1.as<interface0>());
            r.rebind(r1);
            CHECK(&r.as<interface0>() == &r1.as<interface0>());
        }

        SECTION("can rebind to compatible multi_ref")
        {
            implementation impl1;
            stdext::multi_ref<interface0, interface1> r1 = impl1;
            CHECK(&r.as<interface0>() != &r1.as<interface0>());
            r.rebind(r1);
            CHECK(&r.as<interface0>() == &r1.as<interface0>());
        }

        SECTION("operator& returns corresponding multi_ptr")
        {
            stdext::multi_ptr<interface0> p = &r;
            CHECK(p.as<interface0>() == &r.as<interface0>());
        }
    }

    TEST_CASE("multi_ref<interface0, interface1>", "[multi]")
    {
        implementation impl;
        stdext::multi_ref<interface0, interface1> r = impl;

        SECTION("is copyable")
        {
            stdext::multi_ref<interface0, interface1> r1 = r;
            CHECK(&r1.as<interface0>() == &r.as<interface0>());
            CHECK(&r1.as<interface1>() == &r.as<interface1>());
        }

        SECTION("can construct less-constrained multi_ref")
        {
            stdext::multi_ref<interface0> r1 = r;
            CHECK(&r1.as<interface0>() == &r.as<interface0>());
        }

        SECTION("can retrieve reference to interface0")
        {
            static_assert(std::is_same_v<decltype(r.as<interface0>()), interface0&>);
            interface0& ri0 = r.as<interface0>();
            CHECK(&ri0 == &impl);
        }

        SECTION("can retrieve reference to interface1")
        {
            static_assert(std::is_same_v<decltype(r.as<interface1>()), interface1&>);
            interface1& ri1 = r.as<interface1>();
            CHECK(&ri1 == &impl);
        }

        SECTION("can retrieve reference to const interface0")
        {
            static_assert(std::is_same_v<decltype(r.as_const<interface0>()), const interface0&>);
            const interface0& ri0 = r.as_const<interface0>();
            CHECK(&ri0 == &impl);
        }

        SECTION("can retrieve reference to const interface1")
        {
            static_assert(std::is_same_v<decltype(r.as_const<interface1>()), const interface1&>);
            const interface1& ri1 = r.as_const<interface1>();
            CHECK(&ri1 == &impl);
        }

        SECTION("can rebind to other object")
        {
            implementation impl1;
            r.rebind(impl1);

            SECTION("can retrieve reference to interface0")
            {
                interface0& ri0 = r.as<interface0>();
                CHECK(&ri0 == &impl1);
            }

            SECTION("can retrieve reference to interface1")
            {
                interface1& ri1 = r.as<interface1>();
                CHECK(&ri1 == &impl1);
            }

            SECTION("can retrieve reference to const interface0")
            {
                const interface0& ri0 = r.as_const<interface0>();
                CHECK(&ri0 == &impl1);
            }

            SECTION("can retrieve reference to const interface1")
            {
                const interface1& ri1 = r.as_const<interface1>();
                CHECK(&ri1 == &impl1);
            }
        }

        SECTION("can rebind to other multi_ref")
        {
            implementation impl1;
            stdext::multi_ref<interface0, interface1> r1 = impl1;
            CHECK(&r.as<interface0>() != &r1.as<interface0>());
            CHECK(&r.as<interface1>() != &r1.as<interface1>());
            r.rebind(r1);
            CHECK(&r.as<interface0>() == &r1.as<interface0>());
            CHECK(&r.as<interface1>() == &r1.as<interface1>());
        }

        SECTION("can rebind to compatible multi_ref (extra)")
        {
            implementation impl1;
            stdext::multi_ref<interface0, interface1, interface2> r1 = impl1;
            CHECK(&r.as<interface0>() != &r1.as<interface0>());
            CHECK(&r.as<interface1>() != &r1.as<interface1>());
            r.rebind(r1);
            CHECK(&r.as<interface0>() == &r1.as<interface0>());
            CHECK(&r.as<interface1>() == &r1.as<interface1>());
        }

        SECTION("can rebind to compatible multi_ref (reversed)")
        {
            implementation impl1;
            stdext::multi_ref<interface1, interface0> r1 = impl1;
            CHECK(&r.as<interface0>() != &r1.as<interface0>());
            CHECK(&r.as<interface1>() != &r1.as<interface1>());
            r.rebind(r1);
            CHECK(&r.as<interface0>() == &r1.as<interface0>());
            CHECK(&r.as<interface1>() == &r1.as<interface1>());
        }

        SECTION("operator& returns corresponding multi_ptr")
        {
            stdext::multi_ptr<interface0, interface1> p = &r;
            CHECK(p.as<interface0>() == &r.as<interface0>());
            CHECK(p.as<interface1>() == &r.as<interface1>());
        }
    }

    static_assert(std::is_default_constructible_v<stdext::multi_ptr<>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<>, int>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, int*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, implementation*>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, stdext::multi_ptr<interface0>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, stdext::multi_ptr<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, stdext::multi_ptr<interface0, interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, interface0*>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<>, interface1*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<>, interface0*, interface1*>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ptr<>>);
    static_assert(std::is_copy_assignable_v<stdext::multi_ptr<>>);
    static_assert(stdext::is_equality_comparable_v<stdext::multi_ptr<>>);
    static_assert(stdext::is_equality_comparable_with_v<const stdext::multi_ptr<>&, const stdext::multi_ptr<interface0>&>);
    static_assert(stdext::is_equality_comparable_with_v<const stdext::multi_ptr<>&, const stdext::multi_ptr<interface1>&>);
    static_assert(stdext::is_equality_comparable_with_v<const stdext::multi_ptr<>&, const stdext::multi_ptr<interface0, interface1>&>);
    static_assert(std::is_same_v<decltype(*stdext::declval<stdext::multi_ptr<>>()), stdext::multi_ref<>>);

    static_assert(std::is_default_constructible_v<stdext::multi_ptr<interface0>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0>, int>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0>, int*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0>, implementation*>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0>, stdext::multi_ptr<interface0>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0>, stdext::multi_ptr<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0>, stdext::multi_ptr<interface0, interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0>, interface0*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0>, interface1*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0>, interface0*, interface1*>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ptr<interface0>>);
    static_assert(std::is_copy_assignable_v<stdext::multi_ptr<interface0>>);
    static_assert(stdext::is_equality_comparable_v<stdext::multi_ptr<interface0>>);
    static_assert(stdext::is_equality_comparable_with_v<const stdext::multi_ptr<interface0>&, const stdext::multi_ptr<interface0>&>);
    static_assert(!stdext::is_equality_comparable_with_v<const stdext::multi_ptr<interface0>&, const stdext::multi_ptr<interface1>&>);
    static_assert(stdext::is_equality_comparable_with_v<const stdext::multi_ptr<interface0>&, const stdext::multi_ptr<interface0, interface1>&>);
    static_assert(std::is_same_v<decltype(*stdext::declval<stdext::multi_ptr<interface0>>()), stdext::multi_ref<interface0>>);

    static_assert(std::is_default_constructible_v<stdext::multi_ptr<interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface1>, int>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface1>, int*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface1>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface1>, implementation*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface1>, stdext::multi_ptr<interface0>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface1>, stdext::multi_ptr<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface1>, stdext::multi_ptr<interface0, interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface1>, interface0*>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface1>, interface1*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface1>, interface0*, interface1*>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ptr<interface1>>);
    static_assert(std::is_copy_assignable_v<stdext::multi_ptr<interface1>>);
    static_assert(stdext::is_equality_comparable_v<stdext::multi_ptr<interface1>>);
    static_assert(std::is_same_v<decltype(*stdext::declval<stdext::multi_ptr<interface1>>()), stdext::multi_ref<interface1>>);

    static_assert(std::is_default_constructible_v<stdext::multi_ptr<interface0, interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, int>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, int*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, implementation>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, implementation*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, stdext::multi_ptr<interface0>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, stdext::multi_ptr<interface1>>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, stdext::multi_ptr<interface0, interface1>>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, interface0*>);
    static_assert(!std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, interface1*>);
    static_assert(std::is_constructible_v<stdext::multi_ptr<interface0, interface1>, interface0*, interface1*>);
    static_assert(std::is_copy_constructible_v<stdext::multi_ptr<interface0, interface1>>);
    static_assert(std::is_copy_assignable_v<stdext::multi_ptr<interface0, interface1>>);
    static_assert(stdext::is_equality_comparable_v<stdext::multi_ptr<interface0, interface1>>);
    static_assert(std::is_same_v<decltype(*stdext::declval<stdext::multi_ptr<interface0, interface1>>()), stdext::multi_ref<interface0, interface1>>);

    TEST_CASE("multi_ptr<interface0>", "[multi]")
    {
        SECTION("default constructed is equal to nullptr")
        {
            stdext::multi_ptr<interface0> p;
            CHECK(p.as<interface0>() == nullptr);
            CHECK(p == nullptr);
        }

        SECTION("from nullptr")
        {
            stdext::multi_ptr<interface0> p = nullptr;
            CHECK(p.as<interface0>() == nullptr);
            CHECK(p == nullptr);
        }

        SECTION("to live object")
        {
            implementation impl;
            stdext::multi_ptr<interface0> p = &impl;

            SECTION("is copyable")
            {
                stdext::multi_ptr<interface0> p1 = p;
                CHECK(p1.as<interface0>() == p.as<interface0>());
            }

            SECTION("is equality comparable")
            {
                stdext::multi_ptr<interface0> p1 = p;
                CHECK(p1 == p);
                implementation impl1;
                p1 = &impl1;
                CHECK(p1 != p);
            }

            SECTION("can retrieve pointer to interface0")
            {
                static_assert(std::is_same_v<decltype(p.as<interface0>()), interface0*>);
                interface0* pi0 = p.as<interface0>();
                CHECK(pi0 == &impl);
            }

            SECTION("can retrieve pointer to const interface0")
            {
                static_assert(std::is_same_v<decltype(p.as_const<interface0>()), const interface0*>);
                const interface0* pi0 = p.as_const<interface0>();
                CHECK(pi0 == &impl);
            }

            SECTION("can assign from nullptr")
            {
                p = nullptr;
                CHECK(p.as<interface0>() == nullptr);
                CHECK(p == nullptr);
            }

            SECTION("can assign from other object")
            {
                implementation impl1;
                p = &impl1;

                SECTION("can retrieve pointer to interface0")
                {
                    interface0* pi0 = p.as<interface0>();
                    CHECK(pi0 == &impl1);
                }

                SECTION("can retrieve pointer to const interface0")
                {
                    const interface0* pi0 = p.as_const<interface0>();
                    CHECK(pi0 == &impl1);
                }
            }

            SECTION("can assign from other multi_ptr")
            {
                implementation impl1;
                stdext::multi_ptr<interface0> p1 = &impl1;
                CHECK(p.as<interface0>() != p1.as<interface0>());
                p = p1;
                CHECK(p.as<interface0>() == p1.as<interface0>());
            }

            SECTION("can assign from compatible multi_ptr")
            {
                implementation impl1;
                stdext::multi_ptr<interface0, interface1> p1 = &impl1;
                CHECK(p.as<interface0>() != p1.as<interface0>());
                p = p1;
                CHECK(p.as<interface0>() == p1.as<interface0>());
            }

            SECTION("operator* returns corresponding multi_ref")
            {
                stdext::multi_ref<interface0> r = *p;
                CHECK(&r.as<interface0>() == p.as<interface0>());
            }
        }
    }

    TEST_CASE("multi_ptr<interface0, interface1>", "[multi]")
    {
        SECTION("default constructed is equal to nullptr")
        {
            stdext::multi_ptr<interface0, interface1> p;
            CHECK(p.as<interface0>() == nullptr);
            CHECK(p.as<interface1>() == nullptr);
            CHECK(p == nullptr);
        }

        SECTION("from nullptr")
        {
            stdext::multi_ptr<interface0, interface1> p = nullptr;
            CHECK(p.as<interface0>() == nullptr);
            CHECK(p.as<interface1>() == nullptr);
            CHECK(p == nullptr);
        }

        SECTION("to live object")
        {
            implementation impl;
            stdext::multi_ptr<interface0, interface1> p = &impl;

            SECTION("is copyable")
            {
                stdext::multi_ptr<interface0, interface1> p1 = p;
                CHECK(p1.as<interface0>() == p.as<interface0>());
                CHECK(p1.as<interface1>() == p.as<interface1>());
            }

            SECTION("is equality comparable")
            {
                stdext::multi_ptr<interface0> p1 = p;
                CHECK(p1 == p);
                implementation impl1;
                p1 = &impl1;
                CHECK(p1 != p);
            }

            SECTION("can construct less-constrained multi_ptr")
            {
                stdext::multi_ptr<interface0> p1 = p;
                CHECK(p1.as<interface0>() == p.as<interface0>());
            }

            SECTION("can compare for equality with less-constrained multi_ptr")
            {
                stdext::multi_ptr<interface0> p1 = p;
                CHECK(p1 == p);
            }

            SECTION("can retrieve pointer to interface0")
            {
                static_assert(std::is_same_v<decltype(p.as<interface0>()), interface0*>);
                interface0* pi0 = p.as<interface0>();
                CHECK(pi0 == &impl);
            }

            SECTION("can retrieve pointer to interface1")
            {
                static_assert(std::is_same_v<decltype(p.as<interface1>()), interface1*>);
                interface1* pi1 = p.as<interface1>();
                CHECK(pi1 == &impl);
            }

            SECTION("can retrieve pointer to const interface0")
            {
                static_assert(std::is_same_v<decltype(p.as_const<interface0>()), const interface0*>);
                const interface0* pi0 = p.as_const<interface0>();
                CHECK(pi0 == &impl);
            }

            SECTION("can retrieve pointer to const interface1")
            {
                static_assert(std::is_same_v<decltype(p.as_const<interface1>()), const interface1*>);
                const interface1* pi1 = p.as_const<interface1>();
                CHECK(pi1 == &impl);
            }

            SECTION("can assign from nullptr")
            {
                p = nullptr;
                CHECK(p.as<interface0>() == nullptr);
                CHECK(p.as<interface1>() == nullptr);
                CHECK(p == nullptr);
            }

            SECTION("can assign from other object")
            {
                implementation impl1;
                p = &impl1;

                SECTION("can retrieve pointer to interface0")
                {
                    interface0* pi0 = p.as<interface0>();
                    CHECK(pi0 == &impl1);
                }

                SECTION("can retrieve pointer to interface1")
                {
                    interface1* pi1 = p.as<interface1>();
                    CHECK(pi1 == &impl1);
                }

                SECTION("can retrieve pointer to const interface0")
                {
                    const interface0* pi0 = p.as_const<interface0>();
                    CHECK(pi0 == &impl1);
                }

                SECTION("can retrieve pointer to const interface1")
                {
                    const interface1* pi1 = p.as_const<interface1>();
                    CHECK(pi1 == &impl1);
                }
            }

            SECTION("can assign from other multi_ptr")
            {
                implementation impl1;
                stdext::multi_ptr<interface0, interface1> p1 = &impl1;
                CHECK(p.as<interface0>() != p1.as<interface0>());
                CHECK(p.as<interface1>() != p1.as<interface1>());
                p = p1;
                CHECK(p.as<interface0>() == p1.as<interface0>());
                CHECK(p.as<interface1>() == p1.as<interface1>());
            }

            SECTION("can assign from compatible multi_ptr (extra)")
            {
                implementation impl1;
                stdext::multi_ptr<interface0, interface1, interface2> p1 = &impl1;
                CHECK(p.as<interface0>() != p1.as<interface0>());
                CHECK(p.as<interface1>() != p1.as<interface1>());
                p = p1;
                CHECK(p.as<interface0>() == p1.as<interface0>());
                CHECK(p.as<interface1>() == p1.as<interface1>());
            }

            SECTION("can assign from compatible multi_ptr (reversed)")
            {
                implementation impl1;
                stdext::multi_ptr<interface1, interface0> p1 = &impl1;
                CHECK(p.as<interface0>() != p1.as<interface0>());
                CHECK(p.as<interface1>() != p1.as<interface1>());
                p = p1;
                CHECK(p.as<interface0>() == p1.as<interface0>());
                CHECK(p.as<interface1>() == p1.as<interface1>());
            }

            SECTION("operator* returns corresponding multi_ref")
            {
                stdext::multi_ref<interface0, interface1> r = *p;
                CHECK(&r.as<interface0>() == p.as<interface0>());
                CHECK(&r.as<interface1>() == p.as<interface1>());
            }
        }
    }
}
