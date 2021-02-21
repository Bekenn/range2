//
//  main.cpp
//  test
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#include <stdext/stream.h>

#undef NDEBUG
#include <cassert>

using namespace std;
using namespace stdext;

template <class T, size_t length>
constexpr size_t lengthof(T (&)[length])
{
    return length;
}

const byte stuff[] =
{
    byte(0), byte(1), byte(2), byte(3), byte(4), byte(5), byte(6), byte(7),
    byte(8), byte(9), byte(0xa), byte(0xb), byte(0xc), byte(0xd), byte(0xe), byte(0xf)
};

template <class POD, size_t size>
void test(const POD (&data)[size])
{
    memory_input_stream is(stuff, sizeof(stuff));
    for (auto n : data)
        assert(n == is.read<POD>());

    is.reset(stuff, sizeof(stuff));
    for (auto n : data)
        assert(n == is.read<POD>());

    is.seek(seek_from::begin, 0);
    for (auto n : data)
        assert(n == is.read<POD>());

    is.seek(seek_from::begin, 0);
    input_stream_iterator<POD> i(is);
    for (auto n : data)
        assert(n == *i++);
    assert(i == input_stream_iterator<POD>());

    is.seek(seek_from::begin, 0);
    i = input_stream_iterator<POD>(is);
    for (auto n : data)
        assert(n == *i++);
    assert(i == input_stream_iterator<POD>());

    is.seek(seek_from::begin, 0);
    i = input_stream_iterator<POD>(is);
    for (auto n : data)
    {
        assert(n == *i);
        ++i;
    }
    assert(i == input_stream_iterator<POD>());
}

int main()
{
    static const uint8_t data8[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
    };
    static const uint16_t data16[] =
    {
        0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0b0a, 0x0d0c, 0x0f0e
    };
    static const uint32_t data32[] =
    {
        0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c
    };

    test(data8);
    test(data16);
    test(data32);
}
