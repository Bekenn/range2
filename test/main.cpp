//
//  main.cpp
//  test
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#include <iolib/stream.h>

#undef NDEBUG
#include <cassert>

using namespace std;
using namespace iolib;

template <class T, size_t length>
constexpr size_t lengthof(T (&)[length])
{
    return length;
}

const uint8_t stuff[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
};

template <class POD, size_t size>
void test(memory_input_stream& is, POD (&data)[size])
{
    is.reset(stuff, sizeof(stuff));
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

int main(int argc, char* argv[])
{
    uint8_t data8[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
    };
    uint16_t data16[] =
    {
        0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0b0a, 0x0d0c, 0x0f0e
    };
    uint32_t data32[] =
    {
        0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c
    };

    memory_input_stream is(stuff, sizeof(stuff));
    test(is, data8);
    test(is, data16);
    test(is, data32);
}
