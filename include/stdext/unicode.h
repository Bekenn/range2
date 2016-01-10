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

namespace stdext
{
    enum class utf_result
    {
        ok,
        partial,
        error
    };

    constexpr char32_t UNICODE_REPLACEMENT_CHARACTER = 0xFFFD;
    constexpr char32_t INVALID_UNICODE_CHARACTER = char32_t(-1);
    constexpr ::std::size_t MAX_UTF8_CHARACTER_LENGTH = 4;

    ::std::size_t utf8_character_length(char first)
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

    bool utf8_is_leading(char code)
    {
        return (code & 0xC0) != 0x80 && (code & 0xF8) != 0xF8;
    }

    bool utf8_is_trailing(char code)
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
    // The first time this function is called for a given input sequence, state should be initialized to zero.
    // On return, in_first points one position beyond the last input code unit consumed, out_first points one position
    // beyond the last output code unit produced, and state contains partial conversion information, if any, for the last
    // input character sequence.
    // If the function returns utf_result::ok, state == 0 and in_first == in_last.
    // If the function returns utf_result::partial, state contains partial conversion information and either
    // in_first == in_last or out_first == out_last.
    // If the function returns utf_result::error, state == 0 and in_first points one position beyond the input character
    // sequence that could not be converted.
    // The behavior is undefined if state != 0 or if state does not contain a value from a previous call to this function.
    utf_result utf8_to_utf32(const char*& in_first, const char* in_last, char32_t*& out_first, char32_t* out_last, uint32_t& state)
    {
        constexpr unsigned REMAINING_SHIFT = 24;
        constexpr unsigned CONSUMED_SHIFT = 28;

        if (in_first == in_last)
            return state == 0 ? utf_result::ok : utf_result::partial;

        char32_t utf32 = state & 0x00FFFFFF;
        unsigned remaining = (state >> REMAINING_SHIFT) & 0x0F;
        unsigned consumed = (state >> CONSUMED_SHIFT) & 0x0F;

        switch (consumed)
        {
            while (in_first != in_last)
            {
        case 0:
                {
                    auto code = ::std::uint8_t(*in_first++);
                    consumed = 1;

                    if ((code & 0x80) == 0)
                    {
                        utf32 = code;
                        remaining = 0;
                    }
                    else if ((code & 0xE0) == 0xC0)
                    {
                        utf32 = code & 0x1F;
                        remaining = 1;
                    }
                    else if ((code & 0xF0) == 0xE0)
                    {
                        utf32 = code & 0x0F;
                        remaining = 2;
                    }
                    else if ((code & 0xF8) == 0xF0)
                    {
                        utf32 = code & 0x07;
                        remaining = 3;
                    }
                    else
                    {
                        state = 0;
                        return utf_result::error;
                    }

                    if (in_first == in_last)
                    {
                        state = (consumed << CONSUMED_SHIFT)
                            | (remaining << REMAINING_SHIFT)
                            | utf32;
                        return utf_result::partial;
                    }
                }

        // fall through
        case 1:
                switch (remaining)
                {
                case 2:
                    if ((utf32 == 0x00 && *in_first < 0xA0)
                        || (utf32 == 0x0D && *in_first >= 0xA0))
                    {
                        state = 0;
                        return utf_result::error;
                    }
                    break;
                case 3:
                    if ((utf32 == 0x00 && *in_first < 0x90)
                        || (utf32 == 0x04 && *in_first >= 0x90))
                    {
                        state = 0;
                        return utf_result::error;
                    }
                    break;

                default:
                    break;
                }

        // fall through
        default:
                while (remaining != 0)
                {
                    if (in_first == in_last)
                    {
                        state = (consumed << CONSUMED_SHIFT)
                            | (remaining << REMAINING_SHIFT)
                            | utf32;
                        return utf_result::partial;
                    }

                    auto code = *in_first;
                    if ((code & 0xC0) != 0x80)
                    {
                        state = 0;
                        return utf_result::error;
                    }

                    ++in_first;
                    ++consumed;
                    --remaining;

                    utf32 <<= 6;
                    utf32 |= code & 0x3F;
                }

                if (out_first == out_last)
                {
                    state = (consumed << CONSUMED_SHIFT)
                        | (remaining << REMAINING_SHIFT)
                        | utf32;
                    return utf_result::partial;
                }
                *out_first++ = utf32;
            }
        }

        state = 0;
        return utf_result::ok;
    }

    utf_result utf16_to_utf32(const char16_t*& in_first, const char16_t* in_last, char32_t*& out_first, char32_t* out_last, uint32_t& state)
    {
        constexpr unsigned CONSUMED_SHIFT = 24;

        if (in_first == in_last)
            return state == 0 ? utf_result::ok : utf_result::partial;

        char32_t utf32 = state;
        unsigned consumed = utf32 >> CONSUMED_SHIFT;

        switch (consumed)
        {
            while (in_first != in_last)
            {
        case 0:
                utf32 = *in_first++;
                consumed = 1;

                if (is_leading_surrogate(char16_t(utf32)))
                {
                    utf32 &= 0x03FF;
                    utf32 += 0x40;

                    if (in_first == in_last)
                    {
                        state = utf32;
                        return utf_result::partial;
                    }

        // fall through
        case 1:
                    auto code = *in_first;
                    if (!is_trailing_surrogate(code))
                    {
                        state = 0;
                        return utf_result::error;
                    }
                    ++in_first;
                    ++consumed;

                    utf32 <<= 10;
                    utf32 |= code & 0x03FF;
                }

        // fall through
        default:
                if (out_first == out_last)
                {
                    state = utf32
                        | (consumed << CONSUMED_SHIFT);
                    return utf_result::partial;
                }

                *out_first++ = utf32;
            }
        }

        state = 0;
        return utf_result::ok;
    }

    utf_result utf32_to_utf8(const char32_t*& in_first, const char32_t* in_last, char*& out_first, char* out_last, uint32_t& state)
    {
        constexpr unsigned REMAINING_SHIFT = 24;
        constexpr unsigned PRODUCED_SHIFT = 28;

        char32_t code = state & 0x00FFFFFF;
        unsigned remaining = (state >> REMAINING_SHIFT) & 0x0F;
        unsigned produced = (state >> PRODUCED_SHIFT) & 0x0F;

        switch (produced)
        {
        case 0:
            while (in_first != in_last)
            {
                code = *in_first++;
                if (code >= 0x110000 || (code < 0x10000 && is_surrogate(char16_t(code))))
                {
                    state = 0;
                    return utf_result::error;
                }

                remaining = code < 0x80 ? 0
                    : code < 0x800 ? 1
                    : code < 0x1000 ? 2
                    : 3;

                switch (remaining)
                {
                case 0:
                    *out_first++ = char(code);
                    break;
                case 1:
                    *out_first++ = char(0xC0 | (code >> 6));
                    break;
                case 2:
                    *out_first++ = char(0xE0 | (code >> 12));
                    break;
                case 3:
                    *out_first++ = char(0xF0 | (code >> 18));
                    break;
                }
                produced = 1;

                while (remaining != 0)
                {
        default:
                    if (out_first == out_last)
                    {
                        state = (produced << PRODUCED_SHIFT)
                            | (remaining << REMAINING_SHIFT)
                            | code;
                        return utf_result::partial;
                    }

                    --remaining;
                    *out_first++ = char(0x80 | ((code >> 6 * remaining) & 0x3F));
                }
            }
        }

        state = 0;
        return utf_result::ok;
    }

    utf_result utf32_to_utf16(const char32_t*& in_first, const char32_t* in_last, char16_t*& out_first, char16_t* out_last, uint32_t& state)
    {
        constexpr unsigned PRODUCED_SHIFT = 24;

        char32_t code = state & 0x00FFFFFF;
        unsigned produced = state >> PRODUCED_SHIFT;

        switch (produced)
        {
        case 0:
            while (in_first != in_last)
            {
                code = *in_first++;
                if (code >= 0x110000 || (code < 0x10000 && is_surrogate(char16_t(code))))
                {
                    state = 0;
                    return utf_result::error;
                }

                if (out_first == out_last)
                {
                    state = (produced << PRODUCED_SHIFT)
                        | code;
                    return utf_result::partial;
                }

                if (code < 0x10000)
                    *out_first++ = char16_t(code);
                else
                {
                    *out_first++ = char16_t(0xD800 | ((code >> 16) - 1));
                    produced = 1;

        default:
                    if (out_first == out_last)
                    {
                        state = (produced << PRODUCED_SHIFT)
                            | code;
                        return utf_result::partial;
                    }

                    *out_first++ = char16_t(0xDC00 | (code & 0xFFFF));
                }
            }
        }

        state = 0;
        return utf_result::ok;
    }

    template <class Range>
    class utf8_to_utf32_range : private compressed_base<position_type<Range, is_range>>
    {
    public:
        using range_category = single_pass_range_tag;
        using value_type = const char32_t;
        using position = position_type<Range, is_range>;
        using difference_type = difference_type<Range, is_range>;
        using reference = value_type&;
        using range = Range;

    private:
        using base = compressed_base<position_type<Range, is_range>>;

    public:
        explicit utf8_to_utf32_range(const range& wrapped) : base(wrapped.begin_pos()), wrapped(wrapped), value()
        {
            next();
        }
        explicit utf8_to_utf32_range(range&& wrapped) : base(wrapped.begin_pos()), wrapped(::std::move(wrapped)), value()
        {
            next();
        }

        friend bool operator == (const utf8_to_utf32_range& a, const utf8_to_utf32_range& b) noexcept
        {
            return a.wrapped == b.wrapped
                && a.value == b.value;
        }
        friend bool operator != (const utf8_to_utf32_range& a, const utf8_to_utf32_range& b) noexcept
        {
            return !(a == b);
        }

    public:
        position begin_pos() const { return *this; }
        void begin_pos(position pos)
        {
            wrapped.begin_pos(pos);
            next();
        }
        bool is_end_pos(position pos) { value == INVALID_UNICODE_CHARACTER; }
        reference at_pos(position pos) { return value; }

        position& inc_pos(position& pos)
        {
            next();
            pos = begin_pos();
            return pos;
        }

    private:
        void next()
        {
            auto pos = wrapped.begin_pos();
            *this = pos;
            if (wrapped.is_end_pos(pos))
            {
                value = INVALID_UNICODE_CHARACTER;
                break;
            }

            uint32_t state = 0;
            auto result = utf_result::error;
            do
            {
                char utf8 = wrapped.at_pos(pos);
                wrapped.inc_pos(pos);

                auto putf8 = &utf8;
                auto putf32 = &value;

                result = utf8_to_utf32(putf8, putf8 + 1, putf32, putf32 + 1, state);
            } while (result == utf_result::partial && !wrapped.is_end_pos(pos));

            if (result != utf_result::ok)
                value = UNICODE_REPLACEMENT_CHARACTER;
        }

    private:
        range wrapped;
        char32_t value;
    };

    struct to_utf8_tag { };
    struct to_utf16_tag { };
    struct to_utf32_tag { };

    auto to_utf8() { return to_utf8_tag(); }
    auto to_utf16() { return to_utf16_tag(); }
    auto to_utf32() { return to_utf32_tag(); }

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
        return utf8_to_utf32_range<::std::decay_t<Range>>(::std::forward<Range>(range));
    }
}

#endif
