//
//  main.cpp
//  test
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#include <stdext/stream.h>

#include <catch2/catch.hpp>


namespace
{
    const std::byte stuff[] =
    {
        std::byte(0), std::byte(1), std::byte(2), std::byte(3), std::byte(4), std::byte(5), std::byte(6), std::byte(7),
        std::byte(8), std::byte(9), std::byte(0xa), std::byte(0xb), std::byte(0xc), std::byte(0xd), std::byte(0xe), std::byte(0xf)
    };

    template <typename POD, size_t size>
    void test(const POD (&data)[size])
    {
        stdext::memory_input_stream is(stuff, sizeof(stuff));
        for (auto n : data)
            REQUIRE(n == is.read<POD>());

        is.reset(stuff, sizeof(stuff));
        for (auto n : data)
            REQUIRE(n == is.read<POD>());

        is.seek(stdext::seek_from::begin, 0);
        for (auto n : data)
            REQUIRE(n == is.read<POD>());

        is.seek(stdext::seek_from::begin, 0);
        stdext::input_stream_iterator<POD> i(is);
        for (auto n : data)
            REQUIRE(n == *i++);
        REQUIRE(i == stdext::input_stream_iterator<POD>());

        is.seek(stdext::seek_from::begin, 0);
        i = stdext::input_stream_iterator<POD>(is);
        for (auto n : data)
            REQUIRE(n == *i++);
        REQUIRE(i == stdext::input_stream_iterator<POD>());

        is.seek(stdext::seek_from::begin, 0);
        i = stdext::input_stream_iterator<POD>(is);
        for (auto n : data)
        {
            REQUIRE(n == *i);
            ++i;
        }
        REQUIRE(i == stdext::input_stream_iterator<POD>());
    }
}

TEST_CASE("Stream operations", "[stream]")
{
    static const std::uint8_t data8[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
    };
    static const std::uint16_t data16[] =
    {
        0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0b0a, 0x0d0c, 0x0f0e
    };
    static const std::uint32_t data32[] =
    {
        0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c
    };

    test(data8);
    test(data16);
    test(data32);
}
