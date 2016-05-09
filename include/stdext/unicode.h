//
//  concept.h
//  stdext
//
//  Created by James Touton on 1/06/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_UNICODE_INCLUDED
#define STDEXT_UNICODE_INCLUDED
#pragma once

#include "range.h"

#include <tuple>


namespace stdext
{
    enum class utf_result
    {
        ok,
        partial_read,
        partial_write,
        error
    };

    struct utfstate_t
    {
        ::std::uint32_t
            code : 24,
            produced : 2,
            consumed : 2,
            remaining : 2;

        utfstate_t() : code(0), produced(0), consumed(0), remaining(0) { }
    };

    constexpr char32_t UNICODE_REPLACEMENT_CHARACTER = 0xFFFD;
    constexpr char32_t INVALID_UNICODE_CHARACTER = char32_t(-1);
    constexpr ::std::size_t MAX_UTF8_CHARACTER_LENGTH = 4;

    inline ::std::size_t utf8_character_length(char first)
    {
        if ((first & 0x80) == 0x00)
            return 1;
        if ((first & 0xE0) == 0xC0)
            return 2;
        if ((first & 0xF0) == 0xE0)
            return 3;
        if ((first & 0xF8) == 0xF0)
            return 4;
        return 0;
    }

    inline bool utf8_is_leading(char code)
    {
        return (code & 0xC0) != 0x80 && (code & 0xF8) != 0xF8;
    }

    inline bool utf8_is_trailing(char code)
    {
        return (code & 0xC0) == 0x80;
    }

    inline bool is_surrogate(char16_t code)
    {
        return (code & 0xF800) == 0xD800;
    }

    inline bool is_leading_surrogate(char16_t code)
    {
        return (code & 0xFC00) == 0xD800;
    }

    inline bool is_trailing_surrogate(char16_t code)
    {
        return (code & 0xFC00) == 0xDC00;
    }

    // Converts as many code units as possible from utf8 to utf32.  This function supports partial conversions, such that
    // all utf8 code units are consumed until either the input buffer has been completely consumed or the output buffer
    // has been completely filled.  If the input buffer ends with a partial character sequence, no corresponding output
    // code point is written; instead, the partial input sequence is consumed and information regarding the partial
    // conversion is stored in state.  When this function is called again, the partial conversion information is used to
    // resume the conversion, treating the new input sequence as an extension of the previous sequence.
    // The first time this function is called for a given input sequence, state should be empty (default constructed).
    // On return, state contains partial conversion information, if any, for the last input character sequence.
    // If the function returns utf_result::ok, then state is empty and in is exhausted.
    // If the function returns utf_result::partial, then state contains partial conversion information and either
    // in is exhausted or out is full.
    // If the function returns utf_result::error, then state is empty and in points one position beyond the input character
    // sequence that could not be converted.
    // The behavior is undefined if state is non-empty and does not contain a value from a previous call to this function.
    ::std::pair<utf_result, char> to_utf8(char16_t in, utfstate_t& state);
    ::std::pair<utf_result, char> to_utf8(char32_t in, utfstate_t& state);
    ::std::pair<utf_result, char16_t> to_utf16(char in, utfstate_t& state);
    ::std::pair<utf_result, char16_t> to_utf16(char32_t in, utfstate_t& state);
    ::std::pair<utf_result, char32_t> to_utf32(char in, utfstate_t& state);
    ::std::pair<utf_result, char32_t> to_utf32(char16_t in, utfstate_t& state);

    namespace detail
    {
        // It would be really nice to have function template partial specialization...
        template <class OutChar, class InChar> struct to_utf_helper;
        template <class InChar>
        struct to_utf_helper<char, InChar>
        {
            template <class... Args> static auto call(Args&&... args) { return to_utf8(::std::forward<Args>(args)...); }
        };
        template <class InChar>
        struct to_utf_helper<char16_t, InChar>
        {
            template <class... Args> static auto call(Args&&... args) { return to_utf16(::std::forward<Args>(args)...); }
        };
        template <class InChar>
        struct to_utf_helper<char32_t, InChar>
        {
            template <class... Args> static auto call(Args&&... args) { return to_utf32(::std::forward<Args>(args)...); }
        };

        template <class OutChar, class InChar>
        ::std::pair<utf_result, OutChar> to_utf(InChar in, utfstate_t& state)
        {
            return to_utf_helper<OutChar, InChar>::call(in, state);
        }

        template <class Char, class Generator, class Consumer>
        utf_result to_utf(Generator&& in, Consumer&& out, utfstate_t& state)
        {
            auto result = utf_result::ok;

            while (in)
            {
                Char code;
                ::std::tie(result, code) = to_utf<Char>(*in, state);
                if (result == utf_result::error)
                    return utf_result::error;
                if (result != utf_result::partial_read)
                {
                    ++in;
                    if (!out(code))
                        return utf_result::partial_write;
                }
            }

            return result;
        }

        template <class Generator, class Function>
        ::std::pair<utf_result, ::std::size_t> to_utf_length(Generator&& in, Function&& convert)
        {
            auto result = ::std::make_pair(utf_result::ok, size_t(0));
            utfstate_t state;
            while (in)
            {
                result.first = convert(*in, state).first;
                if (result.first == utf_result::error)
                    break;
                ++in;
                ++result.second;
            }
            return result;
        }
    }

    template <class Generator, class Consumer>
    utf_result to_utf8(Generator&& in, Consumer&& out, utfstate_t& state)
    {
        return detail::to_utf<char>(::std::forward<Generator>(in), ::std::forward<Consumer>(out), state);
    }

    template <class Generator, class Consumer>
    utf_result to_utf16(Generator&& in, Consumer&& out, utfstate_t& state)
    {
        return detail::to_utf<char16_t>(::std::forward<Generator>(in), ::std::forward<Consumer>(out), state);
    }

    template <class Generator, class Consumer>
    utf_result to_utf32(Generator&& in, Consumer&& out, utfstate_t& state)
    {
        return detail::to_utf<char32_t>(::std::forward<Generator>(in), ::std::forward<Consumer>(out), state);
    }

    template <class Generator>
    ::std::pair<utf_result, size_t> to_utf8_length(Generator&& in)
    {
        return detail::to_utf_length(in, [](auto&&... args) { return to_utf8(::std::forward<decltype(args)>(args)...); });
    }

    template <class Generator>
    ::std::pair<utf_result, size_t> to_utf16_length(Generator&& in)
    {
        return detail::to_utf_length(in, [](auto&&... args) { return to_utf16(::std::forward<decltype(args)>(args)...); });
    }

    template <class Generator>
    ::std::pair<utf_result, ::std::size_t> to_utf32_length(Generator&& in)
    {
        return detail::to_utf_length(in, [](auto&&... args) { return to_utf32(::std::forward<decltype(args)>(args)...); });
    }

    template <class Range>
    class utf8_to_utf32_generator
    {
    public:
        using iterator_category = ::std::input_iterator_tag;
        using value_type = const char32_t;
        using difference_type = difference_type<Range, is_range>;
        using pointer = value_type*;
        using reference = value_type&;
        using generator_category = basic_generator_tag;
        using range = Range;

    public:
        utf8_to_utf32_generator() : r(), value() { }
        explicit utf8_to_utf32_generator(const range& r) : r(r)
        {
            next();
        }
        explicit utf8_to_utf32_generator(range&& r) : r(::std::move(r))
        {
            next();
        }

    public:
        friend bool operator == (const utf8_to_utf32_generator& a, const utf8_to_utf32_generator& b) noexcept
        {
            return a.r == b.r
                && a.value == b.value;
        }
        friend bool operator != (const utf8_to_utf32_generator& a, const utf8_to_utf32_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(utf8_to_utf32_generator& a, utf8_to_utf32_generator& b)
        {
            using ::std::swap;
            swap(a.r, b.r);
            swap(a.value, b.value);
        }

    public:
        reference operator * () { return value; }
        reference operator -> () { return value; }
        utf8_to_utf32_generator& operator ++ () { next(); return *this; }
        iterator_proxy<utf8_to_utf32_generator> operator ++ ()
        {
            iterator_proxy<utf8_to_utf32_generator> proxy = value;
            next();
            return proxy;
        }

    private:
        void next()
        {
            utfstate_t state;
            auto value_range = make_range(&value, &value + 1);
            auto result = utf8_to_utf32(make_generator(r), make_consumer(value_range), state);
            if (result != utf_result::ok)
                value = UNICODE_REPLACEMENT_CHARACTER;
        }

    private:
        range r;
        char32_t value;
    };

    struct to_utf8_tag { };
    struct to_utf16_tag { };
    struct to_utf32_tag { };

    inline auto to_utf8() { return to_utf8_tag(); }
    inline auto to_utf16() { return to_utf16_tag(); }
    inline auto to_utf32() { return to_utf32_tag(); }

    template <class Range, REQUIRES(::std::is_same<::std::remove_cv_t<value_type<Range, is_range>>, char>::value)>
    auto operator >> (::std::remove_reference_t<Range>&& range, to_utf8_tag)
    {
        return ::std::move(range);
    }
    template <class Range, REQUIRES(::std::is_same<::std::remove_cv_t<value_type<Range, is_range>>, char>::value)>
    auto& operator >> (const ::std::remove_reference_t<Range>& range, to_utf8_tag)
    {
        return range;
    }

    template <class Range, REQUIRES(::std::is_same<::std::remove_cv_t<value_type<Range, is_range>>, char>::value)>
    auto operator >> (Range&& range, to_utf32_tag)
    {
        return utf8_to_utf32_generator<::std::decay_t<Range>>(::std::forward<Range>(range));
    }
}

#endif
