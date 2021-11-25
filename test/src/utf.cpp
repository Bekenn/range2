#include <stdext/file.h>
#include <stdext/stream.h>
#include <stdext/unicode.h>

#include <catch2/catch.hpp>

#include <algorithm>
#include <memory>


TEST_CASE("Unicode conversion UTF-8 to UTF-8", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-8-test.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-8-post.txt"));
    stdext::stream_generator<char> generator(infile);

    SECTION("to_utf8_length")
    {
        auto [result, size] = stdext::to_utf8_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char));
    }

    SECTION("to_utf8")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        auto consumer = stdext::stream_consumer<char>(out);
        REQUIRE(generator >> stdext::to_utf8() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-8 to UTF-16", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-8-test.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-16-post.txt"));
    stdext::stream_generator<char> generator(infile);

    SECTION("to_utf16_length")
    {
        auto [result, size] = stdext::to_utf16_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char16_t));
    }

    SECTION("to_utf16")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        stdext::stream_consumer<char16_t> consumer(out);
        REQUIRE(generator >> stdext::to_utf16() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-8 to UTF-32", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-8-test.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-32-post.txt"));
    auto generator = stdext::stream_generator<char>(infile);

    SECTION("to_utf32_length")
    {
        auto [result, size] = stdext::to_utf32_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char32_t));
    }

    SECTION("to_utf32")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        auto consumer = stdext::stream_consumer<char32_t>(out);
        REQUIRE(generator >> stdext::to_utf32() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-16 to UTF-8", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-16-post.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-8-post.txt"));
    auto generator = stdext::stream_generator<char16_t>(infile);

    SECTION("to_utf8_length")
    {
        auto [result, size] = stdext::to_utf8_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char));
    }

    SECTION("to_utf8")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        auto consumer = stdext::stream_consumer<char>(out);
        REQUIRE(generator >> stdext::to_utf8() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-16 to UTF-16", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-16-post.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-16-post.txt"));
    stdext::stream_generator<char16_t> generator(infile);

    SECTION("to_utf16_length")
    {
        auto [result, size] = stdext::to_utf16_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char16_t));
    }

    SECTION("to_utf16")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        stdext::stream_consumer<char16_t> consumer(out);
        REQUIRE(generator >> stdext::to_utf16() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-16 to UTF-32", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-16-post.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-32-post.txt"));
    auto generator = stdext::stream_generator<char16_t>(infile);

    SECTION("to_utf32_length")
    {
        auto [result, size] = stdext::to_utf32_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char32_t));
    }

    SECTION("to_utf32")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        auto consumer = stdext::stream_consumer<char32_t>(out);
        REQUIRE(generator >> stdext::to_utf32() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-32 to UTF-8", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-32-post.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-8-post.txt"));
    auto generator = stdext::stream_generator<char32_t>(infile);

    SECTION("to_utf8_length")
    {
        auto [result, size] = stdext::to_utf8_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char));
    }

    SECTION("to_utf8")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        auto consumer = stdext::stream_consumer<char>(out);
        REQUIRE(generator >> stdext::to_utf8() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-32 to UTF-16", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-32-post.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-16-post.txt"));
    stdext::stream_generator<char32_t> generator(infile);

    SECTION("to_utf16_length")
    {
        auto [result, size] = stdext::to_utf16_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char16_t));
    }

    SECTION("to_utf16")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        stdext::stream_consumer<char16_t> consumer(out);
        REQUIRE(generator >> stdext::to_utf16() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}

TEST_CASE("Unicode conversion UTF-32 to UTF-32", "[unicode]")
{
    stdext::file_input_stream infile(PATH_STR("UTF-32-post.txt"));
    stdext::file_input_stream testfile(PATH_STR("UTF-32-post.txt"));
    auto generator = stdext::stream_generator<char32_t>(infile);

    SECTION("to_utf32_length")
    {
        auto [result, size] = stdext::to_utf32_length(generator, stdext::utf_conversion_options::sanitize);
        REQUIRE(result == stdext::utf_result::ok);
        REQUIRE(size == testfile.end_position() / sizeof(char32_t));
    }

    SECTION("to_utf32")
    {
        auto buffer_size = size_t(testfile.end_position());
        auto buffer = std::make_unique<std::byte[]>(buffer_size);
        stdext::memory_output_stream out(buffer.get(), buffer_size);

        auto consumer = stdext::stream_consumer<char32_t>(out);
        REQUIRE(generator >> stdext::to_utf32() >> consumer);
        REQUIRE(out.position() == out.end_position());

        REQUIRE(std::equal(buffer.get(), buffer.get() + buffer_size, stdext::input_stream_iterator<std::byte>(testfile)));
    }
}
