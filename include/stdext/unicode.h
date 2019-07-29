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

#include <stdext/range.h>
#include <stdext/string.h>

#include <string>
#include <tuple>


namespace stdext
{
    template <class T>
    struct is_unicode_character_type
    {
        static constexpr bool value = ::std::is_same_v<T, char>
            || ::std::is_same_v<T, char16_t>
            || ::std::is_same_v<T, char32_t>;
    };
    template <class T> constexpr bool is_unicode_character_type_v = is_unicode_character_type<T>::value;

    enum class utf_result
    {
        ok,
        partial_read,
        partial_write,
        error
    };

    struct utfstate_t
    {
        uint32_t
            code : 24,
            produced : 2,
            consumed : 2,
            remaining : 2,
            error : 1;

        utfstate_t() : code(0), produced(0), consumed(0), remaining(0), error(0) { }
    };

    constexpr char32_t UNICODE_REPLACEMENT_CHARACTER = 0xFFFD;
    constexpr char32_t INVALID_UNICODE_CHARACTER = char32_t(-1);
    constexpr size_t MAX_UTF8_CHARACTER_LENGTH = 4;

    constexpr size_t utf8_character_length(char first)
    {
        return (first & 0x80) == char(0x00) ? 1
            : (first & 0xE0) == char(0xC0) && uint8_t(first) >= 0xC2 ? 2
            : (first & 0xF0) == char(0xE0) ? 3
            : (first & 0xF8) == char(0xF0) && uint8_t(first) < 0xF5 ? 4
            : 0;
    }

    constexpr bool utf8_is_leading(char code)
    {
        return uint8_t(code) < 0x80 || (uint8_t(code) >= 0xC2 && uint8_t(code) < 0xF5);
    }

    constexpr bool utf8_is_trailing(char code)
    {
        return (code & 0xC0) == 0x80;
    }

    constexpr bool is_surrogate(char16_t code)
    {
        return (code & 0xF800) == 0xD800;
    }

    constexpr bool is_leading_surrogate(char16_t code)
    {
        return (code & 0xFC00) == 0xD800;
    }

    constexpr bool is_trailing_surrogate(char16_t code)
    {
        return (code & 0xFC00) == 0xDC00;
    }

    constexpr bool is_noncharacter(char32_t code)
    {
        return (code & 0xFFFE) == 0xFFFE || (code >= 0xFDD0 && code < 0xFDF0);
    }

    constexpr bool utf32_is_valid(char32_t code)
    {
        return code < 0x110000
            && (code >= 0x10000 || !is_surrogate(static_cast<char16_t>(code)))
            && !is_noncharacter(code);
    }

    // Converts code units between Unicode encodings.  Each function is intended to be called repeatedly for a single code
    // point until all input code units have been consumed and all output code units have been produced.  Conversion occurs
    // in two stages, each of which may involve multiple invocations.  In the first stage, input code units are consumed
    // until a code point can be determined from the input sequence.  In the second stage, output code units are produced
    // until the code point is fully represented.
    // The conversion sequence begins by calling one of the conversion functions with the initial input code unit and an
    // empty (default-constructed) utfstate_t instance.  The function will return utf_result::partial_read throughout the
    // first stage; the caller should respond by invoking the function again with the next input code unit in the sequence
    // and the same state object.  When the first stage is complete and the second stage begins, the function will return
    // utf_result::partial_write or utf_result::ok along with the initial output code unit.  A utf_result::partial_write
    // result indicates that the conversion sequence is not complete and that the function should be called again (passing
    // the same state object; the in argument is ignored) to retrieve additional code units.  A utf_result::ok result
    // accompanies the final output code unit and indicates that the conversion sequence is complete; at this point, the
    // state argument has been returned to the empty state and may be reused to begin another conversion sequence.
    // The function may return utf_result::error at any point during the first stage of the conversion process to indicate
    // that the input code sequences do not represent a valid Unicode code point.
    // The behavior is undefined if state is non-empty and does not contain a value from a previous call to the function.
    ::std::pair<utf_result, char> to_utf8(char in, utfstate_t& state);
    ::std::pair<utf_result, char> to_utf8(char16_t in, utfstate_t& state);
    ::std::pair<utf_result, char> to_utf8(char32_t in, utfstate_t& state);
    ::std::pair<utf_result, char16_t> to_utf16(char in, utfstate_t& state);
    ::std::pair<utf_result, char16_t> to_utf16(char16_t in, utfstate_t& state);
    ::std::pair<utf_result, char16_t> to_utf16(char32_t in, utfstate_t& state);
    ::std::pair<utf_result, char32_t> to_utf32(char in, utfstate_t& state);
    ::std::pair<utf_result, char32_t> to_utf32(char16_t in, utfstate_t& state);
    ::std::pair<utf_result, char32_t> to_utf32(char32_t in, utfstate_t& state);

    namespace detail
    {
        template <class Char> struct utf_sentinel;
        template <>
        struct utf_sentinel<char>
        {
            static constexpr char value = '\xFF';
        };
        template <>
        struct utf_sentinel<char16_t>
        {
            static constexpr char16_t value = u'\xFFFF';
        };
        template <>
        struct utf_sentinel<char32_t>
        {
            static constexpr char32_t value = U'\x10FFFF';
        };
        template <class Char> constexpr Char utf_sentinel_v = utf_sentinel<Char>::value;

        // It would be really nice to have function template partial specialization...
        template <class OutChar, class InChar> struct to_utf_helper;
        template <class InChar>
        struct to_utf_helper<char, InChar>
        {
            template <class... Args> static auto call(Args&&... args) { return to_utf8(forward<Args>(args)...); }
        };
        template <class InChar>
        struct to_utf_helper<char16_t, InChar>
        {
            template <class... Args> static auto call(Args&&... args) { return to_utf16(forward<Args>(args)...); }
        };
        template <class InChar>
        struct to_utf_helper<char32_t, InChar>
        {
            template <class... Args> static auto call(Args&&... args) { return to_utf32(forward<Args>(args)...); }
        };

        template <class OutChar, class InChar>
        ::std::pair<utf_result, OutChar> to_utf(InChar in, utfstate_t& state)
        {
            return to_utf_helper<OutChar, InChar>::call(in, state);
        }

        template <class Char, class Generator, class Consumer,
            STDEXT_REQUIRES(is_generator_v<::std::decay_t<Generator>> && is_consumer_v<::std::decay_t<Consumer>(Char)>)>
        utf_result to_utf(Generator&& in, Consumer&& out, utfstate_t& state)
        {
            auto result = utf_result::ok;

            while (in)
            {
                Char code;
                ::std::tie(result, code) = to_utf<Char>(*in, state);
                if (result == utf_result::error)
                    return utf_result::error;
                if (result != utf_result::partial_write)
                    ++in;
                if (result != utf_result::partial_read)
                {
                    if (!out(code))
                        return utf_result::partial_write;
                }
            }

            return result;
        }

        template <class Generator, class Function>
        ::std::pair<utf_result, size_t> to_utf_length(Generator&& in, Function&& convert)
        {
            auto result = ::std::make_pair(utf_result::ok, size_t(0));
            utfstate_t state;
            while (in)
            {
                result.first = convert(*in, state).first;
                if (result.first == utf_result::error)
                    break;
                if (result.first != utf_result::partial_write)
                    ++in;
                if (result.first != utf_result::partial_read)
                    ++result.second;
            }
            return result;
        }

        template <class Char, class Generator>
        ::std::pair<utf_result, ::std::basic_string<Char>> to_ustring(Generator&& in)
        {
            ::std::basic_string<Char> str;
            utfstate_t state;
            return { to_utf<Char>(::std::forward<Generator>(in), make_consumer<Char>(::std::back_inserter(str)), state), move(str) };
        }
    }

    template <class Generator, class Consumer,
        STDEXT_REQUIRES(is_consumer_v<::std::decay_t<Consumer>(value_type_t<::std::decay_t<Generator>, can_generate>)>)>
    utf_result to_utf8(Generator&& in, Consumer&& out, utfstate_t& state)
    {
        return detail::to_utf<char>(as_generator(forward<Generator>(in)), forward<Consumer>(out), state);
    }

    template <class Char, class Consumer, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>> && is_consumer_v<::std::decay_t<Consumer>(Char)>)>
    utf_result to_utf8(Char* s, Consumer&& out)
    {
        utfstate_t state;
        return detail::to_utf<char>(make_cstring_generator(s), forward<Consumer>(out), state);
    }

    template <class Generator, class Consumer,
        STDEXT_REQUIRES(is_consumer_v<::std::decay_t<Consumer>(value_type_t<::std::decay_t<Generator>, can_generate>)>)>
    utf_result to_utf16(Generator&& in, Consumer&& out, utfstate_t& state)
    {
        return detail::to_utf<char16_t>(as_generator(forward<Generator>(in)), forward<Consumer>(out), state);
    }

    template <class Char, class Consumer, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    utf_result to_utf16(Char* s, Consumer&& out)
    {
        utfstate_t state;
        return detail::to_utf<char16_t>(make_cstring_generator(s), forward<Consumer>(out), state);
    }

    template <class Generator, class Consumer,
        STDEXT_REQUIRES(is_consumer_v<::std::decay_t<Consumer>(value_type_t<::std::decay_t<Generator>, can_generate>)>)>
    utf_result to_utf32(Generator&& in, Consumer&& out, utfstate_t& state)
    {
        return detail::to_utf<char32_t>(as_generator(forward<Generator>(in)), forward<Consumer>(out), state);
    }

    template <class Char, class Consumer, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    utf_result to_utf32(Char* s, Consumer&& out)
    {
        utfstate_t state;
        return detail::to_utf<char32_t>(make_cstring_generator(s), forward<Consumer>(out), state);
    }

    template <class Generator, STDEXT_REQUIRES(can_generate_v<::std::decay_t<Generator>>)>
    ::std::pair<utf_result, size_t> to_utf8_length(Generator&& in)
    {
        return detail::to_utf_length(as_generator(forward<Generator>(in)),
            [](auto&&... args) { return to_utf8(forward<decltype(args)>(args)...); });
    }

    template <class Char, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    ::std::pair<utf_result, size_t> to_utf8_length(Char* str)
    {
        return to_utf8_length(make_cstring_generator(str));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate_v<::std::decay_t<Generator>>)>
    ::std::pair<utf_result, size_t> to_utf16_length(Generator&& in)
    {
        return detail::to_utf_length(as_generator(forward<Generator>(in)),
            [](auto&&... args) { return to_utf16(forward<decltype(args)>(args)...); });
    }

    template <class Char, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    ::std::pair<utf_result, size_t> to_utf16_length(Char* str)
    {
        return to_utf16_length(make_cstring_generator(str));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate_v<::std::decay_t<Generator>>)>
    ::std::pair<utf_result, size_t> to_utf32_length(Generator&& in)
    {
        return detail::to_utf_length(as_generator(forward<Generator>(in)),
            [](auto&&... args) { return to_utf32(forward<decltype(args)>(args)...); });
    }

    template <class Char, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    ::std::pair<utf_result, size_t> to_utf32_length(Char* str)
    {
        return to_utf32_length(make_cstring_generator(str));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate_v<::std::decay_t<Generator>>)>
    ::std::pair<utf_result, ::std::string> to_u8string(Generator&& in)
    {
        return detail::to_ustring<char>(forward<Generator>(in));
    }

    template <class Char, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    ::std::pair<utf_result, ::std::string> to_u8string(Char* str)
    {
        return to_u8string(make_cstring_generator(str));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate_v<::std::decay_t<Generator>>)>
    ::std::pair<utf_result, ::std::u16string> to_u16string(Generator&& in)
    {
        return detail::to_ustring<char16_t>(forward<Generator>(in));
    }

    template <class Char, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    ::std::pair<utf_result, ::std::u16string> to_u16string(Char* str)
    {
        return to_u16string(make_cstring_generator(str));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate_v<::std::decay_t<Generator>>)>
    ::std::pair<utf_result, ::std::u32string> to_u32string(Generator&& in)
    {
        return detail::to_ustring<char32_t>(forward<Generator>(in));
    }

    template <class Char, STDEXT_REQUIRES(is_unicode_character_type_v<::std::decay_t<Char>>)>
    ::std::pair<utf_result, ::std::u32string> to_u32string(Char* str)
    {
        return to_u32string(make_cstring_generator(str));
    }

    template <class Generator, class Char>
    class to_utf_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = Char;
        using difference_type = generator_difference_type_t<Generator>;
        using pointer = const value_type*;
        using reference = const value_type&;
        using generator = Generator;

    public:
        to_utf_generator() : g(), value() { }
        explicit to_utf_generator(const generator& g) : g(g)
        {
            next();
        }
        explicit to_utf_generator(generator&& g) : g(move(g))
        {
            next();
        }

    public:
        friend bool operator == (const to_utf_generator& a, const to_utf_generator& b) noexcept
        {
            return a.g == b.g
                && a.value == b.value;
        }
        friend bool operator != (const to_utf_generator& a, const to_utf_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(to_utf_generator& a, to_utf_generator& b)
        {
            swap(a.g, b.g);
            swap(a.value, b.value);
        }

    public:
        reference operator * () { return value; }
        reference operator -> () { return value; }
        to_utf_generator& operator ++ () { next(); return *this; }
        iterator_proxy<to_utf_generator> operator ++ (int)
        {
            iterator_proxy<to_utf_generator> proxy = value;
            next();
            return proxy;
        }

        explicit operator bool () const noexcept
        {
            return value != detail::utf_sentinel_v<Char>;
        }

    private:
        void next()
        {
            if (state.error)
            {
                auto result = detail::to_utf<Char>(UNICODE_REPLACEMENT_CHARACTER, state);
                value = result.second;
                if (result.first == utf_result::ok)
                    state.error = 0;
                return;
            }

            if (!g)
            {
                if (state.produced != 0)
                {
                    // The code point is in state, the code unit that we pass doesn't matter.
                    auto result = detail::to_utf<Char>(UNICODE_REPLACEMENT_CHARACTER, state);
                    value = result.second;
                    return;
                }

                // End of sequence.
                value = detail::utf_sentinel_v<Char>;
                return;
            }

            do
            {
                auto result = detail::to_utf<Char>(*g, state);
                switch (result.first)
                {
                case utf_result::ok:
                    ++g;
                    value = result.second;
                    return;
                case utf_result::partial_read:
                    ++g;
                    continue;
                case utf_result::partial_write:
                    value = result.second;
                    return;
                case utf_result::error:
                    if (state.consumed == 0)
                        ++g;
                    state = utfstate_t();
                    result = detail::to_utf<Char>(UNICODE_REPLACEMENT_CHARACTER, state);
                    value = result.second;
                    if (result.first != utf_result::ok)
                        state.error = 1;
                    return;
                }
            } while (g);

            // Ran out of characters during a partial read.
            state = utfstate_t();
            auto result = detail::to_utf<Char>(UNICODE_REPLACEMENT_CHARACTER, state);
            value = result.second;
        }

    private:
        generator g;
        utfstate_t state;
        Char value;
    };

    struct to_utf8_tag { };
    struct to_utf16_tag { };
    struct to_utf32_tag { };

    constexpr auto to_utf8() { return to_utf8_tag(); }
    constexpr auto to_utf16() { return to_utf16_tag(); }
    constexpr auto to_utf32() { return to_utf32_tag(); }

    template <class Generator, STDEXT_REQUIRES(can_generate<::std::decay_t<Generator>>::value)>
    auto operator >> (Generator&& g, to_utf8_tag)
    {
        return to_utf_generator<generator_type_t<::std::decay_t<Generator>, can_generate>, char>(as_generator(forward<Generator>(g)));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate<::std::decay_t<Generator>>::value)>
    auto operator >> (Generator&& g, to_utf16_tag)
    {
        return to_utf_generator<generator_type_t<::std::decay_t<Generator>, can_generate>, char16_t>(as_generator(forward<Generator>(g)));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate<::std::decay_t<Generator>>::value)>
    auto operator >> (Generator&& g, to_utf32_tag)
    {
        return to_utf_generator<generator_type_t<::std::decay_t<Generator>, can_generate>, char32_t>(as_generator(forward<Generator>(g)));
    }
}

#endif
